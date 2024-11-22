#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define BLOCK_SIZE 8
#define KEY_SIZE 8

// Permutation and shift tables
int PC1[56] = { 57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2, 59, 51, 43, 35, 27, 19, 11, 3,
                60, 52, 44, 36, 63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37,
                29, 21, 13, 5, 28, 20, 12, 4 };

int PC2[48] = { 14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2,
                41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32 };

int SHIFTS[16] = { 1, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1, 2, 2, 1 };

int E[48] = { 32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17, 16, 17, 18, 19, 20,
              21, 20, 21, 22, 23, 24, 25, 24, 25, 26, 27, 28, 27, 28, 29, 30, 31, 32 };

int P[32] = { 16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10, 2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6,
              22, 11, 4, 25 };

int IP_INV[64] = { 40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31, 38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13,
                  53, 21, 61, 29, 36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27, 34, 2, 42, 10, 50, 18, 58, 26, 33,
                  1, 41, 9, 49, 17, 57, 25 };

uint64_t IP(uint64_t input) {
    uint64_t output = 0;
    for (int i = 0; i < 64; i++) {
        if ((input >> (63 - IP_INV[i])) & 1)
            output |= (1ULL << (63 - i));
    }
    return output;
}

uint64_t permute(uint64_t input, const int *perm, int size) {
    uint64_t output = 0;
    for (int i = 0; i < size; i++) {
        if ((input >> (63 - perm[i])) & 1)
            output |= (1ULL << (63 - i));
    }
    return output;
}

void split(uint64_t input, uint64_t *left, uint64_t *right) {
    *left = input >> 32;
    *right = input & 0xFFFFFFFF;
}

uint64_t F(uint64_t right, uint64_t key) {
    uint64_t expanded = permute(right, E, 48);
    uint64_t xored = expanded ^ key;
    uint64_t result = 0;
    for (int i = 0; i < 32; i++) {
        result |= ((xored >> (31 - P[i])) & 1) << (31 - i);
    }
    return result;
}

void shiftLeft(uint64_t *part, int shifts) {
    *part = ((*part) << shifts) | (*part >> (28 - shifts));
    *part &= 0xFFFFFFF;
}

void generateKeys(uint64_t key, uint64_t *keys) {
    uint64_t permutedKey = permute(key, PC1, 56);
    uint64_t left, right;
    split(permutedKey, &left, &right);
    for (int round = 0; round < 16; round++) {
        shiftLeft(&left, SHIFTS[round]);
        shiftLeft(&right, SHIFTS[round]);
        uint64_t combined = (left << 28) | right;
        keys[15 - round] = permute(combined, PC2, 48);
    }
}

uint64_t decrypt(uint64_t ciphertext, uint64_t *keys) {
    uint64_t left, right;
    uint64_t permutedText = permute(ciphertext, IP_INV, 64);
    split(permutedText, &left, &right);
    for (int round = 0; round < 16; round++) {
        uint64_t temp = left;
        left = right;
        right = temp ^ F(left, keys[round]);
    }
    uint64_t combined = (left << 32) | right;
    return permute(combined, IP_INV, 64);
}

int main() {
    uint64_t key = 0x133457799BBCDFF1;  // Example 64-bit key (must be 64-bits)
    uint64_t ciphertext = 0x3A7B2F7A5C6F8A6A;  // Example 64-bit ciphertext (must be 64-bits)
    uint64_t keys[16];

    // Generate round keys for decryption
    generateKeys(key, keys);

    // Decrypt ciphertext
    uint64_t plaintext = decrypt(ciphertext, keys);

    printf("Decrypted Text: %llx\n", plaintext);
    return 0;
}
