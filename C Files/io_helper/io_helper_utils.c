/**
 * io_helper_utils.c
 *
 * Ada original: input_output_helper-utils.adb
 */

#include "io_helper_utils.h"

void io_create_word_array_from_int_array(const int    *src,
                                         unsigned int  src_len,
                                         uint32_t     *result)
{
    unsigned int leftover = src_len % 4U;
    unsigned int full_words = (src_len - leftover) / 4U;
    unsigned int i = 0U;

    /* Pack complete 4-byte words */
    for (unsigned int w = 0U; w < full_words; w++) {
        result[w] = io_pack_four_bytes(
            io_int_to_q07(src[i]),
            io_int_to_q07(src[i + 1U]),
            io_int_to_q07(src[i + 2U]),
            io_int_to_q07(src[i + 3U])
        );
        i += 4U;
    }

    /* Pack the partial trailing word (zero-pad missing bytes) */
    if (leftover != 0U) {
        unsigned_byte_t b0 = io_int_to_q07(src[i]);
        unsigned_byte_t b1 = (leftover >= 2U) ? io_int_to_q07(src[i + 1U]) : 0U;
        unsigned_byte_t b2 = (leftover >= 3U) ? io_int_to_q07(src[i + 2U]) : 0U;
        unsigned_byte_t b3 = 0U;
        result[full_words] = io_pack_four_bytes(b0, b1, b2, b3);
    }
}
