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
#ifndef USB_SERVICE_PROTOCOL_HPP
#define USB_SERVICE_PROTOCOL_HPP
#include <FreeRTOS.h>
#include <task.h>
#include "hal_usb_serial.h"
namespace usb_service_protocol {

/**
 * @brief The max amount of arguments a command can take...
 * @note The default value is 10. */
inline constexpr uint8_t kUSBProtoMaxAmountOfArguments = 10;

/**
 * @brief The register structs containing commands and the related information
 * @note  - cmd contains the command in string format:
 * e.g. Command help would be "HELP".
 * - num_of_args contains the amount of arguments the command takes
 * - stream_cmd indicates if the command gives streaming output, 
 * meaning it will be run continuously until killed by an enter key press.
 * - cmd_cb will have the pointer to the CB to run when CMD requested from Console
 * The returned string of this cb will be printed to user in console.
 */
typedef struct {
  const char cmd_[kUSBProtoMaxAmountOfArguments];
  uint8_t num_of_args_;
  bool stream_cmd_;
  const char* (*cmd_cb)(char** args, int num_of_args);
} USBServiceProtocolRegisters;

/**
 * @brief Init the USB service protocol, this will set the internal registers according to the parameters
 * 
 * @param registers Pointer to registerdata array containing the register info and cb function
 * @param num_of_registers The total number of registers stored in the registerdata array
 */
void Init(const usb_serial_inst_t usb_inst, USBServiceProtocolRegisters* registers, const size_t num_of_registers);

/**
 * @brief Set background polling task for the USB service module.
 *        The task set will be monitoring the console character by character.
 * 
 * @note  Warning! Module needs to be initialized first!
 * 
 * @param task_handle Pointer to external task_handle to attach background task to
 */
void SetPollingTask(TaskHandle_t* task_handle);

}  // namespace usb_service_protocol

#endif