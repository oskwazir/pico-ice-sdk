/*
 * MIT License
 * 
 * Copyright (c) 2023-2026 tinyVision.ai
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @defgroup ice_fpga
 * @brief FPGA I/O and control
 * @{
 *
 * Depending on the RTL loaded onto the FPGA, it can use its internal RC
 * oscillator or a more accurate clock signal provided by the RP2040. This
 * library allows to configure this signal, control the FPGA startup, and
 * communicate over SPI to the same pins used for SRAM or FLASH access.
 * An LED pin is used for communicating with the FPGA.
 */

#pragma once
#include <stdint.h>
#include <stdbool.h>

#include "ice_fpga_data.h"

#ifdef __cplusplus
extern "C" {
#endif

/** KHz to Hz */
#define AS_KHZ(_f) (_f * 1000)
/** MHz to Hz */
#define AS_MHZ(_f) AS_KHZ(_f * 1000)
/** Default 48MHz FPGA frequency */
#define ICE_FPGA_DEFAULT_FREQUENCY AS_MHZ(48)

/**
 * @brief Let the FPGA start and export a clock for it over `ICE_FPGA_CLOCK_PIN`.
 * @param freq_hz Exported clock frequency in Hz. Supported range is XOSC/65536 to SYS_PLL (125 or 150MHz)
 *
 * The RP2 exports its own crystal-based clock to the iCE40, through the
 * dedicated [`CLOCK GPOUT0`](https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf)
 * feature.
 * @return 0 on success, negative on fail
 */
int ice_fpga_init(const ice_fpga fpga, uint32_t freq_hz);

/**
 * @brief Release the stop mode if it was present, and wait that the FPGA
 *  confirms its startup with ICE_FPGA_CDONE_PIN.
 * @return 0 on success, negative on fail
 */
int ice_fpga_start(const ice_fpga fpga);

/**
 * @brief Set the ICE_FPGA_CRESET_B_PIN to LOW which keeps the FPGA in reset
 *  mode.
 * @return 0 on success, negative on fail
 */
int ice_fpga_stop(const ice_fpga fpga);

/**
 * @brief Set the ICE_FPGA_CRESET_B_PIN to LOW which keeps the FPGA in reset
 *  mode.
 * @return 0 on success, negative on fail
 */
int ice_fpga_stop(const ice_fpga fpga);

/**
 * @brief deinit and free FPGA and associated resources
 * @return 0 on success, negative on fail
 */
int ice_fpga_deinit(const ice_fpga fpga);

#ifdef __cplusplus
}
#endif

/** @} */
