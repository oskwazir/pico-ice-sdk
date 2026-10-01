# Why this repo is here

This is my fork of tinyVision's pico-ice-sdk (C, for the pico2-ice: RP2350B + iCE40UP5K).
I'm not building it or contributing to it. I'm reading it to learn how the RP2350 drives
the FPGA so I can write a Rust crate that does the same thing: `pico2-ice`, in
`~/Repos/pico2-ice-rs` (embassy-rp 0.10, `rp235xb`).

I'm new to FPGAs. Walk me through the code, answer questions, and explain the hardware
reasons behind what the C does. I write the Rust myself, so don't write crate code unless
I ask.

## Branches

- `main`: untouched upstream, commit `f3ddedc` (2026-03-06). Upstream is the `upstream`
  remote.
- `annotations`: my comments on the code. Comments go here, and never change the code's
  behaviour. Add comments only when I ask; I usually write my own.

This file is untracked and excluded in `.git/info/exclude`, so it stays out of both
branches.

## The plan these files feed

`~/Repos/hardware/pico2-ice-rust-crate.md` holds the spec and the staged bring-up.
`~/Repos/hardware/pico-ice-sdk-reading-guide.md` is the detailed walkthrough of this
repo: what to look for in each file, with line numbers at `f3ddedc`. Read the guide
before answering questions about a file; it may already cover the point. The reading
order:

1. `include/boards/pico2_ice.h`
2. `src/ice_fpga_data.c`
3. `include/ice_fpga.h`, `src/ice_fpga.c`
4. `examples/rp2_fpga/main.c`
5. `src/ice_cram.c`, `src/ice_cram.pio`
6. `examples/rp2_cram/main.c`
7. `src/pico-sdk_ice_hal.c`
8. `src/ice_flash.c`
9. `examples/ice_makefile_blinky/`

Skip `ice_usb.c`, `tinyuf2_*` and the `rp2_usb_*` examples. USB drag-and-drop/DFU loading
is out of scope for the crate.

## Things already found in this code

- `pico2_ice.h` says `ICE_GPOUT_CLOCK_PIN 22`, but that define is stale and unused. The
  real FPGA clock pin is GPIO 21, in `src/ice_fpga_data.c`.
- Loading CRAM means transmitting on GPIO 4, which is SPI0's RX pin. That's why
  `ice_hal_spi_init` falls back to PIO (`ice_cram.pio`).
- One CS pin (GPIO 5) serves both the flash and CRAM loading. Its level when CRESET is
  released decides which mode the FPGA boots in.
- `ice_cram_close` sends 56 clocks after CDONE (`:102-103`), meeting Lattice's minimum
  of 49 (FPGA-TN-02001). But it returns the CRESET level (`:109`), not CDONE, so it
  reports success even when the bitstream is rejected.
- `ice_fpga_start` doesn't wait for CDONE despite its doc comment.
  `ice_fpga_configured` does the wait, but no header declares it and no example calls it.
- The PIO CRAM path ignores the 1 MHz `freq` argument and runs at about 37.5 Mbit/s
  (`clk_div = 1`, 4 cycles per bit).
- `include/boards/ice40up5k.h` holds pico-ice (RP2040) values and is wrong for this board.
- `CS_psram = -1`: the PSRAM isn't on the FPGA's bus.
- `include/ice_spi.h` and `src/ice_spi.c` were deleted in `0bb23bd` (2025-05-24), the
  pico2-ice port merge, along with `ice_sram.*`. The ten `ice_spi_*` functions became the
  four in `include/ice_HAL.h:54-82`.
- `examples/rp2_fpga_io/` is pre-port dead code, added by that same merge off the old
  branch. It includes the deleted `ice_spi.h` (`:29`) without calling anything from it,
  uses the old one-arg `ice_fpga_init(12)` (`:39`, and `12` is the pre-`f3ddedc` MHz
  convention), and calls `ice_fpga_write`/`ice_fpga_read` (`:46-47`). The API it describes
  is gone.
- `ice_fpga_write` and `ice_fpga_read` have no definition anywhere in the repo. The only
  call sites are `examples/rp2_fpga_io/main.c:46-47` and `src/ice_usb.c:282,287`, so
  `ice_usb.c` doesn't compile at `f3ddedc` either. That also means `rp2_fpga_io` can't be
  rescued by fixing its `main.c`: `CMakeLists.txt:23` links `pico_ice_usb`, which contains
  `ice_usb.c`, so the link fails from inside the library.
- `include/ice_fpga.h` declares `ice_fpga_stop` twice, at `:77` and `:84`, with identical
  doc comments. The second slot is where a declaration for `ice_fpga_configured`
  (`src/ice_fpga.c:73`) would sit, between `ice_fpga_stop` and `ice_fpga_deinit`.
- `include/ice_HAL.h:64` uses `size_t` but includes only `<stdint.h>` and `<errno.h>`
  (`:25-26`). Every other consumer includes a pico-sdk header first and gets `size_t` by
  accident. `src/ice_led.c:26-28` includes only `#define` headers, so it's the one file
  where the order matters. It survives on the real toolchain: newlib's `<errno.h>` pulls
  `sys/errno.h` -> `sys/reent.h` -> `<stddef.h>`, so `size_t` is there. Verified against
  newlib 4.5.0 headers; `ice_led.c` compiles clean with them and fails with glibc. So the
  bug is latent, and only bites non-newlib libcs like the CLion indexing setup.
- Nothing builds all the examples at once. There's no `examples/CMakeLists.txt`, and each
  `rp2_*` example is a standalone CMake project with its own `pico-ice-sdk` and `pico-sdk`
  symlinks. That's how the stale examples went unnoticed.

## How to write to me

Plain, direct prose. Short answers. Cite `file:line` when pointing at code.

I'm new to C as well as to FPGAs. Don't assume toolchain mechanics are obvious to me:
preprocessor text pasting, include order, declaration vs definition, which build stage an
error comes from, CMake cache variables vs compiler `-D`. Name the mechanism and the stage
when it's load-bearing for the point, not as background on every answer.
