#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26

void initializeCipher(char *cipher, const char *key) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        cipher[i] = key[i];
    }
}

void encryptMessage(const char *message, const char *cipher, char *encrypted) {
    for (int i = 0; message[i] != '\0'; i++) {
        char ch = message[i];

        if (isalpha(ch)) {
            if (isupper(ch)) {
                encrypted[i] = toupper(cipher[ch - 'A']);
            } else {
                encrypted[i] = tolower(cipher[ch - 'a']);
            }
        } else {
            encrypted[i] = ch; // Non-alphabetic characters remain unchanged
        }
    }
}

int main() {
    char key[ALPHABET_SIZE + 1];
    char cipher[ALPHABET_SIZE];
    char message[100];
    char encrypted[100];

    // Define a substitution key (example: a simple shift)
    printf("Enter a substitution key (26 unique letters): ");
    fgets(key, sizeof(key), stdin);
    
    // Remove newline character from fgets
    key[strcspn(key, "\n")] = '\0';

    // Validate the key (length and uniqueness)
    if (strlen(key) != ALPHABET_SIZE) {
        printf("Key must be exactly 26 characters long.\n");
        return 1;
    }

    int letterCount[ALPHABET_SIZE] = {0}; // To track letter occurrences
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        char ch = tolower(key[i]);
        if (ch < 'a' || ch > 'z' || letterCount[ch - 'a'] > 0) {
            printf("Key must contain unique letters A-Z.\n");
            return 1;
        }
        letterCount[ch - 'a']++;
    }

    // Initialize the cipher mapping
    initializeCipher(cipher, key);

    // Get the plaintext message
    printf("Enter the message to encrypt: ");
    fgets(message, sizeof(message), stdin);
    
    // Remove newline character from fgets
    message[strcspn(message, "\n")] = '\0';

    // Encrypt the message
    encryptMessage(message, cipher, encrypted);
    encrypted[strlen(message)] = '\0'; // Null-terminate the encrypted string

    // Output the encrypted message
    printf("Encrypted message: %s\n", encrypted);

    return 0;
}