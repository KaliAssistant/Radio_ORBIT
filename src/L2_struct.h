/*
 * File:        src/L2_struct.h
 * Author:      KaliAssistant <work.kaliassistant.github@gmail.com>
 * URL:         https://github.com/KaliAssistant/Radio_ORBIT
 * Licence:     GNU/GPLv3.0
 *
 * Description: 
 *    L2 DataLink layer frame definition for Radio_ORBIT.
 *    A lightweight, flexible communication model designed for Radio Mesh AREA Network.
 *    This file is critical for protocol compatibility.
 *
 * WARNING:
 *   - DO NOT MODIFY THIS FILE MANUALLY.
 *   - Any changes must be approved and reviewed carefully.
 *   - Modifying the structure will break compatibility between devices.
 *   - Always ensure sizeof(L2Frame) == 224 bytes.
 *
 */

#ifndef L2_STRUCT_H
#define L2_STRUCT_H

#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif


/* Static assert that works in both C and C++ */
#ifdef __cplusplus
  #define STATIC_ASSERT(cond, msg) static_assert(cond, #msg)
#else
  #if __STDC_VERSION__ >= 201112L
    #define STATIC_ASSERT(cond, msg) _Static_assert(cond, #msg)
  #else
    #define STATIC_ASSERT(cond, msg) typedef char static_assertion_##msg[(cond) ? 1 : -1]
  #endif
#endif


/*
 *  L2 Frame
 *
 * | Layer         | SFD | Type(or length) | Payload | FCS(32bit CRC) | EFD |
 * --------------------------------------------------------------------------
 * | Length(Bytes) |  1  |       2         |   216   |       4        |  1  |
 * --------------------------------------------------------------------------
 * | Layer 2 Frame | <--               224 Bytes                       -->  |
 *
 *
 *  
 *  +--------------------------- L2 Frame -------------------------------+
 *  +    00  01  02  03  04  05  06  07  08  09  0A  0B  0C  0D  0E  0F  +
 *  +--------------------------------------------------------------------+
 *  + 0| 5A [*TAG1*][----------------------216 Bytes Payload ---------- |+ 
 *  + ~| ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ |+
 *  + D| ------------------------------------------][- 4Bytes CRC -] FF |+
 *  +--------------------------------------------------------------------+
 *
 *  *TAG1*: Type or length 
 */

#define MAGIC_L2LAYER_SFD 0x5A     // SFD For DataLink Layer 0x5A
#define MAGIC_L2LAYER_EFD 0xFF     // EFD For DataLink Layer 0xFF


/* L2 Frame structure definition (must be 224 bytes) */
typedef struct {
  /* 224 Bytes */
  uint8_t SFD;                      // L2: 0x00
  uint8_t TAG[2];                   // L2: 0x01-0x02
  uint8_t Payload[216];             // L2: 0x03-0xDA
  uint8_t ChkSum[4];     // CRC32   // L2: 0xDB-0xDE
  uint8_t EFD;                      // L2: 0xDF
} L2Frame;


STATIC_ASSERT(sizeof(L2Frame) == 224, L2Frame_must_be_224_bytes);

#ifdef __cplusplus
}
#endif

#endif // L2_STRUCT_H
