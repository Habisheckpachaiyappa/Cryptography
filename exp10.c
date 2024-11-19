#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MATRIX_SIZE 5
#define MAX_TEXT_LENGTH 256

void createPlayfairMatrix(char matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    char key[] = "MFHIKUNOPQZVWXYELARGDSBTC";
    int index = 0;

    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matrix[i][j] = key[index++];
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

void encryptPlayfair(const char *preparedText, const char matrix[MATRIX_SIZE][MATRIX_SIZE], char *encryptedText) {
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
            encryptedText[j++] = matrix[row1][(col1 + 1) % MATRIX_SIZE];
            encryptedText[j++] = matrix[row2][(col2 + 1) % MATRIX_SIZE];
        } else if (col1 == col2) {
            encryptedText[j++] = matrix[(row1 + 1) % MATRIX_SIZE][col1];
            encryptedText[j++] = matrix[(row2 + 1) % MATRIX_SIZE][col2];
        } else {
            encryptedText[j++] = matrix[row1][col2];
            encryptedText[j++] = matrix[row2][col1];
        }
    }
    encryptedText[j] = '\0';
}

int main() {
    char matrix[MATRIX_SIZE][MATRIX_SIZE];
    char preparedText[MAX_TEXT_LENGTH];
    char encryptedText[MAX_TEXT_LENGTH];

    createPlayfairMatrix(matrix);

    char message[] = "Must see you over Cadogan West. Coming at once.";
    prepareText(message, preparedText);
    encryptPlayfair(preparedText, matrix, encryptedText);

    printf("Encrypted Message: %s\n", encryptedText);
    return 0;
}