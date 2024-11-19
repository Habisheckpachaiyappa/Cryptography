#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MATRIX_SIZE 5
#define MAX_TEXT_LENGTH 256

void createPlayfairMatrix(char *key, char matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    int used[26] = {0}, index = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        char ch = toupper(key[i]);
        if (isalpha(ch) && ch != 'J' && !used[ch - 'A']) {
            matrix[index / MATRIX_SIZE][index % MATRIX_SIZE] = ch;
            used[ch - 'A'] = 1;
            index++;
        }
    }
    for (char ch = 'A'; ch <= 'Z'; ch++) {
        if (ch == 'J') continue;
        if (!used[ch - 'A']) {
            matrix[index / MATRIX_SIZE][index % MATRIX_SIZE] = ch;
            index++;
        }
    }
}

void prepareText(const char *text, char *preparedText) {
    int j = 0;
    for (int i = 0; text[i] != '\0'; i++) {
        char ch = toupper(text[i]);
        if (isalpha(ch)) preparedText[j++] = ch;
    }
    preparedText[j] = '\0';
    for (int i = 0; i < j; i += 2) {
        if (i + 1 < j) {
            if (preparedText[i] == preparedText[i + 1]) {
                for (int k = j; k > i + 1; k--) preparedText[k + 1] = preparedText[k];
                preparedText[i + 1] = 'X'; j++;
            }
        } else {
            preparedText[j++] = 'X';
        }
    }
    preparedText[j] = '\0';
}

void decryptPlayfair(const char *preparedText, const char matrix[MATRIX_SIZE][MATRIX_SIZE], char *decryptedText) {
    int j = 0;
    for (int i = 0; preparedText[i] != '\0'; i += 2) {
        char first = preparedText[i], second = preparedText[i + 1];
        int row1, col1, row2, col2;
        for (int r = 0; r < MATRIX_SIZE; r++) {
            for (int c = 0; c < MATRIX_SIZE; c++) {
                if (matrix[r][c] == first) { row1 = r; col1 = c; }
                if (matrix[r][c] == second) { row2 = r; col2 = c; }
            }
        }
        if (row1 == row2) {
            decryptedText[j++] = matrix[row1][(col1 + MATRIX_SIZE - 1) % MATRIX_SIZE];
            decryptedText[j++] = matrix[row2][(col2 + MATRIX_SIZE - 1) % MATRIX_SIZE];
        } else if (col1 == col2) {
            decryptedText[j++] = matrix[(row1 + MATRIX_SIZE - 1) % MATRIX_SIZE][col1];
            decryptedText[j++] = matrix[(row2 + MATRIX_SIZE - 1) % MATRIX_SIZE][col2];
        } else {
            decryptedText[j++] = matrix[row1][col2];
            decryptedText[j++] = matrix[row2][col1];
        }
    }
    decryptedText[j] = '\0';
}

int main() {
    char key[] = "PT109", matrix[MATRIX_SIZE][MATRIX_SIZE], preparedText[MAX_TEXT_LENGTH], decryptedText[MAX_TEXT_LENGTH];
    char message[] = "KXJEY UREBE ZWEHE WRYTU HEYFS KREHE GOYFI WTTTU OLKSY CAJPO BOTEI ZONTX BYBNT GONEY CUZWR GDSON SXBOU YWRHE BAAHY USEDQ";
    createPlayfairMatrix(key, matrix);
    prepareText(message, preparedText);
    decryptPlayfair(preparedText, matrix, decryptedText);
    printf("Decrypted Message: %s\n", decryptedText);
    return 0;
}