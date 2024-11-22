#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define ALPHABET_SIZE 26
#define MAX_CIPHERTEXT_LENGTH 1000

// English letter frequency (from most to least frequent)
const double english_freq[ALPHABET_SIZE] = {0.12702, 0.09356, 0.08167, 0.06966, 0.12702,
                                            0.04025, 0.06749, 0.07507, 0.01929, 0.00153,
                                            0.00772, 0.04025, 0.02151, 0.06749, 0.07607,
                                            0.01929, 0.00023, 0.00153, 0.06966, 0.08167,
                                            0.01889, 0.00153, 0.07607, 0.01929, 0.00153,
                                            0.04025};

// Function to count the frequency of letters in the ciphertext
void countFrequency(const char *ciphertext, int *freq) {
    int i = 0;
    while (ciphertext[i] != '\0') {
        if (isalpha(ciphertext[i])) {
            freq[tolower(ciphertext[i]) - 'a']++;
        }
        i++;
    }
}

// Function to calculate the "likeliness" of a given decryption based on English letter frequencies
double calculateLikeliness(const int *freq) {
    double likeliness = 0.0;
    int total_letters = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        total_letters += freq[i];
    }

    // Calculate the likelihood based on the frequency distribution of letters in the ciphertext
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (total_letters > 0) {
            double observed = (double)freq[i] / total_letters;
            likeliness += observed * english_freq[i];
        }
    }

    return likeliness;
}

// Function to decrypt the ciphertext with a given substitution key
void decrypt(const char *ciphertext, char *plaintext, char *key) {
    int i = 0;
    while (ciphertext[i] != '\0') {
        if (isalpha(ciphertext[i])) {
            char base = isupper(ciphertext[i]) ? 'A' : 'a';
            plaintext[i] = (tolower(ciphertext[i]) - base + (key[tolower(ciphertext[i]) - 'a'] - 'a')) % ALPHABET_SIZE + base;
        } else {
            plaintext[i] = ciphertext[i]; // Non-alphabetic characters remain unchanged
        }
        i++;
    }
    plaintext[i] = '\0'; // Null-terminate the plaintext
}

// Function to generate and sort possible plaintexts based on frequency analysis
void frequencyAttack(const char *ciphertext, int topN) {
    int freq[ALPHABET_SIZE] = {0};
    countFrequency(ciphertext, freq);

    // Create a mapping of letters from most frequent in ciphertext to most frequent in English
    char sorted_ciphertext[ALPHABET_SIZE], sorted_plaintext[ALPHABET_SIZE];
    int sorted_indices[ALPHABET_SIZE];
    
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        sorted_indices[i] = i;
    }
    
    // Sort ciphertext frequency from highest to lowest
    for (int i = 0; i < ALPHABET_SIZE - 1; i++) {
        for (int j = i + 1; j < ALPHABET_SIZE; j++) {
            if (freq[sorted_indices[i]] < freq[sorted_indices[j]]) {
                int temp = sorted_indices[i];
                sorted_indices[i] = sorted_indices[j];
                sorted_indices[j] = temp;
            }
        }
    }

    // Create the sorted cipher and plaintext arrays based on sorted frequency
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        sorted_ciphertext[i] = 'a' + sorted_indices[i];
        sorted_plaintext[i] = 'a' + i;
    }

    // Try top N shifts and print out corresponding plaintexts
    char key[ALPHABET_SIZE];
    char decrypted_text[MAX_CIPHERTEXT_LENGTH];

    printf("Top %d Possible Plaintexts:\n", topN);
    for (int i = 0; i < topN; i++) {
        // Generate a key for this shift
        for (int j = 0; j < ALPHABET_SIZE; j++) {
            key[sorted_ciphertext[j] - 'a'] = sorted_plaintext[(j + i) % ALPHABET_SIZE];
        }
        
        // Decrypt the ciphertext using the key
        decrypt(ciphertext, decrypted_text, key);
        printf("Possible plaintext #%d: %s\n", i + 1, decrypted_text);
    }
}

int main() {
    char ciphertext[MAX_CIPHERTEXT_LENGTH];
    int topN;

    // Input ciphertext and number of top plaintexts to show
    printf("Enter the ciphertext: ");
    fgets(ciphertext, sizeof(ciphertext), stdin);
    ciphertext[strcspn(ciphertext, "\n")] = 0;  // Remove newline character

    printf("Enter the number of top possible plaintexts to display: ");
    scanf("%d", &topN);

    // Perform the frequency attack
    frequencyAttack(ciphertext, topN);

    return 0;
}
