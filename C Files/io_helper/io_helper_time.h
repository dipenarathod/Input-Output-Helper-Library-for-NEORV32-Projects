/**
 * io_helper_time.h
 *
 * RISC-V mcycle / mcycleh CSR reading and cycle-to-microsecond printing.
 * Ada original: Input_Output_Helper.Time_Measurements (.ads / .adb)
 *
 * io_read_cycle() is inlined for minimal overhead at the measurement call
 * site, matching Ada's pragma Inline.
 *
 * Adjust IO_CLOCK_HZ to match your NEORV32 clock configuration.
 */

#ifndef IO_HELPER_TIME_H
#define IO_HELPER_TIME_H

#include <stdint.h>

/** System clock frequency in Hz.  Ada: Clock_Hz : constant Unsigned_64. */
#define IO_CLOCK_HZ  72000000ULL

/**
 * Read the full 64-bit mcycle counter via two CSR reads.
 * Ada: function Read_Cycle return Unsigned_64
 *
 * Uses RISC-V inline assembly (GCC extended asm), matching the Ada
 * System.Machine_Code Asm() calls in the original.
 *
 * NOTE: On RV32 there is a tiny race if mcycleh increments between the
 *       two reads.  For short measurement windows at 72 MHz this is
 *       negligible; add a re-read loop if you need guaranteed accuracy.
 */
static inline uint64_t io_read_cycle(void)
{
    uint32_t low, high;
    __asm__ volatile ("csrr %0, mcycle"  : "=r"(low)  : : );
    __asm__ volatile ("csrr %0, mcycleh" : "=r"(high) : : );
    return ((uint64_t)high << 32U) | (uint64_t)low;
}

/**
 * Print cycle count and derived microsecond time via UART0.
 * Ada: procedure Print_Time (Name : String; Cycles : Unsigned_64)
 *
 * neorv32_uart0_printf has no %llu support on all SDK versions, so the
 * 64-bit values are split into high/low 32-bit halves for printing.
 *
 * @param name    Label string (e.g. "matmul").
 * @param cycles  Elapsed cycle count (end_cycles - start_cycles).
 */
void io_print_time(const char *name, uint64_t cycles);

#endif /* IO_HELPER_TIME_H */
