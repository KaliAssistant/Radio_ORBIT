/*
 * File:        test/l2d5_struct_test.c
 * Author:      KaliAssistant <work.kaliassistant.github@gmail.com>
 * URL:         https://github.com/KaliAssistant/Radio_ORBIT
 * Licence:     GNU/GPLv3.0
 *
 * Description:
 *    ORBIT L2.5 Frame Struct Test Program.
 *
 * NOTE:
 *   - This program only tested on Debian GNU/Linux.
 *
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../src/L2D5_struct.h"
#include <sys/random.h>
#include <zlib.h>
#include <sodium.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <time.h>
#include <printHexTable/printHexTable.h>
#define CRYPTO_X25519_PK_BYTES  32
#define CRYPTO_X25519_SK_BYTES  32
#define CRYPTO_X25519_SHR_BYTES 32



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

int aes256_cbc_encrypt(const uint8_t *key, const uint8_t *iv,
                       const uint8_t *in, uint8_t *out, size_t len) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int outlen1 = 0, outlen2 = 0;

    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);
    EVP_CIPHER_CTX_set_padding(ctx, 0);  // disable padding

    EVP_EncryptUpdate(ctx, out, &outlen1, in, len);
    EVP_EncryptFinal_ex(ctx, out + outlen1, &outlen2);

    EVP_CIPHER_CTX_free(ctx);
    return outlen1 + outlen2;
}

int aes256_cbc_decrypt(const uint8_t *key, const uint8_t *iv,
                       const uint8_t *in, uint8_t *out, size_t len) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    int outlen1 = 0, outlen2 = 0;

    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);
    EVP_CIPHER_CTX_set_padding(ctx, 0);  // disable padding

    EVP_DecryptUpdate(ctx, out, &outlen1, in, len);
    EVP_DecryptFinal_ex(ctx, out + outlen1, &outlen2);

    EVP_CIPHER_CTX_free(ctx);
    return outlen1 + outlen2;
}

// SHA-384(shared) → 32-byte AES key + 16-byte IV
void derive_key_iv(const uint8_t *shared, uint8_t *key_out, uint8_t *iv_out) {
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    uint8_t digest[48];

    EVP_DigestInit_ex(ctx, EVP_sha384(), NULL);
    EVP_DigestUpdate(ctx, shared, 32);
    EVP_DigestFinal_ex(ctx, digest, NULL);
    EVP_MD_CTX_free(ctx);

    memcpy(key_out, digest, 32);
    memcpy(iv_out,  digest + 32, 16);
}

void print_hex(const char *label, const uint8_t *buf, size_t len) {
    printf("%s: ", label);
    for (size_t i = 0; i < len; i++) printf("%02X", buf[i]);
    printf("\n");
}

int main() {

  // Initialize libsodium
  if(sodium_init() == -1) {
    return 1;
  }


  uint8_t nodeA_pk[CRYPTO_X25519_PK_BYTES];
  uint8_t nodeA_sk[CRYPTO_X25519_SK_BYTES];
  crypto_box_keypair(nodeA_pk, nodeA_sk);

  uint8_t nodeB_pk[CRYPTO_X25519_PK_BYTES];
  uint8_t nodeB_sk[CRYPTO_X25519_SK_BYTES];
  crypto_box_keypair(nodeB_pk, nodeB_sk);

  uint8_t shared_key[CRYPTO_X25519_SHR_BYTES];
  crypto_scalarmult(shared_key, nodeA_sk, nodeB_pk);
  
  uint8_t key[32], iv[16];

  derive_key_iv(shared_key, key, iv);




  L2D5Frame_t l2d5test;
  l2d5test.TAG = L2D5TAG_HELLO_PKT_NB;
  for(int i=0; i<4; i++) {
    l2d5test.KeyHint[i] = 0;
  }
  l2d5test.FLAG=L2D5FLAG_MKFLAG(7, L2D5FLAG_ERR_NML, L2D5FLAG_NUL_A);
  uint8_t* testSrcAddress = gen_rdm_bytestream(16);
  memcpy(l2d5test.SrcAddress, testSrcAddress, 16);
  for(int i=0; i<16; i++) {
    l2d5test.DstAddress[i] = 0xFF;
  }
  l2d5test.TTL[0] = 0;
  l2d5test.TTL[1] = 0;





  L2D5Routing_HelloPkt_t testHelloPkt;
  memcpy(testHelloPkt.NodeSrcAddr, testSrcAddress, 16);
  memcpy(testHelloPkt.PublicKey, shared_key, 32);
  time_t timestamp_now = time(NULL);
  uint32_t time_uint32 = (uint32_t) timestamp_now;
  uint8_t time_uint8_array[sizeof(time_uint32)];
  memcpy(time_uint8_array, &time_uint32, sizeof(time_uint32));
  memcpy(testHelloPkt.TimeStamp, time_uint8_array, 4);
  testHelloPkt.SEQ[0] = 0xA;
  testHelloPkt.SEQ[1] = 0x0;
  testHelloPkt.NodesLimit[0] = 0xFE;
  testHelloPkt.NodesLimit[1] = 0x00;
  for(int i=0; i<120; i++) {
    testHelloPkt.Padding[i] = 0;
  }






  memcpy(l2d5test.Payload, &testHelloPkt, 176);


  uint8_t l2d5Frame_buffer[216];
  memcpy(l2d5Frame_buffer, &l2d5test, 216);

  //printf("+----------------------- L2.5 Frame Non-Encrypted ------------------------ ASCII -------+\n");



  ANSI_Color_Map_t l2d5Frame_ansi_map[] = {
    {0x00, 0x00, ' ', ' ', "\e[1;32m"},
    {0x01, 0x04, '[', ']', "\e[0;33m"},
    {0x05, 0x05, ' ', ' ', "\e[1;93m"},
    {0x06, 0x15, '[', ']', "\e[0;36m"},
    {0x16, 0x25, '[', ']', "\e[0;94m"},
    {0x26, 0x27, '[', ']', "\e[1;35m"},
    {0x28, 0xD7, '[', ']', "\e[0;95m"}
  };

  ANSI_ErrTag_Map_t l2d5Frame_ansi_err_map[] = {
    {0x72, 0x72, 0x1},
    {0x81, 0x81, 0x2},
    {0x82, 0x82, 0x1},
    {0x90, 0x90, 0x3},
    {0x91, 0x91, 0x2},
    {0x92, 0x92, 0x1},
    {0x93, 0x93, 0x0},
    {0xA3, 0xA3, 0x3},
    {0xB3, 0xB3, 0x2},
    {0xC3, 0xC3, 0x1},
    {0xB4, 0xB4, 0x2},
    {0xC4, 0xC4, 0x1},
    {0xC5, 0xC5, 0x1}

  };
  #define ARRAY_LEN(x) (sizeof(x) / sizeof((x)[0]))

  printColorHexTable256(l2d5Frame_buffer, sizeof(l2d5Frame_buffer), l2d5Frame_ansi_map, ARRAY_LEN(l2d5Frame_ansi_map), l2d5Frame_ansi_err_map, ARRAY_LEN(l2d5Frame_ansi_err_map), "L2.5 Frame Non-Encrypted", "printHexTable version "PRINTHEXTABLE_VERSION"  by @KaliAssistant");  
  printf("\e[1;32m ██ TAG\e[0m \e[0;33m ██ KEYHINT\e[0m \e[1;93m ██ FLAG\e[0m \e[0;36m ██ SRCADDR\e[0m \e[0;94m ██ DSTADDR\e[0m \e[1;35m ██ TTL\e[0m \e[0;95m ██ PAYLOAD\e[0m\n");
  printf("\n");
  printf("%s+@error\e[0m %s+@warning\e[0m %s+@debug\e[0m\n", ANSI_LEVEL_COLOR[3], ANSI_LEVEL_COLOR[2], ANSI_LEVEL_COLOR[1]);
  printHexTable256(l2d5Frame_buffer, sizeof(l2d5Frame_buffer), "", "");
   //print_hex("AES Key", key, 32);
  //print_hex("AES IV",  iv, 16);
  return 0;
}
