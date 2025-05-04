#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <printHexTable/printHexTable.h>
#include <printHexTable/ANSI_types.h>
#include <colorUtils/conv.h>

int main(int argc, char** argv) {
  uint8_t data[512][256] = {0x40};
  char color_code[131072][30];
  ANSI_Color_Map_t rainbow[512][256];
  ANSI_Color_Map_t startz_begin[100][256];
  ANSI_ErrTag_Map_t* errmap;
  for(int i=0; i< 131072; i++) {
    float hue = ((float)i / 131072) * 360.0f;
    float rf, gf, bf;
    hsv2rgb(hue, 1.0f, 1.0f, &rf, &gf, &bf);
    uint8_t r, g, b;
    f01_2rgb888(rf, gf, bf, &r, &g, &b);
    uint8_t color_int = rgb2ansi256(r, g, b);
    sprintf(color_code[i], "\e[38;5;%d;48;5;%dm", color_int, color_int);
  }
  for(int i=0; i<512; i++) {
      for(int j=0; j< 256; j++) {
        rainbow[i][j].byteAddrBegin = j;
        rainbow[i][j].byteAddrEnd = j;
        rainbow[i][j].charBegin = ' ';
        rainbow[i][j].charEnd = ' ';
        rainbow[i][j].ansiColorStr = color_code[i*256+j];
      }
  }
  
  for(int i=0; i<256; i++) {
    startz_begin[0][i].byteAddrBegin = i;
    startz_begin[0][i].byteAddrEnd = i;
    startz_begin[0][i].charBegin = ' ';
    startz_begin[0][i].charEnd = ' ';
    startz_begin[0][i].ansiColorStr = "\e[0;107;97m";
  }
  for(int i=0; i<256; i++) {
    startz_begin[1][i].byteAddrBegin = i;
    startz_begin[1][i].byteAddrEnd = i;
    startz_begin[1][i].charBegin = ' ';
    startz_begin[1][i].charEnd = ' ';
    startz_begin[1][i].ansiColorStr = "\e[0;40;30m";
  }
  for(int i=0; i<256; i++) {
    startz_begin[2][i].byteAddrBegin = i;
    startz_begin[2][i].byteAddrEnd = i;
    startz_begin[2][i].charBegin = ' ';
    startz_begin[2][i].charEnd = ' ';
    startz_begin[2][i].ansiColorStr = "\e[0;41;31m";
  }
  for(int i=0; i<256; i++) {
    startz_begin[3][i].byteAddrBegin = i;
    startz_begin[3][i].byteAddrEnd = i;
    startz_begin[3][i].charBegin = ' ';
    startz_begin[3][i].charEnd = ' ';
    startz_begin[3][i].ansiColorStr = "\e[0;42;32m";
  }
  for(int i=0; i<256; i++) {
    startz_begin[4][i].byteAddrBegin = i;
    startz_begin[4][i].byteAddrEnd = i;
    startz_begin[4][i].charBegin = ' ';
    startz_begin[4][i].charEnd = ' ';
    startz_begin[4][i].ansiColorStr = "\e[0;43;33m";
  }
  for(int i=0; i<256; i++) {
    startz_begin[5][i].byteAddrBegin = i;
    startz_begin[5][i].byteAddrEnd = i;
    startz_begin[5][i].charBegin = ' ';
    startz_begin[5][i].charEnd = ' ';
    startz_begin[5][i].ansiColorStr = "\e[0;44;34m";
  }
  for(int i=0; i<256; i++) {
    startz_begin[6][i].byteAddrBegin = i;
    startz_begin[6][i].byteAddrEnd = i;
    startz_begin[6][i].charBegin = ' ';
    startz_begin[6][i].charEnd = ' ';
    startz_begin[6][i].ansiColorStr = "\e[0;45;35m";
  }
  for(int i=0; i<256; i++) {
    startz_begin[7][i].byteAddrBegin = i;
    startz_begin[7][i].byteAddrEnd = i;
    startz_begin[7][i].charBegin = ' ';
    startz_begin[7][i].charEnd = ' ';
    startz_begin[7][i].ansiColorStr = "\e[0;46;36m";
  }

  char ttt[][80] = {
    "$",
    "##########",
    "@@@@@@@@@@@",
    "@@@@@@@@@@@@@@@@@",
    "@@@@@@@@@@@@@@@@@@@@@",
    "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@",
    "@@@@@@@@@@@@@@@@@@@@@",
    "@@@@@@@@@@@@@@@@@",
    "@@@@@@@@@@@@@@@@@@@@@@@@@@",
    "#################################"
  };

  char jjj[][80] = {
    "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$",
    "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!",
    "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&"
  };

  char kkk[] = "printHexTable version "PRINTHEXTABLE_VERSION"  by @KaliAssistant";
  for(int i=0; i<10; i++) {
    printf("\ec");
    printColorHexTable256(data[0], sizeof(data[0]), startz_begin[i%2], 256, errmap, 0, ":-)", kkk);
    usleep(90000);
  }

  for(int o=0; o<5; o++) {
    for(int i=2; i<8; i++) {
      printf("\ec");
      printColorHexTable256(data[0], sizeof(data[0]), startz_begin[i], 256, errmap, 0, ":-)", kkk);
      usleep(20000);
    }
  }
  for(int i=2; i<8; i++) {
      printf("\ec");
      printColorHexTable256(data[0], sizeof(data[0]), startz_begin[i], 256, errmap, 0, ":-)", kkk);
      usleep(100000);
    }

  printf("\ec");
  printColorHexTable256(data[0], sizeof(data[0]), startz_begin[1], 256, errmap, 0, ":-)", kkk);
  usleep(1000000);

  while(1) {
    for(int i=0; i<512; i++) {
      printf("\ec");
      usleep(5000);
      printColorHexTable256(data[i], sizeof(data[i]), rainbow[i], 256, errmap, 0, ":-)", kkk);
    }


  }
  return 0;
}
