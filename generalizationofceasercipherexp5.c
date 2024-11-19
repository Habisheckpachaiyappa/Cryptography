#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Function to check if a is coprime with 26
int isCoprime(int a, int n) {
    while (n != 0) {
        int temp = n;
        n = a % n;
        a = temp;
    }
    return a == 1; // If gcd is 1, they are coprime
}

// Function to encrypt plaintext using the affine cipher
void affineCipherEncrypt(const char *plaintext, int a, int b, char *ciphertext) {
    for (int i = 0; plaintext[i] != '\0'; i++) {
        char ch = plaintext[i];

        if (isalpha(ch)) {
            char base = isupper(ch) ? 'A' : 'a';
            // Encrypt the character
            ciphertext[i] = (a * (ch - base) + b) % ALPHABET_SIZE + base;
        } else {
            ciphertext[i] = ch; // Non-alphabetic characters remain unchanged
        }
    }
    ciphertext[strlen(plaintext)] = '\0'; // Null-terminate the ciphertext
}

int main() {
    char plaintext[100];
    char ciphertext[100];
    int a, b;

    printf("Enter the plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = '\0'; // Remove newline character

    printf("Enter the value of a (must be coprime with 26): ");
    scanf("%d", &a);
    printf("Enter the value of b (0 <= b < 26): ");
    scanf("%d", &b);

    // Check if a is coprime with 26
    if (!isCoprime(a, ALPHABET_SIZE)) {
        printf("Error: The value of a (%d) is not allowed. It must be coprime with 26.\n", a);
        return 1;
    }

    // Check if b is within the valid range
    if (b < 0 || b >= ALPHABET_SIZE) {
        printf("Error: The value of b (%d) must be in the range [0, 25].\n", b);
        return 1;
    }

    // Encrypt the plaintext
    affineCipherEncrypt(plaintext, a, b, ciphertext);

    // Output the encrypted message
    printf("Ciphertext: %s\n", ciphertext);

    return 0;
}