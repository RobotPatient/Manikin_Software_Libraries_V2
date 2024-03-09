/* *******************************************************************************************
 * Copyright (c) 2023 by RobotPatient Simulators
 *
 * Authors: Victor Hogeweij
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction,
 *
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so,
 *
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
***********************************************************************************************/
#include <usb_service_protocol.hpp>
#include "string.h"
#include "tusb.h"

namespace usb_service_protocol {
inline constexpr uint8_t kReadBufferSize = 100;
inline constexpr char kTerminalEntryCharacter = '>';
inline constexpr char kNewLineCharacter = '\n';
inline constexpr uint8_t kCarriageReturnCharacter = '\r';
inline constexpr uint8_t kStrTerminationCharacter = '\0';
inline constexpr uint8_t kSpaceCharacter = ' ';
inline constexpr uint8_t kBackspaceAsciiCode = 127;
inline constexpr int kNoNewCharacter = -1;
inline constexpr char kPrintOnNextLine[] = "\r\n";

/**
 * @brief Read index for the read buffer
 * This variable is used to keep track of the amount of elements in the read buffer
 */
static uint8_t read_index = 0;

static usb_serial_inst_t serial_inst;

/**
 * @brief Dimensions of the buffer that the task being created will use as its stack.
 * 
 * @note This is the number of words the stack will hold, not the number of bytes.  
 * For example, if each stack item is 32-bits, and this is set to 100,
 * then 400 bytes (100 * 32-bits) will be allocated. 
 */
inline constexpr int kReadTaskStackSize = 200;

/**
 * @brief The priority given to the ReadTask
 * 
 * @note The priority can be set to a number within 0-32,
 * 0 highest priority, 32 lowest priority
 */
inline constexpr uint8_t kReadTaskPriority = 5;

/**
 * @brief Structure that will hold the TCB of the task being created
 * 
 */
StaticTask_t staticReadTask;

/**
 * @brief Buffer that the task being created will use as its stack
 * 
 * @note This is an array of StackType_t variables.  
 * The size of StackType_t is dependent on the RTOS port.
 */
StackType_t ReadTaskStack[kReadTaskStackSize];

/**
 * @brief Struct containing parsed arguments
 * 
 * This struct is used to pass around parsed arguments from the parser function
 * to the command interpreter.
 * 
 */
typedef struct {
  char* argsBuffer[kUSBProtoMaxAmountOfArguments];
  uint8_t argNum;
} ParsedArgs;

/**
 * @brief Internal register definitions (exact copy of the parameters given in Init function)
 * 
 * @note serviceProtocolRegister contains a pointer to the array containing the registers used by the usbregister protocol
 *       NumRegisters contains the number of registers in the pointer array.
 */
USBServiceProtocolRegisters* serviceProtocolRegisters = NULL;
uint8_t NumRegisters = 0;

/**
 * @brief This variable contains the Last executed command/register
 * It is needed to remember if previous register was stream cmd
 */
USBServiceProtocolRegisters* LastRegister;

/**
 * @brief This function replaces spaces in the input string with terminated \0 character
 * 
 * @param input The \0 terminated string to be parsed
 * 
 * @return NULL if no space was found or end of buffer reached. Char* pointer to location after space
 * 
 * @note Output of this function will look like this:
 *       inpur: "CMD ARG"
 *       AFTER RUNNING the function (input buffer will be altered by function):
 *       input: "CMD\0ARG"
 *       function will return: Pointer to "ARG" section of string.
 */
constexpr char* Getarg(char* input) {
  if (input == NULL)
    return NULL;
  // Loop over the passed in string,
  // till space-, termination- or carriage-return character encountered
  do {
    input++;
  } while (*input != kSpaceCharacter && *input != kStrTerminationCharacter && *input != kCarriageReturnCharacter);
  // Termination char means end of the string
  if (*input == '\0')
    return NULL;
  // The reason for inserting the STR termination char
  // Is to force other c functions like printf to stop reading the
  // string at the place where the space was..
  // This way we get a nice formatted string of which we only have to
  // save the pointers of the places in the string where arguments are saved :)
  *input = kStrTerminationCharacter;
  // This is the pointer to a new possible argument, not the one we found..
  return input + 1;
}

/**
 * @brief Parse the arguments out of string entered by user in console
 * 
 * @note This function uses the Getarg function to get all arguments and puts them in the 
 *       Parsedarg struct.
 * 
 * @param buffer \0 terminated string entered by user in console containing zero or more arguments
 * @return ParsedArgs Struct containing pointers to all the locations of the arguments in the string.
 */
ParsedArgs Parsearg(char* buffer) {
  ParsedArgs Args;
  Args.argNum = 0;
  Args.argsBuffer[0] = Getarg(buffer);
  const bool more_than_one_argument_entered = Args.argsBuffer[0] != NULL;
  if (more_than_one_argument_entered) {
    // There are more arguments.. Loop over the readbuffer searching for more arguments (whitespaces)
    for (uint8_t argNum = 1; argNum < kUSBProtoMaxAmountOfArguments; argNum++) {
      Args.argsBuffer[argNum] = Getarg(Args.argsBuffer[argNum - 1]);
      if (Args.argsBuffer[argNum] == NULL)
        break;
      else
        Args.argNum++;
    }
  }
  return Args;
}

/**
 * @brief Parse the command and arguments and run the command entered by user in console
 * 
 * @note The arguments are parsed first then the command.. 
 * Parsing the arguments terminates the readbuffer at the first argument place 
 * (if arguments entered, spaces will be replaced with \0), making reading the command easier
 * Then a linear search is done to lookup the command in the register descriptions array (ServiceProtocolRegisters).
 * If the command name matches the command argument, the number of arguments is evaluated and the cb function is executed!
 * 
 * @param buffer This buffer contains the user entered \0 terminated string
 * @return The command response of the command ran or error message (with unrecognized commmand, too few, too many arguments)
 */
const char* Runcmd(char* buffer) {
  ParsedArgs args = Parsearg(buffer);
  for (uint8_t command_index = 0; command_index < NumRegisters; command_index++) {
    const bool command_found = (strcmp(serviceProtocolRegisters[command_index].cmd_, buffer) == 0);
    if (command_found) {
      const bool too_many_arguments = args.argNum > serviceProtocolRegisters[command_index].num_of_args_;
      const bool too_few_arguments = (args.argNum < serviceProtocolRegisters[command_index].num_of_args_);
      if (too_many_arguments) {
        return "!E Too many arguments!";
      } else if (too_few_arguments) {
        return "!E Too few arguments!";
      } else {
        LastRegister = &serviceProtocolRegisters[command_index];
        return serviceProtocolRegisters[command_index].cmd_cb(args.argsBuffer, args.argNum);
      }
      break;
    }
  }
  return "!E Command unrecognized!";
}

/**
 * @brief ParseInput parses the incoming characters
 * 
 * @note ParseInput takes the character, read_buffer and stream command parameters from the ReadTask
 * and decides on basis of the character coming in: 
 * - To run the input command from the readbuffer (Carriage return character)
 * - Remove character from the readbuffer (Backspace character)
 * - Add character to readbuffer (any other character)
 * 
 * Edge case: Previous command is stream command.. When stream command is interrupted
 * an enter has to be entered in the serial console while running. 
 * But if the carriage return is parsed like normal, it would run an empty command.
 * Resulting in an command unrecognized error!
 * Thats why an extra if statement is added... To catch this specific condition
 * 
 * @param read_buffer Pointer to the read_buffer containing the previous user input
 * @param character The new incoming character
 * @param last_command_was_stream_cmd Boolean indicating whether the previous ran command is a streaming command
 */
inline void ParseInput(char* read_buffer, int character, const bool last_command_was_stream_cmd) {
  switch (character) {
    case kBackspaceAsciiCode: {
      // Backspace means removing previous character in buffer
      // And moving the cursor back 1 position
      read_buffer[read_index] = kStrTerminationCharacter;
      const bool buffer_not_empty = (read_index != 0);
      if (buffer_not_empty) {
        // Serial.write(character);
        tud_cdc_n_write_char(serial_inst, character);
        tud_cdc_n_write_flush(serial_inst);
        read_index--;
      }
      break;
    }
    case kCarriageReturnCharacter: {
      tud_cdc_n_write_char(serial_inst, kNewLineCharacter);
      tud_cdc_n_write_char(serial_inst, character); // Now print the \r so the cursor pos is at the beginning of the line
      tud_cdc_n_write_flush(serial_inst);
      read_buffer[read_index++] = character;
      // We of course don't want to run the entered empty command when a stream command is interrupted by enter key
      if (last_command_was_stream_cmd) {
        LastRegister = NULL;
      } else {
        const char* command_output = Runcmd(read_buffer);
        usb_serial_write_string(serial_inst, command_output);
        tud_cdc_n_write_char(serial_inst, kNewLineCharacter);
        tud_cdc_n_write_char(serial_inst, character);
        tud_cdc_n_write_flush(serial_inst);
      }
      tud_cdc_n_write_char(serial_inst, kTerminalEntryCharacter);
      tud_cdc_n_write_flush(serial_inst);
      memset(read_buffer, kStrTerminationCharacter, read_index);
      read_index = 0;
      break;
    }
    default: {
      if (read_index < kReadBufferSize) {
        tud_cdc_n_write_char(serial_inst, character);
        tud_cdc_n_write_flush(serial_inst);
        read_buffer[read_index++] = character;
      }
      break;
    }
  }
}

/**
 * @brief  This task will continuously monitor the serial console for incoming characters.
 * Or run the previous command if the command is stream command.
 *  (Stream commands are commands that run continuously until enter key pressed).
 * 
 * @param pvParameters function parameters forwarded from its declaration in the init function.
 */
void ReadTask(void* pvParameters) {
  static char read_buffer[kReadBufferSize];
  int character = kNoNewCharacter;
  // Buffers are never terminated when uninitialized.
  // If not memset our parsing functions will not work!
  memset(read_buffer, kStrTerminationCharacter, kReadBufferSize);
  // Wait for serial to be opened
  while (usb_serial_available(serial_inst) == 0) {
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }

  // This gives our lovely '>' character at the beginning of terminal
  tud_cdc_n_write_char(serial_inst, kTerminalEntryCharacter);
  tud_cdc_n_write_flush(serial_inst);
  for (;;) {
     character = usb_serial_read_char(serial_inst);
     bool last_command_was_stream_cmd = LastRegister != NULL ? LastRegister->stream_cmd_ : false;
     bool new_character = character != kNoNewCharacter;
    if (new_character) {
       ParseInput(read_buffer, character, last_command_was_stream_cmd);
    } else if (last_command_was_stream_cmd) {
       const char* stream_cmd_output = LastRegister->cmd_cb(NULL, 0);
       usb_serial_write_string(serial_inst, stream_cmd_output);
       usb_serial_write_string(serial_inst, kPrintOnNextLine);
    }
    vTaskDelay(10/portTICK_PERIOD_MS);
  }
}

void Init(const usb_serial_inst_t usb_inst, USBServiceProtocolRegisters* registers, const size_t num_of_registers) {
  serviceProtocolRegisters = registers;
  NumRegisters = num_of_registers;
  serial_inst = usb_inst;
}

void SetPollingTask(TaskHandle_t* task_handle) {
  const bool USBServiceProtocolIsInitialized = (serviceProtocolRegisters != NULL && NumRegisters > 0);
  if (USBServiceProtocolIsInitialized) {
    /* Create the task without using any dynamic memory allocation. */
    *task_handle = xTaskCreateStatic(ReadTask,                                /* Function that implements the task. */
                          "USBProtoReadTask",                      /* Text name for the task. */
                          kReadTaskStackSize,                      /* Number of indexes in the xStack array. */
                          reinterpret_cast<void*>(1),              /* Parameter passed into the task. */
                          2, /* Priority at which the task is created. */
                          ReadTaskStack,                           /* Array to use as the task's stack. */
                          &staticReadTask);                        /* Variable to hold the task's data structure. */
  }
}
}  // namespace usb_service_protocol
