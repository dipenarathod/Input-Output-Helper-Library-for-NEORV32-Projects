/*
 * io_helper_debug.c — corrected to match io_helper_debug.h signatures
 *
 * Signatures (from header):
 *   void io_print_tensor_q07(const char *name, const uint32_t *data, unsigned int size)
 *   void io_print_vector_q07(const char *name, const uint32_t *data, unsigned int size)
 *
 *   'size' = total number of Q0.7 bytes (elements) in the packed array.
 *
 * io_get_byte_from_tensor(data, index) uses a flat byte index:
 *   byte 'index' lives in data[index/4] at byte lane (index % 4).
 */

#include "io_helper_debug.h"
#include "io_helper.h"
#include "neorv32.h"

/* ------------------------------------------------------------------ */
/*  Internal helpers                                                    */
/* ------------------------------------------------------------------ */

/* Print exactly 3 decimal digits, zero-padded. Range: 0..999. */
static void print_frac3(int v)
{
    if (v < 10)
        neorv32_uart0_printf("00%d", v);
    else if (v < 100)
        neorv32_uart0_printf("0%d", v);
    else
        neorv32_uart0_printf("%d", v);
}

/*
 * Print one Q0.7 byte as " 0.XXX" or "-0.XXX".
 * Integer arithmetic only — no %f, no %03d.
 * frac = round(|q| * 1000 / 128)
 */
static void print_q07_value(int8_t q)
{
    /* Special case: int8_t minimum = -128 = exactly -1.000 in Q0.7 */
    if (q == -128)
    {
        neorv32_uart0_printf("-1.000");
        return;
    }

    int abs_q = (q < 0) ? -(int)q : (int)q;
    int frac = (abs_q * 1000 + 64) / 128;
    /* No clamp needed now — abs_q max is 127, frac max is 992 */

    if (q < 0)
        neorv32_uart0_printf("-0.");
    else
        neorv32_uart0_printf(" 0.");
    print_frac3(frac);
}

/* ------------------------------------------------------------------ */
/*  Public API — signatures match io_helper_debug.h exactly            */
/* ------------------------------------------------------------------ */

/*
 * Print a packed Q0.7 word-array as a 2D block.
 * 'size' = number of Q0.7 bytes.
 * Groups of 4 bytes per row to show the word boundaries.
 */
void io_print_tensor_q07(const char *name,
                         const uint32_t *data,
                         unsigned int dimension)
{
    unsigned int total = dimension * dimension;
    neorv32_uart0_printf("%s [\n", name);
    for (unsigned int i = 0U; i < total; i++)
    {
        int8_t val = (int8_t)io_get_byte_from_tensor(data, i);
        if (i % dimension == 0U)
            neorv32_uart0_printf("  [");
        print_q07_value(val);
        if (i % dimension == dimension - 1U)
            neorv32_uart0_printf("],\n");
        else
            neorv32_uart0_printf(", ");
    }
    neorv32_uart0_printf("]\n");
}

/*
 * Print a packed Q0.7 word-array as a flat vector.
 * 'size' = number of Q0.7 bytes.
 */
void io_print_vector_q07(const char *name,
                         const uint32_t *data,
                         unsigned int size)
{
    neorv32_uart0_printf("%s [ ", name);
    for (unsigned int i = 0; i < size; i++)
    {
        int8_t val = (int8_t)io_get_byte_from_tensor(data, i);
        print_q07_value(val);
        if (i < size - 1)
            neorv32_uart0_printf(", ");
    }
    neorv32_uart0_printf(" ]\n");
}
