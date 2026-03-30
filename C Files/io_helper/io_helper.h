/**
 * io_helper.h
 *
 * Core register-access helpers and Q0.7 fixed-point conversion utilities.
 * C port of the Ada Input_Output_Helper package, targeting the NEORV32
 * RISC-V soft-core and its C software framework.
 *
 * Ada original: Input_Output_Helper (.ads / .adb)
 */

#ifndef IO_HELPER_H
#define IO_HELPER_H

#include <stdint.h>
#include <stdbool.h>

/* -----------------------------------------------------------------------
 * Type aliases  (mirror Ada subtypes)
 * ----------------------------------------------------------------------- */
typedef uint8_t  unsigned_byte_t;   /* Ada: Unsigned_Byte  (Interfaces.Unsigned_8)  */
typedef uint32_t word_t;            /* Ada: Word           (Interfaces.Unsigned_32) */

/* -----------------------------------------------------------------------
 * Register access
 * Ada: R32, Add_Byte_Offset, Write_Reg, Read_Reg
 * All inlined for zero-overhead peripheral access.
 * ----------------------------------------------------------------------- */

/**
 * Return a volatile 32-bit pointer to the given MMIO address.
 * Ada: function R32 (Addr : System.Address) return access Volatile_Word
 */
static inline volatile uint32_t *io_r32(uintptr_t addr)
{
    return (volatile uint32_t *)addr;
}

/**
 * Add a byte offset to a base address.
 * Ada: function Add_Byte_Offset (Address; Offset : Unsigned_32) return System.Address
 */
static inline uintptr_t io_add_byte_offset(uintptr_t address, uint32_t offset)
{
    return address + (uintptr_t)offset;
}

/**
 * Write a 32-bit value to a memory-mapped register.
 * Ada: procedure Write_Reg (Addr : System.Address; Value : Word)
 */
static inline void io_write_reg(uintptr_t addr, uint32_t value)
{
    *((volatile uint32_t *)addr) = value;
}

/**
 * Read a 32-bit value from a memory-mapped register.
 * Ada: function Read_Reg (Addr : System.Address) return Word
 */
static inline uint32_t io_read_reg(uintptr_t addr)
{
    return *((volatile uint32_t *)addr);
}

/* -----------------------------------------------------------------------
 * Byte / word packing helpers
 * ----------------------------------------------------------------------- */

/**
 * Extract one byte from a 32-bit word by byte index (0 = LSB).
 * Ada: function Unpack_Byte_At_Index (W : Word; Index : Natural) return Unsigned_Byte
 */
static inline unsigned_byte_t io_unpack_byte_at_index(uint32_t w, unsigned int index)
{
    return (unsigned_byte_t)((w >> (index * 8U)) & 0xFFU);
}

/**
 * Get a byte from a packed word array by flat byte index.
 * Ada: function Get_Byte_From_Tensor (Data : Word_Array; Index : Natural) return Unsigned_Byte
 *
 * @param data   Pointer to the word array.
 * @param index  Flat byte index across the array.
 */
unsigned_byte_t io_get_byte_from_tensor(const uint32_t *data, unsigned int index);

/**
 * Number of 32-bit words needed to hold N*N (or N) packed bytes.
 * Ada: function Tensor_Words (N : Natural; One_Dimensional : Boolean := False) return Natural
 *
 * @param n               Side length of tensor (or vector length when one_dimensional = true).
 * @param one_dimensional true  → (N + 3) / 4
 *                        false → (N*N + 3) / 4
 */
unsigned int io_tensor_words(unsigned int n, bool one_dimensional);

/* -----------------------------------------------------------------------
 * Q0.7 fixed-point conversions
 * Format: 1 sign bit + 7 fractional bits, represented as uint8.
 * Range:  [-1.0, 1.0)  → stored as 0-255.
 * ----------------------------------------------------------------------- */

/** Q0.7 uint8 → float.  Ada: Q07_To_Float */
float           io_q07_to_float(unsigned_byte_t value);

/** float → Q0.7 uint8, clamped.  Ada: Float_To_Q07 */
unsigned_byte_t io_float_to_q07(float value);

/** signed int → Q0.7 uint8, clamped to [-128, 127].  Ada: Int_To_Q07 */
unsigned_byte_t io_int_to_q07(int value);

/** Q0.7 uint8 → signed int.  Ada: Q07_To_Int */
int             io_q07_to_int(unsigned_byte_t value);

/**
 * Camera luminance byte (0-255, unsigned) → Q0.7 format.
 * Shifts range: 0=black → -128, 128=mid-gray → 0, 255=white → +127.
 * Ada: uint8_To_Q07
 */
unsigned_byte_t io_uint8_to_q07(unsigned_byte_t value);

#endif /* IO_HELPER_H */
