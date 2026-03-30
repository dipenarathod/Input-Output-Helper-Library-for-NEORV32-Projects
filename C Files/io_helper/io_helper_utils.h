/**
 * io_helper_utils.h
 *
 * Byte packing / unpacking and integer-array-to-word-array conversion.
 * Ada original: Input_Output_Helper.Utils (.ads / .adb)
 *
 * Pack_Four_Bytes and Unpack_Four_Bytes are inlined here (header-only).
 * Create_Word_Array_From_Integer_Array is in io_helper_utils.c.
 */

#ifndef IO_HELPER_UTILS_H
#define IO_HELPER_UTILS_H

#include "io_helper.h"

/* -----------------------------------------------------------------------
 * Pack four bytes into one 32-bit word (little-endian: B0 = bits [7:0])
 * Ada: function Pack_Four_Bytes (B0, B1, B2, B3 : Unsigned_Byte) return Word
 * ----------------------------------------------------------------------- */
static inline uint32_t io_pack_four_bytes(unsigned_byte_t b0,
                                          unsigned_byte_t b1,
                                          unsigned_byte_t b2,
                                          unsigned_byte_t b3)
{
    return  (uint32_t)b0
          | ((uint32_t)b1 <<  8U)
          | ((uint32_t)b2 << 16U)
          | ((uint32_t)b3 << 24U);
}

/* -----------------------------------------------------------------------
 * Unpack a 32-bit word into four individual bytes.
 * Ada: procedure Unpack_Four_Bytes (W : Word; B0, B1, B2, B3 : out Unsigned_Byte)
 * In C, "out" parameters become pointers.
 * ----------------------------------------------------------------------- */
static inline void io_unpack_four_bytes(uint32_t w,
                                        unsigned_byte_t *b0,
                                        unsigned_byte_t *b1,
                                        unsigned_byte_t *b2,
                                        unsigned_byte_t *b3)
{
    *b0 = io_unpack_byte_at_index(w, 0U);
    *b1 = io_unpack_byte_at_index(w, 1U);
    *b2 = io_unpack_byte_at_index(w, 2U);
    *b3 = io_unpack_byte_at_index(w, 3U);
}

/* -----------------------------------------------------------------------
 * Convert an integer array to a packed Q0.7 word array.
 * Each integer is clamped/converted to Q0.7 uint8, then four bytes
 * are packed per 32-bit word.  A partial final word is zero-padded.
 *
 * Ada: procedure Create_Word_Array_From_Integer_Array
 *        (Integer_Source : in Integer_Array; Result_Word_Array : out Word_Array)
 *
 * @param src      Pointer to source integer array.
 * @param src_len  Number of elements in src.
 * @param result   Caller-allocated output array; must hold
 *                 io_tensor_words(src_len, true) words.
 * ----------------------------------------------------------------------- */
void io_create_word_array_from_int_array(const int    *src,
                                         unsigned int  src_len,
                                         uint32_t     *result);

#endif /* IO_HELPER_UTILS_H */
