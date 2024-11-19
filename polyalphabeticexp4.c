#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26

// Function to encrypt the plaintext using the Vigenère cipher
void encryptVigenere(const char *plaintext, const char *key, char *ciphertext) {
    int keyLength = strlen(key);
    int j = 0; // Index for the key

    for (int i = 0; plaintext[i] != '\0'; i++) {
        char ch = plaintext[i];

        if (isalpha(ch)) {
            // Get the key character (adjust for case)
            char keyChar = toupper(key[j % keyLength]) - 'A';
            char base = isupper(ch) ? 'A' : 'a'; // Determine base for case
            // Encrypt the character
            ciphertext[i] = (toupper(ch) - 'A' + keyChar) % ALPHABET_SIZE + base;
            j++; // Move to the next character in the key
        } else {
            ciphertext[i] = ch; // Non-alphabetic characters remain unchanged
        }
    }
    ciphertext[strlen(plaintext)] = '\0'; // Null-terminate the ciphertext
}

int main() {
    char plaintext[100];
    char key[100];
    char ciphertext[100];

    printf("Enter the plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = '\0'; // Remove newline character

    printf("Enter the key: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0'; // Remove newline character

    // Encrypt the plaintext
    encryptVigenere(plaintext, key, ciphertext);

    // Output the encrypted message
    printf("Ciphertext: %s\n", ciphertext);

    return 0;
}