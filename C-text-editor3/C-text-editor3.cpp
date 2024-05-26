#include <stdio.h>
#include <stdlib.h> // for dynamic memory allocation
#include <string.h>
#define  _GNU_SOURCE

void print_commands();
char* get_input(char* input, int bufferSize);
void append_text(int LineNewLength, char* input, char** text, int currentLineNum);
void start_new_line(char*** text, int* numberOfRaws, int* currentLineNum, size_t bufferSize);
void save_to_file(char** text, int numberOfRaws,char* input);
void load_from_file(char*** text, char* mystring, int bufferSize, int* numberOfRaws, char* input);
void print_to_console(int numberOfRaws, int bufferSize, char** text);
void insert_text(char* temporary, int bufferSize, char** text, int line, int index, char* input);
void search(int substringLen, char* input, int numberOfRaws, int match_bool, char** text, int matches_num);

int main() {
    char* input = 0;
    const size_t bufferSize = 256;
    int numberOfRaws = 1;
    int LineNewLength = 0;
    int currentLineNum = 0;
    int matches_num = 0;
    int match_bool = 0;
    int substringLen = 0;
    int line, index;

    char* mystring = (char*)calloc(bufferSize, sizeof(char));
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
                system("CLS");
                printf("Enter text to append:\n");
                get_input(input, bufferSize);
                append_text(LineNewLength,input,text,currentLineNum);
                break;
            case '2':
                start_new_line(&text, &numberOfRaws, &currentLineNum, bufferSize); 
                break;
            case '3':
                system("CLS");
                printf("Enter the file name for saving: \n");
                get_input(input, bufferSize);
                save_to_file(text, numberOfRaws, input);
                break;
            case '4':
                system("CLS");
                printf("Enter the file name for loading:\n");
                get_input(input, bufferSize);
                load_from_file(&text,mystring,bufferSize,&numberOfRaws,input);
                break;
            case '5':
                system("CLS");
                print_to_console(numberOfRaws, bufferSize, text);
                break;
            case '6':
                system("CLS");
                printf("Choose line and index:\n");
                scanf_s("%d %d", &line, &index);
                getchar();
                printf("Enter text to insert:\n");
                get_input(input, bufferSize);
                insert_text(temporary,bufferSize,text,line,index,input);
                break;
            case '7':
                system("CLS");
                printf("Enter text to search:\n");
                get_input(input, bufferSize);
                search(substringLen,input,numberOfRaws, match_bool, text,matches_num);
                break;
            case '8':
                system("CLS");
                printf("Exiting...\n");
                for (int i = 0; i < numberOfRaws; i++)
                {
                    (text[i]);
                }
                free(input);
                free(text);
                free(temporary);
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

void append_text(int LineNewLength, char*input, char** text, int currentLineNum) {
    LineNewLength = strlen(input) + strlen(text[currentLineNum]) + 1;
    text[currentLineNum] = (char*)realloc(text[currentLineNum], LineNewLength * sizeof(char));
    strcat_s(text[currentLineNum], LineNewLength, input);
}

void start_new_line(char*** text, int* numberOfRaws, int* currentLineNum, size_t bufferSize) {
    system("CLS");
    printf("New line is started\n");
    (*numberOfRaws)++;
    (*currentLineNum)++;
    *text = (char**)realloc(*text, (*numberOfRaws) * sizeof(char*));
    (*text)[*currentLineNum] = (char*)calloc(bufferSize, sizeof(char));
}

static void save_to_file(char** text, int numberOfRaws, char* input){
    FILE* file;

    fopen_s(&file, input, "w");
    if (file != NULL) {
        for (int i = 0; i < numberOfRaws; i++) {
            fputs(text[i], file);
            fputs("\n", file);
        }
    }
    fclose(file);
}

static void load_from_file(char*** text, char* mystring, int bufferSize, int* numberOfRaws, char* input) {

    FILE* file;

    fopen_s(&file, input, "r");
    if (file == NULL) {
        printf("Error opening file!");
    }
    else {
        while (fgets(mystring, bufferSize, file))
        {
            mystring[strcspn(mystring, "\n")] = '\0';
            *text = (char**)realloc(*text, ((*numberOfRaws) + 1) * sizeof(char*));
            (*text)[*numberOfRaws] = (char*)calloc(bufferSize, sizeof(char));
            strcpy_s((*text)[*numberOfRaws], bufferSize, mystring);
            (*numberOfRaws)++;
        }
        fclose(file);
    }
}

void print_to_console(int numberOfRaws,int bufferSize, char** text) {
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
}

static void insert_text(char* temporary,int bufferSize, char** text, int line, int index, char* input) {
    strcpy_s(temporary, bufferSize, text[line - 1]);
    temporary[index] = '\0';
    strcat_s(temporary, bufferSize, input);
    strcat_s(temporary, bufferSize, text[line - 1] + index);
    text[line - 1] = (char*)calloc(bufferSize, sizeof(char));
    strcpy_s(text[line - 1], bufferSize, temporary);
    printf("New line is: %s\n", text[line - 1]);
}

static void search(int substringLen,char* input,int numberOfRaws,int match_bool, char** text, int matches_num) {
    substringLen = strlen(input);
    for (int i = 0; i < numberOfRaws; i++)
    {
        char* currentLine = text[i];
        int lineLen = strlen(currentLine);

        for (int j = 0; j <= lineLen - substringLen; j++)
        {
            match_bool = 1;
            for (int l = 0; l < substringLen; l++)
            {
                if (currentLine[j + l] != input[l])
                {
                    match_bool = 0;
                    break;
                }
            }
            if (match_bool) {
                matches_num += 1;
                printf("Text is present at this position: %d %d\n", i, j);
            }
        }
    }
    printf("Number of matches: %d\n", matches_num);
    matches_num = 0;
}

