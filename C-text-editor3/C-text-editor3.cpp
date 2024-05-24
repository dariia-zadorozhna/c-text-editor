#include <stdio.h>
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
                break;
            case '4':
                printf("Enter the file name for loading:\n");
                get_input(input, bufferSize);
                break;
            case '5':
                printf("The current text is:\n");
                break;
            case '6':
                printf("Choose line and index:\n");
                break;
            case '7':
                printf("Enter text to search:\n");
                get_input(input, bufferSize);
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

