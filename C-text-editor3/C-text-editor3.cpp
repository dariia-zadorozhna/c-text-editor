﻿#include <stdio.h>
#include <stdlib.h> // for dynamic memory allocation
#include <string.h>
#define  _GNU_SOURCE

void print_commands();
char* get_input(char* input, int bufferSize);

int main() {
    char* input = 0;
    const size_t bufferSize = 256;
    int numberOfRaws = 1;
    int LineNewLength = 0;
    int currentLineNum = 0;
    int matches = 0;
    int match = 0;
    int substringLen = 0;

    char* temporary = (char*)calloc(bufferSize, sizeof(char));

    input = (char*)malloc(bufferSize * sizeof(char));

    if (input == NULL) {
        printf("Memory allocation failed. Exiting...\n");
        return 1;
    }

    char** text = (char**)calloc(numberOfRaws, sizeof(char*));
    text[0] = (char*)calloc(bufferSize, sizeof(char));

    while (1) {
        print_commands();
        get_input(input, bufferSize);
        if (strlen(input) == 1)
        {
            switch (input[0]) {
            case '1':
                printf("Enter text to append:\n");
                get_input(input, bufferSize);
                LineNewLength = strlen(input) + strlen(text[currentLineNum]) + 1;
                text[currentLineNum] = (char*)realloc(text[currentLineNum], LineNewLength * sizeof(char));
                strcat_s(text[currentLineNum], LineNewLength, input);
                break;
            case '2':
                printf("New line is started\n");
                numberOfRaws++;
                currentLineNum++;
                text = (char**)realloc(text, numberOfRaws * sizeof(char*));
                text[currentLineNum] = (char*)calloc(bufferSize, sizeof(char));
                break;
            case '3':
                printf("Enter the file name for saving: \n");
                get_input(input, bufferSize);

                FILE* file;

                fopen_s(&file, input, "w");
                if (file != NULL) {
                    for (int i = 0; i < numberOfRaws; i++) {
                        fputs(text[i], file);
                        fputs("\n", file);
                    }
                }
                fclose(file);
                break;
            case '4':
                printf("Enter the file name for loading:\n");
                get_input(input, bufferSize);
                char mystring[bufferSize];

                fopen_s(&file, input, "r");
                if (file == NULL) {
                    printf("Error opening file!");
                }
                else {
                    while (fgets(mystring, bufferSize, file))
                    {
                        mystring[strcspn(mystring, "\n")] = '\0';
                        text = (char**)realloc(text, (numberOfRaws + 1) * sizeof(char*));
                        text[numberOfRaws] = (char*)calloc(bufferSize, sizeof(char));
                        strcpy_s(text[numberOfRaws], bufferSize, mystring);
                        numberOfRaws++;
                    }
                    fclose(file);
                }
                break;
            case '5':
                printf("The current text is:\n");
                for (int i = 0; i < numberOfRaws; i++) {
                    for (int j = 0; j < bufferSize; j++)
                    {
                        if (text[i][j] == '\0') {
                            printf("\n");
                            break;
                        }
                        printf("%c", text[i][j]);
                    }
                }
                break;
            case '6':
                int line, index;
                printf("Choose line and index:\n");
                scanf_s("%d %d", &line, &index);
                getchar();
                printf("Enter text to insert:\n");
                get_input(input, bufferSize);
                strcpy_s(temporary, bufferSize, text[line - 1]);
                temporary[index] = '\0';
                strcat_s(temporary, bufferSize, input);
                strcat_s(temporary, bufferSize, text[line - 1] + index);
                text[line - 1] = (char*)calloc(bufferSize, sizeof(char));
                strcpy_s(text[line - 1], bufferSize, temporary);
                printf("New line is: %s\n", text[line - 1]);
                break;
            case '7':
                printf("Enter text to search:\n");
                get_input(input, bufferSize);
                substringLen = strlen(input);
                for (int i = 0; i < numberOfRaws; i++)
                {
                    char* currentLine = text[i];
                    int lineLen = strlen(currentLine);

                    for (int j = 0; j <= lineLen - substringLen; j++)
                    {
                        match = 1;
                        for (int l = 0; l < substringLen; l++)
                        {
                            if (currentLine[j + l] != input[l])
                            {
                                match = 0;
                            }
                        }
                        if (match) {
                            matches += 1;
                        }
                    }
                }
                printf("Your matches are: %d\n", matches);
                break;
            case '8':
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid command. Try again:\n");
            }
        }

    }
    return 0;
}
void print_commands() {
    printf("Choose a command:\n"
        "1 - Append text symbols to the end\n"
        "2 - Start the new line\n"
        "3 - Save text to the file\n"
        "4 - Load text from the file\n"
        "5 - Print the current text to console\n"
        "6 - Insert the text by line and symbol index\n"
        "7 - Search\n"
        "8 - Exit\n");
}

char* get_input(char* input, int bufferSize) {
    fgets(input, bufferSize, stdin);
    input[strcspn(input, "\n")] = '\0';
    return input;
}

