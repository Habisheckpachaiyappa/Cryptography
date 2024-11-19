#include <stdio.h>
#include <string.h>

void caesarCipher(char *text, int shift) {
    char ch;
    for (int i = 0; text[i] != '\0'; i++) {
        ch = text[i];

        // Encrypt uppercase letters
        if (ch >= 'A' && ch <= 'Z') {
            ch = (ch - 'A' + shift) % 26 + 'A';
            text[i] = ch;
        }
        // Encrypt lowercase letters
        else if (ch >= 'a' && ch <= 'z') {
            ch = (ch - 'a' + shift) % 26 + 'a';
            text[i] = ch;
        }
        // Non-alphabetic characters remain unchanged
    }
}

int main() {
    char text[100];
    int k;

    printf("Enter the text to be encrypted: ");
    fgets(text, sizeof(text), stdin);
    
    // Remove newline character from fgets
    text[strcspn(text, "\n")] = '\0';

    printf("Enter the shift value (1-25): ");
    scanf("%d", &k);

    // Ensure k is within the range 1-25
    if (k < 1 || k > 25) {
        printf("Shift value must be between 1 and 25.\n");
        return 1;
    }

    caesarCipher(text, k);
    printf("Encrypted text: %s\n", text);

    return 0;
}