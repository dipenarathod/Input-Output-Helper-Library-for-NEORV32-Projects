/**
 * io_helper.c
 *
 * Implementation of core helpers: tensor byte access, tensor sizing,
 * and Q0.7 fixed-point conversion routines.
 *
 * Ada original: input_output_helper.adb
 */

#include "io_helper.h"

/* -----------------------------------------------------------------------
 * Tensor helpers
 * ----------------------------------------------------------------------- */

unsigned_byte_t io_get_byte_from_tensor(const uint32_t *data, unsigned int index)
{
    unsigned int word_index = index / 4U;
    unsigned int byte_index = index % 4U;
    return io_unpack_byte_at_index(data[word_index], byte_index);
}

unsigned int io_tensor_words(unsigned int n, bool one_dimensional)
{
    if (one_dimensional) {
        return (n + 3U) / 4U;
    }
    return (n * n + 3U) / 4U;
    /*
     * The +3 ensures partially-filled words are counted.
     * e.g. 9×9 = 81 bytes → (81+3)/4 = 21 words (not 20).
     */
}

/* -----------------------------------------------------------------------
 * Q0.7 fixed-point conversions
 * ----------------------------------------------------------------------- */

float io_q07_to_float(unsigned_byte_t value)
{
    if (value < 128U) {
        return (float)value / 128.0f;
    }
    /* Negative value: reinterpret unsigned as two's-complement signed */
    return (float)((int)value - 256) / 128.0f;
}

unsigned_byte_t io_float_to_q07(float value)
{
    /* Scale then clamp to [-128, 127] */
    int scaled = (int)(value * 128.0f);
    if (scaled <= -128) { scaled = -128; }
    else if (scaled > 127) { scaled =  127; }

    if (scaled < 0) {
        /* Wrap negative signed int to unsigned uint8 (two's complement) */
        return (unsigned_byte_t)(uint8_t)(256 + scaled);
    }
    return (unsigned_byte_t)(uint8_t)scaled;
}

unsigned_byte_t io_int_to_q07(int value)
{
    if (value <= -128) { return (unsigned_byte_t)128U; }
    if (value >=  127) { return (unsigned_byte_t)127U; }
    if (value <    0 ) { return (unsigned_byte_t)(uint8_t)(256 + value); }
    return (unsigned_byte_t)(uint8_t)value;
}

int io_q07_to_int(unsigned_byte_t value)
{
    if (value < 128U) { return (int)value; }
    return (int)value - 256;
}

unsigned_byte_t io_uint8_to_q07(unsigned_byte_t value)
{
    /* Shift unsigned [0,255] to signed Q0.7 range [-128, 127] */
    int signed_val = (int)value - 128;
    if (signed_val < 0) {
        return (unsigned_byte_t)(uint8_t)(256 + signed_val);
    }
    return (unsigned_byte_t)(uint8_t)signed_val;
}
