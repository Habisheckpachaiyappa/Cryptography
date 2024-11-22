#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MOD 26

// Function to multiply matrices mod 26
void multiplyMatrix(int matrix[2][2], int vector[2], int result[2]) {
    for (int i = 0; i < 2; i++) {
        result[i] = 0;
        for (int j = 0; j < 2; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
        result[i] %= MOD;
    }
}

// Function to calculate the modular inverse of a number modulo MOD
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
        nums[i] = toupper(text[i]) - 'A'; // Convert character to number (0-25)
    }
}

// Function to convert numbers back to characters
void numbersToText(int* nums, char* text, int length) {
    for (int i = 0; i < length; i++) {
        text[i] = nums[i] + 'A';
    }
    text[length] = '\0';
}

// Function to perform a known or chosen plaintext attack on Hill cipher
void hillCipherAttack(int pt1[2], int pt2[2], int ct1[2], int ct2[2], int key[2][2]) {
    // Convert plaintext and ciphertext pairs into numbers and form matrices
    int matrixP[2][2] = {{pt1[0], pt1[1]}, {pt2[0], pt2[1]}};
    int matrixC[2][2] = {{ct1[0], ct1[1]}, {ct2[0], ct2[1]}};
    
    int invMatrixP[2][2];
    inverseMatrix(matrixP, invMatrixP); // Find the inverse of the plaintext matrix
    
    // Multiply the inverse of the plaintext matrix with the ciphertext matrix
    int result[2][2] = {{0, 0}, {0, 0}};
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < 2; k++) {
                result[i][j] += invMatrixP[i][k] * matrixC[k][j];
            }
            result[i][j] %= MOD;
        }
    }
    
    // The resulting matrix is the key matrix
    key[0][0] = result[0][0];
    key[0][1] = result[0][1];
    key[1][0] = result[1][0];
    key[1][1] = result[1][1];
}

int main() {
    // Chosen plaintext pairs (each pair consists of two letters converted to numbers)
    int pt1[2] = {12, 4};  // "ME" -> [12, 4]
    int pt2[2] = {19, 7};  // "AT" -> [19, 7]
    
    // Corresponding ciphertext pairs
    int ct1[2] = {7, 14};  // "HI" -> [7, 14]
    int ct2[2] = {0, 5};   // "AF" -> [0, 5]
    
    // The key matrix to be recovered
    int key[2][2];
    
    // Perform the attack to recover the key matrix
    hillCipherAttack(pt1, pt2, ct1, ct2, key);
    
    // Print the recovered key matrix
    printf("Recovered Key Matrix:\n");
    printf("%d %d\n", key[0][0], key[0][1]);
    printf("%d %d\n", key[1][0], key[1][1]);
    
    return 0;
}
