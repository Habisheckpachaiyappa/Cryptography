#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define KEY_SIZE 8  // 64 bits
#define HALF_KEY_SIZE 28  // 28 bits per half of the key
#define SUBKEY_SIZE 48  // 48 bits per subkey

// Initial Permutation (PC-1) table for 56-bit key to 56-bit permuted key
int PC1[56] = { 57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36, 63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4 };

// Final Permutation (PC-2) table to get 48-bit subkeys
int PC2[48] = { 14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32 };

// Number of left shifts for each round (same as usual DES key schedule)
int SHIFTS[16] = { 1, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1, 2, 2, 1 };

// Function to permute the key using a given permutation table
uint64_t permute(uint64_t input, const int *perm, int size) {
    uint64_t output = 0;
    for (int i = 0; i < size; i++) {
        if ((input >> (63 - perm[i])) & 1)
            output |= (1ULL << (63 - i));
    }
    return output;
}

// Function to split the key into two 28-bit halves
void split(uint64_t key, uint64_t *left, uint64_t *right) {
    *left = key >> HALF_KEY_SIZE;  // Left 28 bits
    *right = key & 0xFFFFFFF;      // Right 28 bits (masked to 28 bits)
}

// Function to shift the left and right halves of the key
void shiftLeft(uint64_t *left, uint64_t *right, int shifts) {
    *left = ((*left) << shifts) | (*left >> (HALF_KEY_SIZE - shifts));
    *right = ((*right) << shifts) | (*right >> (HALF_KEY_SIZE - shifts));
    *left &= 0xFFFFFFF;  // Keep only the lower 28 bits
    *right &= 0xFFFFFFF;  // Keep only the lower 28 bits
}

// Function to generate the 16 subkeys for DES encryption
void generateSubkeys(uint64_t key, uint64_t *subkeys) {
    uint64_t permutedKey = permute(key, PC1, 56);  // Apply PC1 to the key
    uint64_t left, right;
    split(permutedKey, &left, &right);  // Split into two 28-bit halves

    for (int round = 0; round < 16; round++) {
        // Apply the shifts to the two halves of the key
        shiftLeft(&left, &right, SHIFTS[round]);

        // Combine left and right halves and apply PC2 to generate subkey
        uint64_t combined = (left << HALF_KEY_SIZE) | right;
        subkeys[round] = permute(combined, PC2, SUBKEY_SIZE);  // Apply PC2
    }
}

// Function to print the subkeys in hexadecimal
void printSubkeys(uint64_t *subkeys) {
    for (int i = 0; i < 16; i++) {
        printf("Subkey %d: %016llx\n", i + 1, subkeys[i]);
    }
}

int main() {
    // Example 64-bit key (56 bits + 8 parity bits)
    uint64_t key = 0x133457799BBCDFF1;  // 64-bit key

    uint64_t subkeys[16];

    // Generate 16 subkeys for the DES encryption
    generateSubkeys(key, subkeys);

    // Print the generated subkeys
    printSubkeys(subkeys);

    return 0;
}
