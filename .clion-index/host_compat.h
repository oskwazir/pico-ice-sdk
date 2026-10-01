/* Force-included by compile_commands.json only. Never part of a real build.
 *
 * pico-sdk targets arm-none-eabi-gcc + newlib, whose <sys/cdefs.h> supplies
 * __unused, __printflike and friends. pico-sdk only defines them itself in its
 * IAR branch (pico/platform/compiler.h:31-47, under `#elif defined __ICCARM__`).
 * Indexing with host glibc gcc, nothing defines them, so headers like
 * hardware/irq.h:197 and pico/stdio.h:222 fail to parse and every symbol in
 * that translation unit stops resolving.
 *
 * Also pulls <stddef.h> for size_t: include/ice_HAL.h:64 uses it but includes
 * only <stdint.h> and <errno.h>, and gets away with it everywhere except
 * src/ice_led.c, whose other includes (boards.h, ice_led.h) are pure #defines.
 */
#include <stddef.h>
#include <sys/cdefs.h>

#ifndef __unused
#define __unused        __attribute__((__unused__))
#endif
#ifndef __used
#define __used          __attribute__((__used__))
#endif
#ifndef __printflike
#define __printflike(a, b)
#endif
#ifndef __aligned
#define __aligned(x)    __attribute__((__aligned__(x)))
#endif
#ifndef __always_inline
#define __always_inline __attribute__((__always_inline__))
#endif
#ifndef __noinline
#define __noinline      __attribute__((__noinline__))
#endif
#ifndef __packed
#define __packed        __attribute__((__packed__))
#endif

/* glibc's <sys/cdefs.h> defines __CONCAT as a single-level paste:
 *     #define __CONCAT(x,y) x ## y
 * Operands of ## are not macro-expanded, so the nested use at
 * hardware/timer.h:182
 *     __CONCAT(__CONCAT(timer,PICO_DEFAULT_TIMER), _hw)
 * pastes ')' onto '_hw' and fails. newlib, pico-sdk's own IAR fallback
 * (pico/platform/compiler.h:52-56) and llvm_libc/sys/cdefs.h:12-13 all use the
 * two-level form, which expands the inner call first. Match them.
 */
#undef __CONCAT
#undef __CONCAT1
#define __CONCAT1(x, y) x ## y
#define __CONCAT(x, y)  __CONCAT1(x, y)
