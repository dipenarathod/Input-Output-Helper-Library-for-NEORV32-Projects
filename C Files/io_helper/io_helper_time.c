/**
 * io_helper_time.c
 *
 * Ada original: input_output_helper-time_measurements.adb
 */

#include "io_helper_time.h"
#include "neorv32.h"

void io_print_time(const char *name, uint64_t cycles)
{
    uint64_t us = (cycles * 1000000ULL) / IO_CLOCK_HZ;

    /*
     * neorv32_uart0_printf supports %u (uint32_t) but not %llu.
     * Split each 64-bit value into its high and low 32-bit halves.
     * For typical measurement windows at 72 MHz the high word will
     * almost always be 0, giving clean single-number output.
     */
    uint32_t cycles_hi = (uint32_t)(cycles >> 32U);
    uint32_t cycles_lo = (uint32_t)(cycles & 0xFFFFFFFFULL);
    uint32_t us_hi     = (uint32_t)(us     >> 32U);
    uint32_t us_lo     = (uint32_t)(us     & 0xFFFFFFFFULL);

    if (cycles_hi == 0U) {
        neorv32_uart0_printf("%s cycles = %u\n", name, cycles_lo);
    } else {
        neorv32_uart0_printf("%s cycles = %u_%u\n", name, cycles_hi, cycles_lo);
    }

    if (us_hi == 0U) {
        neorv32_uart0_printf("%s time (us) = %u\n", name, us_lo);
    } else {
        neorv32_uart0_printf("%s time (us) = %u_%u\n", name, us_hi, us_lo);
    }
}
