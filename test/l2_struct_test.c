/*
 * File:        test/l2_struct_test.c
 * Author:      KaliAssistant <work.kaliassistant.github@gmail.com>
 * URL:         https://github.com/KaliAssistant/Radio_ORBIT
 * Licence:     GNU/GPLv3.0
 *
 * Description:
 *    ORBIT L2 Frame Struct Test Program. Using Random Payload And CRC32
 *
 * NOTE:
 *   - This program only tested on Debian GNU/Linux.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "../src/L2_struct.h"
#include <sys/random.h>
#include <zlib.h>
#include <printHexTable/printHexTable.h>



uint8_t* gen_rdm_bytestream(size_t const num_bytes) {
    uint8_t * const stream = malloc(num_bytes);
    if (stream == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    ssize_t ret = getrandom(stream, num_bytes, 0);
    if (ret != (ssize_t)num_bytes) {
        perror("getrandom failed");
        free(stream);
        exit(EXIT_FAILURE);
    }
    return stream;
}



void displayStruct() {
  printf("\e[0;90m");
  printf("L2 Frame:\n\n");
  printf("| Layer         | SFD | Type(or length) | Payload | FCS(32bit CRC) | EFD |\n");
  printf("--------------------------------------------------------------------------\n");
  printf("| Length(Bytes) |  1  |       2         |   216   |       4        |  1  |\n");
  printf("--------------------------------------------------------------------------\n");
  printf("| Layer 2 Frame | <--               224 Bytes                       -->  |\e[0m\n\n");
}


int main() {
  L2Frame l2test;
  l2test.SFD = 0x5A;
  l2test.EFD = 0xFF;
  l2test.TAG[0] = 0x0A;
  l2test.TAG[1] = 0x04;
  uint8_t* payload_test = gen_rdm_bytestream(216);
  memcpy(l2test.Payload, payload_test, 216);
  //l2test.Payload = payload_test;
  uint32_t checksum = crc32(0, l2test.Payload, 216);

  l2test.ChkSum[0] = (uint8_t)((checksum >> 24) & 0xFF);
  l2test.ChkSum[1] = (uint8_t)((checksum >> 16) & 0xFF);
  l2test.ChkSum[2] = (uint8_t)((checksum >> 8) & 0xFF);
  l2test.ChkSum[3] = (uint8_t)(checksum & 0xFF);

  uint8_t print_l2_buffer[224];
  print_l2_buffer[0] = l2test.SFD;
  print_l2_buffer[1] = l2test.TAG[0];
  print_l2_buffer[2] = l2test.TAG[1];
  for(int i=0; i<216; i++) {
    print_l2_buffer[3+i] = l2test.Payload[i];
  }

  for(int i=0; i<4; i++) {
    print_l2_buffer[219+i] = l2test.ChkSum[i];
  }
  print_l2_buffer[223] = l2test.EFD;


  displayStruct();
  
  ANSIColorMap256_t l2Frame_ansi_map = {0};
  addr2AnsiColorMap256(&l2Frame_ansi_map, 0x00, 0x00, "\e[1;32m", 0x00, ' ', 0x00, ' ', 1);
  addr2AnsiColorMap256(&l2Frame_ansi_map, 0x01, 0x02, "\e[0;33m", 0x01, '[', 0x02, ']', 1);
  addr2AnsiColorMap256(&l2Frame_ansi_map, 0x03, 0xDA, "\e[0;36m", 0x03, '[', 0xDA, ']', 1);
  addr2AnsiColorMap256(&l2Frame_ansi_map, 0xDB, 0xDE, "\e[0;35m", 0xDB, '[', 0xDE, ']', 1);
  addr2AnsiColorMap256(&l2Frame_ansi_map, 0xDF, 0xDF, "\e[1;31m", 0xDF, ' ', 0xDF, ' ', 1);



  char *output_str = printColorHexTable256(print_l2_buffer, sizeof(print_l2_buffer), &l2Frame_ansi_map, NULL, "L2 Frame", "printHexTable version "PRINTHEXTABLE_VERSION"  by @KaliAssistant");


  printf("%s", output_str);
  printf("\e[1;32m ██ SFD\e[0m\t\e[0;33m ██ TAG\e[0m\t\e[0;36m ██ PAYLOAD\e[0m\t\e[0;35m ██ CHECKSUM\e[0m\t\e[1;31m ██ EFD\e[0m\n");

  free(payload_test);
  free(output_str);
  printf("\nORBIT L2 Frame Struct Test Program. Using Random Payload And CRC32\n");
  printf("Author: KaliAssistant <work.kaliassistant.github@gmail.com>\n");
  printf("URL:    https://github.com/KaliAssistant/Radio_ORBIT\n");

  return 0;
}
