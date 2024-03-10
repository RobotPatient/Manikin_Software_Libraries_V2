/* *******************************************************************************************
 * Copyright (c) 2024 by RobotPatient Simulators
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

#ifndef USB_SERVICE_PROTOCOL_HPP
#define USB_SERVICE_PROTOCOL_HPP

#include <FreeRTOS.h>
#include <task.h>
#include "hal_usb_serial.h"
#include "tusb.h"

namespace usb_service_protocol {

inline constexpr uint8_t kUSBProtoMaxAmountOfArguments = 10;
inline constexpr char kTerminalEntryCharacter = '>';
inline constexpr char kNewLineCharacter = '\n';
inline constexpr uint8_t kCarriageReturnCharacter = '\r';
inline constexpr uint8_t kStrTerminationCharacter = '\0';
inline constexpr uint8_t kSpaceCharacter = ' ';
inline constexpr uint8_t kBackspaceAsciiCode = 8;
inline constexpr uint8_t kBackspaceEscapeChar = 37;
inline constexpr int kNoNewCharacter = -1;
inline constexpr char kPrintOnNextLine[] = "\r\n";

/**
 * @brief The register structs containing commands and the related information
 * @note  - cmd contains the command in string format: e.g. Command help would be "HELP".
 *        - num_of_args contains the amount of arguments the command takes
 *        - stream_cmd indicates if the command gives streaming output, 
 * meaning it will be run continuously until killed by an enter key press.
 *         - cmd_cb will have the pointer to the CB to run when CMD requested from Console
 * The returned string of this cb will be printed to user in console.
 */
typedef struct {
  const char cmd_[kUSBProtoMaxAmountOfArguments];
  uint8_t num_of_args_;
  uint8_t stream_cmd_;
  const char* (*cmd_cb)(char** args, int num_of_args);
} USBServiceProtocolRegisters;

template <usb_serial_inst_t serial_inst, size_t read_buffer_size, uint8_t NumRegisters>
class USBServiceProtocol {
 public:
  constexpr void init(const USBServiceProtocolRegisters* registers) {
    serviceProtocolRegisters = registers;
    // Buffers are never terminated when uninitialized.
    // If not memset our parsing functions will not work!
    memset(read_buffer_, kStrTerminationCharacter, read_buffer_size);
    tud_cdc_n_write_char(serial_inst, kTerminalEntryCharacter);
  }

  void polling_task() {
    int character = kNoNewCharacter;
    if (usb_serial_available(serial_inst)) {
      tud_cdc_n_write_flush(serial_inst);
      character = usb_serial_read_char(serial_inst);
      uint8_t last_command_was_stream_cmd = LastRegister != NULL ? LastRegister->stream_cmd_ : 0;
      uint8_t new_character = character != kNoNewCharacter;
      if (new_character) {
        ParseInput(read_buffer_, character, last_command_was_stream_cmd);
      } else if (last_command_was_stream_cmd) {
        const char* stream_cmd_output = LastRegister->cmd_cb(NULL, 0);
        usb_serial_write_string(serial_inst, stream_cmd_output);
        usb_serial_write_string(serial_inst, kPrintOnNextLine);
      }
    }
  }

 private:
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

  char read_buffer_[read_buffer_size];
  /**
 * @brief Internal register definitions (exact copy of the parameters given in Init function)
 * 
 * @note serviceProtocolRegister contains a pointer to the array containing the registers used by the usbregister protocol
 *       NumRegisters contains the number of registers in the pointer array.
 */
  const USBServiceProtocolRegisters* serviceProtocolRegisters = NULL;

  /**
 * @brief This variable contains the Last executed command/register
 * It is needed to remember if previous register was stream cmd
 */
  const USBServiceProtocolRegisters* LastRegister = NULL;

  /**
 * @brief Read index for the read buffer
 * This variable is used to keep track of the amount of elements in the read buffer
 */
  uint8_t read_index = 0;

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
    // We add one to the input to return a pointer after the NULL termination char
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
    const uint8_t more_than_one_argument_entered = Args.argsBuffer[0] != NULL;
    if (more_than_one_argument_entered) {
      // There are more arguments.. Loop over the readbuffer searching for more arguments (whitespaces)
      for (uint8_t argNum = 1; argNum < kUSBProtoMaxAmountOfArguments; argNum++) {
        Args.argsBuffer[argNum] = Getarg(Args.argsBuffer[argNum - 1]);
        if (Args.argsBuffer[argNum] == NULL) {
          break;
        }
        else {
          Args.argNum++;
        }
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
      const uint8_t command_found = (strcmp(serviceProtocolRegisters[command_index].cmd_, buffer) == 0);
      if (command_found) {
        const uint8_t too_many_arguments = (args.argNum > serviceProtocolRegisters[command_index].num_of_args_);
        const uint8_t too_few_arguments = (args.argNum < serviceProtocolRegisters[command_index].num_of_args_);
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
 * @param last_command_was_stream_cmd uint8_tean indicating whether the previous ran command is a streaming command
 */
  inline void ParseInput(char* read_buffer, int character, const uint8_t last_command_was_stream_cmd) {
    switch (character) {
      case kBackspaceAsciiCode: {
        // Backspace means removing previous character in buffer
        // And moving the cursor back 1 position
        read_buffer[read_index] = kStrTerminationCharacter;
        const uint8_t buffer_not_empty = (read_index != 0);
        if (buffer_not_empty) {
          tud_cdc_n_write_char(serial_inst, character);
          tud_cdc_n_write_flush(serial_inst);
          read_index--;
        }
        break;
      }
      case kCarriageReturnCharacter: {
        read_buffer[read_index++] = character;
        usb_serial_write_string(serial_inst, kPrintOnNextLine);
        // We of course don't want to run the entered empty command when a stream command is interrupted by enter key
        if (last_command_was_stream_cmd) {
          LastRegister = NULL;
        } else {
          const char* command_output = Runcmd(read_buffer);
          usb_serial_write_string(serial_inst, command_output);
          size_t command_output_size = strlen(command_output);
          if(command_output_size > CFG_TUD_CDC_TX_BUFSIZE) {
          for(size_t i = 1; i < (command_output_size/CFG_TUD_CDC_TX_BUFSIZE)+1; i++) {
                 usb_serial_write_string(serial_inst, (command_output+(i*CFG_TUD_CDC_TX_BUFSIZE)));
             }
           }
          usb_serial_write_string(serial_inst, kPrintOnNextLine);
        }
        tud_cdc_n_write_char(serial_inst, kTerminalEntryCharacter);
        tud_cdc_n_write_flush(serial_inst);
        memset(read_buffer, kStrTerminationCharacter, read_index);
        read_index = 0;
        break;
      }
      default: {
        if (read_index < read_buffer_size) {
          tud_cdc_n_write_char(serial_inst, character);
          tud_cdc_n_write_flush(serial_inst);
          read_buffer[read_index++] = character;
        }
        break;
      }
    }
  }
};
}  // namespace usb_service_protocol

#endif