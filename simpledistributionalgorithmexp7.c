#include <stdio.h>
#include <string.h>

#define MAX_CHARS 256

void analyzeFrequency(const char *ciphertext, int *frequency) {
    for (int i = 0; ciphertext[i] != '\0'; i++) {
        frequency[(unsigned char)ciphertext[i]]++;
    }
}

void printFrequency(int *frequency) {
    printf("Character Frequencies:\n");
    for (int i = 0; i < MAX_CHARS; i++) {
        if (frequency[i] > 0) {
            printf("'%c': %d\n", i, frequency[i]);
        }
    }
}

int main() {
    const char *ciphertext = "53‡‡†305))6*;4826)4‡.)4‡);806*;48†8¶60))85;;]8*;:‡*8†83"
                             "(88)5*†;46(;88*96*?;8)*‡(;485);5*†2:*‡(;4956*2(5*—4)8¶8*"
                             ";4069285);)6†8)4‡‡;1(‡9;48081;8:8‡1;48†85;4)485†528806*81 (‡9;48;(88;4(‡?34;48)4‡;161;:188;‡?";
    
    int frequency[MAX_CHARS] = {0};

    analyzeFrequency(ciphertext, frequency);
    printFrequency(frequency);

    return 0;
}