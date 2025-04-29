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


  printf("+--------------------------- L2 Frame -------------------------------+\n");
  printf("+    00  01  02  03  04  05  06  07  08  09  0A  0B  0C  0D  0E  0F  +\n");
  printf("+--------------------------------------------------------------------+\n");
  int buffer_count=0;

  for(uint8_t h=0x0; h<0xE; h++) {
    printf("+ %X|", h);
    for(int i=0; i<16;i++) {
      char dec_str_b = ' ';
      char dec_str_e = ' ';
      
      char* ansi_clr ="";

      if(buffer_count+i == 1) dec_str_b='[';
      if(buffer_count+i == 2) dec_str_e=']';
      if(buffer_count+i == 3) dec_str_b='[';
      if(buffer_count+i == 218) dec_str_e=']';
      if(buffer_count+i == 219) dec_str_b='[';
      if(buffer_count+i == 222) dec_str_e=']';

      if(buffer_count+i == 0) ansi_clr="\e[1;32m";
      if(buffer_count+i >=1 && buffer_count+i <=2) ansi_clr="\e[0;33m";
      if(buffer_count+i >=3 && buffer_count+i <=218) ansi_clr="\e[0;36m";
      if(buffer_count+i >=219 && buffer_count+i <=222) ansi_clr="\e[0;35m";
      if(buffer_count+i == 223) ansi_clr="\e[1;31m";

      printf("%s%c%02X%c%s", ansi_clr, dec_str_b, print_l2_buffer[buffer_count+i], dec_str_e, "\e[0m");
    }
    buffer_count+=16;
    printf("|+\n");
  }
  printf("+--------------------------------------------------------------------+\n");

  printf("\e[1;32m ██ SFD\e[0m\t\e[0;33m ██ TAG\e[0m\t\e[0;36m ██ PAYLOAD\e[0m\t\e[0;35m ██ CHECKSUM\e[0m\t\e[1;31m ██ EFD\e[0m\n");

  free(payload_test);
  
  printf("\nORBIT L2 Frame Struct Test Program. Using Random Payload And CRC32\n");
  printf("Author: KaliAssistant <work.kaliassistant.github@gmail.com>\n");
  printf("URL:    https://github.com/KaliAssistant/Radio_ORBIT\n");

  return 0;
}
