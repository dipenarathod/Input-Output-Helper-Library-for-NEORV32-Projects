/**
 * io_helper_debug.h
 *
 * Debug-print utilities for packed Q0.7 tensors and vectors.
 * Ada original: Input_Output_Helper.Debug (.ads / .adb)
 *
 * NOTE: Uses neorv32_uart0_printf() from the NEORV32 C SDK.
 *       neorv32_uart0_printf does NOT support %f or %llu; floats are
 *       rendered manually as "±int.frac" (3 decimal places).
 */

#ifndef IO_HELPER_DEBUG_H
#define IO_HELPER_DEBUG_H

#include <stdint.h>
#include "io_helper.h"

/**
 * Print a 2-D square tensor stored in packed Q0.7 format.
 * Ada: procedure Print_Tensor_Q07 (Name; Data : Word_Array; Dimension : Natural)
 *
 * @param name       Label printed before the tensor.
 * @param data       Packed Q0.7 word array (row-major, 4 bytes / word).
 * @param dimension  Side length N (prints an N×N matrix).
 */
void io_print_tensor_q07(const char   *name,
                          const uint32_t *data,
                          unsigned int   dimension);

/**
 * Print a 1-D vector stored in packed Q0.7 format.
 * Ada: procedure Print_Vector_Q07 (Name; Data : Word_Array; Vector_Length : Natural)
 *
 * @param name          Label printed before the vector.
 * @param data          Packed Q0.7 word array.
 * @param vector_length Number of elements.
 */
void io_print_vector_q07(const char   *name,
                          const uint32_t *data,
                          unsigned int   vector_length);

#endif /* IO_HELPER_DEBUG_H */
