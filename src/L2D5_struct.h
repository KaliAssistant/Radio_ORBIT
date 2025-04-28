/*
 * File:        src/L2D5_struct.h
 * Author:      KaliAssistant <work.kaliassistant.github@gmail.com>
 * URL:         https://github.com/KaliAssistant/Radio_ORBIT
 * Licence:     GNU/GPLv3.0
 *
 * Description: 
 *    L2.5 OEMR layer frame definition for Radio_ORBIT.
 *    A lightweight, flexible communication model designed for Radio Mesh AREA Network.
 *    This file is critical for protocol compatibility.
 *
 * WARNING:
 *   - DO NOT MODIFY THIS FILE MANUALLY.
 *   - Any changes must be approved and reviewed carefully.
 *   - Modifying the structure will break compatibility between devices.
 *   - Always ensure sizeof(L2D5Frame) == 216 bytes.
 *
 */

#ifndef L2D5_STRUCT_H
#define L2D5_STRUCT_H

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
 *  L2.5 Frame
 *
 * | Layer              | TAG | KeyHint | FLAG | Source Address | Destination Address | TTL | Payload |
 * ----------------------------------------------------------------------------------------------------
 * | Length(Bytes)      |  1  |     8   |   1  |      16        |         16          |  2  |   172   |
 * ----------------------------------------------------------------------------------------------------
 * | L2.5 Non-Encrypted | <-- |xxxxxxxxx|--                 216 Bytes                             --> |
 * ----------------------------------------------------------------------------------------------------
 * | L2.5 Encryped      | <-- 10+206 Bytes --> | #################### ENCRYPTED ##################### | 
 *
 *
 *
 *  +-------------------------- L2.5 Frame ------------------------------+
 *  +    00  01  02  03  04  05  06  07  08  09  0A  0B  0C  0D  0E  0F  +
 *  +--------------------------------------------------------------------+
 *  + 0|[*T][---------- Key Hint ----------][*F][-=-=-=-=- 16 Bytes src |+
 *  + 1| address =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-][-=-=-=-=- 16 Bytes dst |+
 *  + 2| address =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-][-=TTL-][-=172B payload |+
 *  + ~| ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ |+
 *  + D| -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=] XX  XX  XX  XX  XX  XX  XX  XX |+
 *  +--------------------------------------------------------------------+
 *
 *  *T: TAG
 *  *F: FLAG  
 */



typedef struct {
  /* 216 Bytes */
  uint8_t TAG;                        // L2.5: 0x00
  uint8_t KeyHint[8];                 // L2.5: 0x01-0x08
  uint8_t FLAG;                       // L2.5: 0x09
  uint8_t SrcAddress[16];             // L2.5: 0x0A-0x19
  uint8_t DstAddress[16];             // L2.5: 0x1A-0x29
  uint8_t TTL[2];                     // L2.5: 0x2A-0x2B
  uint8_t Payload[172];               // L2.5: 0x2C-0xD7
} L2D5Frame;


#ifdef __cplusplus
}
#endif

#endif // L2D5_STRUCT_H
