#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MOD 26

// Function to multiply matrices mod 26
void multiplyMatrix(int matrix[2][2], int vector[2]) {
    int result[2];
    for (int i = 0; i < 2; i++) {
        result[i] = 0;
        for (int j = 0; j < 2; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
        result[i] %= MOD;
    }
    // Store the result back in the vector
    for (int i = 0; i < 2; i++) {
        vector[i] = result[i];
    }
}

// Function to find the modular inverse of a number modulo MOD
int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1; // No inverse exists
}

// Function to calculate the inverse of a 2x2 matrix mod 26
void inverseMatrix(int matrix[2][2], int invMatrix[2][2]) {
    int det = (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]) % MOD;
    if (det < 0) det += MOD;

    // Inverse of determinant mod 26
    int invDet = modInverse(det, MOD);
    
    // Inverse matrix calculation using adjugate method
    invMatrix[0][0] = (matrix[1][1] * invDet) % MOD;
    invMatrix[0][1] = (-matrix[0][1] * invDet) % MOD;
    invMatrix[1][0] = (-matrix[1][0] * invDet) % MOD;
    invMatrix[1][1] = (matrix[0][0] * invDet) % MOD;

    // Ensure positive values
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            if (invMatrix[i][j] < 0) invMatrix[i][j] += MOD;
        }
    }
}

// Function to convert plaintext to numbers
void textToNumbers(char* text, int* nums, int length) {
    for (int i = 0; i < length; i++) {
        if (text[i] == ' ') {
            nums[i] = 23; // Treat space as 'X' (23rd letter)
        } else {
            nums[i] = toupper(text[i]) - 'A'; // Convert character to number (0-25)
        }
    }
}

// Function to convert numbers back to characters
void numbersToText(int* nums, char* text, int length) {
    for (int i = 0; i < length; i++) {
        text[i] = nums[i] + 'A';
    }
    text[length] = '\0';
}

// Hill cipher encryption function
void hillCipherEncrypt(char* plaintext, int key[2][2], char* ciphertext) {
    int len = strlen(plaintext);
    int nums[len];
    textToNumbers(plaintext, nums, len);
    
    // Encrypt pairs of numbers
    for (int i = 0; i < len; i += 2) {
        int pair[2] = { nums[i], nums[i+1] };
        multiplyMatrix(key, pair);
        nums[i] = pair[0];
        nums[i+1] = pair[1];
    }
    numbersToText(nums, ciphertext, len);
}

// Hill cipher decryption function
void hillCipherDecrypt(char* ciphertext, int key[2][2], char* decryptedText) {
    int len = strlen(ciphertext);
    int nums[len];
    int invKey[2][2];
    inverseMatrix(key, invKey);
    
    textToNumbers(ciphertext, nums, len);

    // Decrypt pairs of numbers
    for (int i = 0; i < len; i += 2) {
        int pair[2] = { nums[i], nums[i+1] };
        multiplyMatrix(invKey, pair);
        nums[i] = pair[0];
        nums[i+1] = pair[1];
    }
    numbersToText(nums, decryptedText, len);
}

int main() {
    char plaintext[] = "meet me at the usual place at ten rather than eight oclock";
    char ciphertext[100], decryptedText[100];

    // Key matrix for Hill cipher
    int key[2][2] = { {9, 4}, {5, 7} };

    // Encrypt the message
    hillCipherEncrypt(plaintext, key, ciphertext);
    printf("Encrypted ciphertext: %s\n", ciphertext);

    // Decrypt the ciphertext
    hillCipherDecrypt(ciphertext, key, decryptedText);
    printf("Decrypted plaintext: %s\n", decryptedText);

    return 0;
}

