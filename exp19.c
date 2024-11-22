#include <stdio.h>
#include <string.h>
#include <openssl/des.h>

#define BLOCK_SIZE 8  // DES and 3DES operate on 8-byte blocks

// Function to encrypt a block using 3DES in CBC mode
void encrypt3DES_CBC(unsigned char *plaintext, unsigned char *ciphertext, int len, unsigned char *key, unsigned char *iv) {
    DES_key_schedule schedule1, schedule2, schedule3;
    DES_cblock key1, key2, key3, iv_block;
    
    // Split the 24-byte 3DES key into three 8-byte keys
    memcpy(key1, key, 8);
    memcpy(key2, key + 8, 8);
    memcpy(key3, key + 16, 8);
    
    // Set up the 3DES key schedules
    DES_set_key((DES_cblock*)key1, &schedule1);
    DES_set_key((DES_cblock*)key2, &schedule2);
    DES_set_key((DES_cblock*)key3, &schedule3);
    
    // Copy IV for CBC mode
    memcpy(iv_block, iv, BLOCK_SIZE);

    // Encrypt each block using 3DES in CBC mode
    for (int i = 0; i < len; i += BLOCK_SIZE) {
        unsigned char input_block[BLOCK_SIZE];
        unsigned char output_block[BLOCK_SIZE];
        
        // Copy plaintext block
        memcpy(input_block, plaintext + i, BLOCK_SIZE);

        // XOR input block with IV (CBC step)
        for (int j = 0; j < BLOCK_SIZE; j++) {
            input_block[j] ^= iv_block[j];
        }
        
        // Perform 3DES encryption (Encrypt -> Decrypt -> Encrypt)
        DES_ecb3_encrypt((DES_cblock*)input_block, (DES_cblock*)output_block, &schedule1, &schedule2, &schedule3, DES_ENCRYPT);
        
        // Copy ciphertext block
        memcpy(ciphertext + i, output_block, BLOCK_SIZE);

        // Update the IV (CBC mode: IV for next block is the current ciphertext)
        memcpy(iv_block, output_block, BLOCK_SIZE);
    }
}

// Function to print data in hexadecimal format
void printHex(unsigned char *data, int len) {
    for (int i = 0; i < len; i++) {
        printf("%02x", data[i]);
    }
    printf("\n");
}

int main() {
    // Example plaintext and key for 3DES CBC encryption
    unsigned char plaintext[] = "This is a test message for 3DES CBC encryption!";
    int len = strlen((char*)plaintext);

    unsigned char key[24] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
                              0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
                              0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef };  // 24-byte key for 3DES

    unsigned char iv[BLOCK_SIZE] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };  // 8-byte IV (initialization vector)

    unsigned char ciphertext[BLOCK_SIZE * ((len / BLOCK_SIZE) + 1)];

    // Perform 3DES CBC encryption
    encrypt3DES_CBC(plaintext, ciphertext, len, key, iv);

    // Print the ciphertext in hexadecimal format
    printf("Ciphertext (in hex): ");
    printHex(ciphertext, len);

    return 0;
}
