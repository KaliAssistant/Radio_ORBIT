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
 *   - Always ensure sizeof(L2D5Frame_Encrypted) == 216 bytes.
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

/*
 *  L2.5 TAG definition
 *  
 *  |======================================== TAG ========================================|
 *  ---------------------------------------------------------------------------------------
 *  | NAME | ENCRYPTED | OEMR | FORWARD | TCP | BOARDCAST | ANYCAST | MULTICAST | UNICAST |
 *  ---------------------------------------------------------------------------------------
 *  | bit  |     0     |   1  |     2   |  3  |     4     |    5    |     6     |     7   |
 *
 *  TYPE definition
 *
 *  |     TYPE/bit     | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
 *  ----------------------------------------------------
 *  |   HELLO_PKT_NB   | 0 | 1 | 0 | 0 | 1 | 0 | 0 | 0 |
 *  ----------------------------------------------------
 *  |   HELLO_PKT_RM   | 1 | 1 | 1 | 0 | 0 | 0 | 1 | 1 |  // TODO : MULTICAST OR UNICAST? (UNICAST, but MULTICAST...)
 *  ----------------------------------------------------
 *  |   TCP_DATA_DC    | 1 | 0 | 0 | 1 | 0 | 0 | 0 | 1 |
 *  ----------------------------------------------------
 *  |   TCP_DATA_RM    | 1 | 0 | 1 | 1 | 0 | 0 | 0 | 1 |
 */


/*
 *  L2.5 FLAG definition
 *
 *  |====================== FLAG ======================
 *  ---------------------------------------------------
 *  | NAME |       PRI       |  ERR FLAG  | NULL FLAG |
 *  ---------------------------------------------------
 *  | bit  | 0-3 (0-15, L->H)|  4-5 *U1*  | 6-7 (N/A) |
 *
 *  *U1*: ERR FLAG definition
 *  
 *  | TYPE/bit | 4 | 5 |
 *  --------------------
 *  | ERR_NML  | 0 | 0 |
 *  --------------------
 *  | ERR_DBG  | 0 | 1 |
 *  --------------------
 *  | ERR_ERR  | 1 | 0 |
 *  --------------------
 *  | ERR_WARN | 1 | 1 |
 */

#define MAGIC_L2D5LAYER_TAGTYPE_HELLO_PKT_NB 0b01001000
#define MAGIC_L2D5LAYER_TAGTYPE_HELLO_PKT_RM 0b11100011
#define MAGIC_L2D5LAYER_TAGTYPE_TCP_DATA_DC 0b10010001
#define MAGIC_L2D5LAYER_TAGTYPE_TCP_DATA_RM 0b10110001

typedef enum {
  L2D5TAG_HELLO_PKT_NB = 0b01001000,
  L2D5TAG_HELLO_PKT_RM = 0b11100011,
  L2D5TAG_TCP_DATA_DC = 0b10010001,
  L2D5TAG_TCP_DATA_RM = 0b10110001
} L2D5TAG_t;

typedef enum {
  L2D5FLAG_ERR_NML  = 0b00,
  L2D5FLAG_ERR_DBG  = 0b01,
  L2D5FLAG_ERR_ERR  = 0b10,
  L2D5FLAG_ERR_WARN = 0b11
} L2D5FLAG_ERR_t;

typedef enum {
  L2D5FLAG_NUL_A = 0b00,
  L2D5FLAG_NUL_B = 0b01,
  L2D5FLAG_NUL_C = 0b10,
  L2D5FLAG_NUL_D = 0b11
} L2D5FLAG_NUL_t;


#define L2D5FLAG_PRI_SHIFT 0
#define L2D5FLAG_ERR_SHIFT 4
#define L2D5FLAG_NUL_SHIFT 6

#define L2D5FLAG_PRI_MASK  (0x0F << L2D5FLAG_PRI_SHIFT) // bits 0-3
#define L2D5FLAG_ERR_MASK  (0x03 << L2D5FLAG_ERR_SHIFT) // bits 4-5
#define L2D5FLAG_NUL_MASK  (0x03 << L2D5FLAG_NUL_SHIFT) // bits 6-7
                                                        
                                                        
static inline uint8_t L2D5FLAG_MKFLAG(uint8_t _pri, L2D5FLAG_ERR_t _err_flag, L2D5FLAG_NUL_t _nul_flag) {
  return ((_pri & 0x0F) << L2D5FLAG_PRI_SHIFT) |
         ((_err_flag & 0x03) << L2D5FLAG_ERR_SHIFT) |
         ((_nul_flag & 0x03) << L2D5FLAG_NUL_SHIFT);
}

static inline uint8_t L2D5FLAG_GET_PRI(uint8_t _aByte) {
  return (_aByte & L2D5FLAG_PRI_MASK) >> L2D5FLAG_PRI_SHIFT;
}

static inline L2D5FLAG_ERR_t L2D5FLAG_GET_ERR(uint8_t _aByte) {
  return (L2D5FLAG_ERR_t)((_aByte & L2D5FLAG_ERR_MASK) >> L2D5FLAG_ERR_SHIFT);
}

static inline L2D5FLAG_NUL_t L2D5FLAG_GET_NUL(uint8_t _aByte) {
  return (L2D5FLAG_NUL_t)((_aByte & L2D5FLAG_NUL_MASK) >> L2D5FLAG_NUL_SHIFT);
}


/* L2.5 Frame structure definition (must be 216 bytes) */
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


/* L2.5 Frame Encrypted structure definition (must be 216 bytes) */
typedef struct {
  /* 216 Bytes */
  uint8_t TAG;                        // L2.5: 0x00
  uint8_t KeyHint[8];                 // L2.5: 0x01-0x08
  uint8_t FLAG;                       // L2.5: 0x09
  uint8_t EncryptedPayload[206];      // L2.5: 0x0A-0xD7
} L2D5Frame_Encrypted;


STATIC_ASSERT(sizeof(L2D5Frame) == 216, L2D5Frame_must_be_216bytes);
STATIC_ASSERT(sizeof(L2D5Frame_Encrypted) == 216, L2D5Frame_Encrypted_must_be_216bytes);

#ifdef __cplusplus
}
#endif

#endif // L2D5_STRUCT_H
