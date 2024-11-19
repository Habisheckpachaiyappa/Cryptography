#include <stdio.h>

#define ALPHABET_SIZE 26

// Function to find the modular inverse of a under modulo m
int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1; // Inverse doesn't exist
}

int main() {
    // Frequencies of the letters
    char mostFrequentCiphertext = 'B'; // Corresponds to 1
    char secondMostFrequentCiphertext = 'U'; // Corresponds to 20

    // Corresponding plaintext letters
    char assumedMostFrequentPlaintext = 'E'; // Corresponds to 4
    char assumedSecondMostFrequentPlaintext = 'T'; // Corresponds to 19

    // Convert characters to their corresponding indices
    int C1 = mostFrequentCiphertext - 'A'; // C1 = 1
    int C2 = secondMostFrequentCiphertext - 'A'; // C2 = 20
    int P1 = assumedMostFrequentPlaintext - 'A'; // P1 = 4
    int P2 = assumedSecondMostFrequentPlaintext - 'A'; // P2 = 19

    // Solve for a and b
    for (int a = 1; a < ALPHABET_SIZE; a++) {
        if (modInverse(a, ALPHABET_SIZE) != -1) { // Check if a has an inverse
            // Calculate b using equation (1)
            int b = (C1 - (a * P1) % ALPHABET_SIZE + ALPHABET_SIZE) % ALPHABET_SIZE;

            // Verify with equation (2)
            if ((C2 - (a * P2) % ALPHABET_SIZE + ALPHABET_SIZE) % ALPHABET_SIZE == b) {
                printf("Possible keys found:\n");
                printf("a = %d, b = %d\n", a, b);
            }
        }
    }

    return 0;
}