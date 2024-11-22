#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26

// English letter frequency (in order from most to least frequent)
const double english_freq[ALPHABET_SIZE] = { 0.12702, 0.09356, 0.08167, 0.06966, 0.12702, 
                                             0.04025, 0.06749, 0.07507, 0.01929, 0.00153, 
                                             0.00772, 0.04025, 0.02151, 0.06749, 0.07607, 
                                             0.01929, 0.00023, 0.00153, 0.06966, 0.08167, 
                                             0.01889, 0.00153, 0.07607, 0.01929, 0.00153, 
                                             0.04025};

// Function to count letter frequency in ciphertext
void countFrequency(const char *ciphertext, int *freq) {
    int i = 0;
    while (ciphertext[i] != '\0') {
        if (isalpha(ciphertext[i])) {
            freq[tolower(ciphertext[i]) - 'a']++;
        }
        i++;
    }
}

// Function to decrypt the ciphertext with a given shift
void decryptWithShift(const char *ciphertext, int shift, char *plaintext) {
    int i = 0;
    while (ciphertext[i] != '\0') {
        if (isalpha(ciphertext[i])) {
            char base = isupper(ciphertext[i]) ? 'A' : 'a';
            plaintext[i] = (ciphertext[i] - base - shift + ALPHABET_SIZE) % ALPHABET_SIZE + base;
        } else {
            plaintext[i] = ciphertext[i];
        }
        i++;
    }
    plaintext[i] = '\0';
}

// Function to calculate the "likeliness" of a given plaintext based on English letter frequencies
double calculateLikeliness(const char *plaintext) {
    int freq[ALPHABET_SIZE] = {0};
    countFrequency(plaintext, freq);

    double likeliness = 0.0;
    int total_letters = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        total_letters += freq[i];
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (total_letters > 0) {
            double observed = (double)freq[i] / total_letters;
            likeliness += observed * english_freq[i];
        }
    }

    return likeliness;
}

// Function to perform the attack and output top N possible plaintexts
void frequencyAttack(const char *ciphertext, int topN) {
    char plaintext[1000];
    double likeliness[ALPHABET_SIZE] = {0.0};
    
    // Try all shifts (0 to 25)
    for (int shift = 0; shift < ALPHABET_SIZE; shift++) {
        decryptWithShift(ciphertext, shift, plaintext);
        likeliness[shift] = calculateLikeliness(plaintext);
    }

    // Find the top N plaintexts with highest likelihood
    int indices[ALPHABET_SIZE];
    for (int i = 0; i < ALPHABET_SIZE; i++) indices[i] = i;

    // Sort the shifts by likelihood
    for (int i = 0; i < ALPHABET_SIZE - 1; i++) {
        for (int j = i + 1; j < ALPHABET_SIZE; j++) {
            if (likeliness[indices[i]] < likeliness[indices[j]]) {
                int temp = indices[i];
                indices[i] = indices[j];
                indices[j] = temp;
            }
        }
    }

    printf("Top %d possible plaintexts:\n", topN);
    for (int i = 0; i < topN && i < ALPHABET_SIZE; i++) {
        decryptWithShift(ciphertext, indices[i], plaintext);
        printf("Shift %d: %s\n", indices[i], plaintext);
    }
}

int main() {
    char ciphertext[] = "Vkrj chvkc chzb dzhq kbsbi.";

    int topN = 10; // Display top 10 possible plaintexts
    frequencyAttack(ciphertext, topN);

    return 0;
}
