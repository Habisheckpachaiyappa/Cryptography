#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 5

void createMatrix(char *key, char matrix[SIZE][SIZE]) {
    int used[26] = {0}, k = 0;
    for (int i = 0; key[i]; i++) {
        char ch = toupper(key[i]);
        if (isalpha(ch) && !used[ch - 'A']) {
            used[ch - 'A'] = 1;
            if (ch == 'J') ch = 'I'; // Treat 'J' as 'I'
            matrix[k / SIZE][k % SIZE] = ch; k++;
        }
    }
    for (char ch = 'A'; ch <= 'Z'; ch++) {
        if (!used[ch - 'A'] && ch != 'J') {
            matrix[k / SIZE][k % SIZE] = ch; k++;
        }
    }
}

void preparePlaintext(char *plaintext, char *prepared) {
    int j = 0;
    for (int i = 0; plaintext[i]; i++) {
        char ch = toupper(plaintext[i]);
        if (isalpha(ch)) prepared[j++] = ch;
    }
    for (int i = 0; i < j; i += 2) {
        if (i + 1 == j || prepared[i] == prepared[i + 1]) {
            prepared[i + 1] = 'X'; j++;
        }
    }
    prepared[j] = '\0';
}

void encryptPair(char a, char b, char matrix[SIZE][SIZE], char *ciphertext) {
    int row1, col1, row2, col2;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (matrix[i][j] == a) { row1 = i; col1 = j; }
            if (matrix[i][j] == b) { row2 = i; col2 = j; }
        }
    }
    if (row1 == row2) {
        ciphertext[0] = matrix[row1][(col1 + 1) % SIZE];
        ciphertext[1] = matrix[row2][(col2 + 1) % SIZE];
    } else if (col1 == col2) {
        ciphertext[0] = matrix[(row1 + 1) % SIZE][col1];
        ciphertext[1] = matrix[(row2 + 1) % SIZE][col2];
    } else {
        ciphertext[0] = matrix[row1][col2];
        ciphertext[1] = matrix[row2][col1];
    }
}

int main() {
    char key[100], plaintext[100], prepared[100], ciphertext[100], matrix[SIZE][SIZE];
    printf("Enter keyword: "); fgets(key, sizeof(key), stdin);
    printf("Enter plaintext: "); fgets(plaintext, sizeof(plaintext), stdin);
    
    createMatrix(key, matrix);
    preparePlaintext(plaintext, prepared);
    
    int len = strlen(prepared);
    for (int i = 0; i < len; i += 2) {
        encryptPair(prepared[i], prepared[i + 1], matrix, ciphertext + i);
    }
    ciphertext[len] = '\0';
    printf("Ciphertext: %s\n", ciphertext);
    return 0;
}