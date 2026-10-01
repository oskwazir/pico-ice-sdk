/* Indexer stub for the header pioasm generates from src/ice_cram.pio
 * (CMakeLists.txt:31-33). Declares only what src/pico-sdk_ice_hal.c uses.
 *
 * THE INSTRUCTION OPCODES BELOW ARE PLACEHOLDER ZEROS, not the real assembly.
 * Read src/ice_cram.pio for the actual program. Nothing builds from this file.
 */
#ifndef _ICE_CRAM_PIO_H
#define _ICE_CRAM_PIO_H

#include "hardware/pio.h"

#define ice_cram_wrap_target 0
#define ice_cram_wrap 2
#define ice_cram_pio_version 0

static const uint16_t ice_cram_program_instructions[] = {
    0x0000, /* 0: nop            side 1     */
    0x0000, /* 1: out    x, 1               */
    0x0000, /* 2: mov    pins, x side 0 [1] */
};

static const struct pio_program ice_cram_program = {
    .instructions = ice_cram_program_instructions,
    .length = 3,
    .origin = -1,
    .pio_version = ice_cram_pio_version,
};

static inline pio_sm_config ice_cram_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + ice_cram_wrap_target, offset + ice_cram_wrap);
    sm_config_set_sideset(&c, 2, true, false);
    return c;
}

#endif
