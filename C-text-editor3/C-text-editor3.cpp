#include <stdio.h>
#include <stdlib.h> // for dynamic memory allocation
#include <string.h>
#define  _GNU_SOURCE

void print_commands();
char* get_input(char* input, int bufferSize);
void append_text(int LineNewLength, char* input, char** text, int currentLineNum,int bufferSize);
void start_new_line(char*** text, int* numberOfRaws, int* currentLineNum, size_t bufferSize);
void save_to_file(char** text, int numberOfRaws,char* input,int bufferSize);
void load_from_file(char*** text, char* mystring, int bufferSize, int* numberOfRaws, char* input, int* currentLineNum);
void print_to_console(int numberOfRaws, int bufferSize, char** text);
void insert_text(char* temporary, int bufferSize, char** text, int line, int index, char* input);
void search(int substringLen, char* input, int numberOfRaws, int match_bool, char** text, int matches_num,int bufferSize);

int main() {
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
    char* input = (char*)malloc(bufferSize * sizeof(char));
    char** text = (char**)calloc(numberOfRaws, sizeof(char*));
    text[0] = (char*)calloc(bufferSize, sizeof(char));

    if (input == NULL || mystring == NULL || temporary == NULL || text == NULL || text[0] == NULL) {
        printf("Memory allocation failed. Exiting...\n");
        free(input);
        free(mystring);
        free(temporary);
        free(text);
        return 1;
    }

    print_commands();

    while (1) {
        /*print_commands();*/
        printf("Type in the next command:\n");
        get_input(input, bufferSize);
        if (strlen(input) == 1)
        {
            switch (input[0]) {
            case '1':
                append_text(LineNewLength,input,text,currentLineNum,bufferSize);
                break;
            case '2':
                start_new_line(&text, &numberOfRaws, &currentLineNum, bufferSize); 
                break;
            case '3':
                save_to_file(text, numberOfRaws, input,bufferSize);
                break;
            case '4':
                load_from_file(&text,mystring,bufferSize,&numberOfRaws,input, &currentLineNum);
                break;
            case '5':
                print_to_console(numberOfRaws, bufferSize, text);
                break;
            case '6':
                /*system("CLS");*/
                printf("Choose line and index:\n");
                if ((scanf_s("%d %d", &line, &index)) == 2) {
                    while (getchar() != '\n') {
                        continue;
                    }

                    if (line <= numberOfRaws && line >= 0 && index >= 0) {
                        insert_text(temporary, bufferSize, text, line, index, input);
                    }
                    else {
                        printf("This line does not exist yet or the index is wrong.\n");
                    }
                }
                else {
                    printf("The entered line and index are incorrect.\n");
                }
                break;
            case '7':
                search(substringLen,input,numberOfRaws, match_bool, text,matches_num,bufferSize);
                break;
            case '8':
                /*system("CLS");*/
                printf("Exiting...\n");
                for (int i = 0; i < numberOfRaws; i++)
                {
                    (text[i]);
                }
                free(input);
                free(text);
                free(temporary);
                free(mystring);
                return 0;
            default:
                printf("Invalid command. Try again:\n");
            }
        }
        else {
            /*system("CLS");*/
            printf("The command is invalid. Try again!\n");
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

void append_text(int LineNewLength, char*input, char** text, int currentLineNum,int bufferSize) {
   /* system("CLS");*/
    printf("Enter text to append:\n");
    get_input(input, bufferSize);
    LineNewLength = strlen(input) + strlen(text[currentLineNum]) + 1;
    text[currentLineNum] = (char*)realloc(text[currentLineNum], LineNewLength * sizeof(char));
    if (text[currentLineNum] == NULL) {
        printf("Memory allocation failed.\n");
    }
    strcat_s(text[currentLineNum], LineNewLength, input);
}

void start_new_line(char*** text, int* numberOfRaws, int* currentLineNum, size_t bufferSize) {
    /*system("CLS");*/
    printf("New line is started\n");
    (*numberOfRaws)++;
    (*currentLineNum)++;
    *text = (char**)realloc(*text, (*numberOfRaws) * sizeof(char*));
    (*text)[*currentLineNum] = (char*)calloc(bufferSize, sizeof(char));
    if (*text == NULL || (*text)[*currentLineNum] == NULL) {
        printf("Memory allocation failed.\n");
    }
}

static void save_to_file(char** text, int numberOfRaws, char* input, int bufferSize){
    /*system("CLS");*/
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
}

static void load_from_file(char*** text, char* mystring, int bufferSize, int* numberOfRaws, char* input, int* currentLineNum) {
    /*system("CLS");*/
    printf("Enter the file name for loading:\n");
    get_input(input, bufferSize);

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
            if (*text == NULL || (*text)[*currentLineNum] == NULL) {
                printf("Memory allocation failed.\n");
            }
            strcpy_s((*text)[*numberOfRaws], bufferSize, mystring);
            (*numberOfRaws)++;
            (*currentLineNum)++;
        }
        fclose(file);
    }
}

void print_to_console(int numberOfRaws,int bufferSize, char** text) {
    /*system("CLS");*/
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
    printf("Enter text to insert:\n");
    get_input(input, bufferSize);
    strcpy_s(temporary, bufferSize, text[line - 1]);
    temporary[index] = '\0';
    strcat_s(temporary, bufferSize, input);
    strcat_s(temporary, bufferSize, text[line - 1] + index);
    text[line - 1] = (char*)calloc(bufferSize, sizeof(char));
    strcpy_s(text[line - 1], bufferSize, temporary);
    printf("New line is: %s\n", text[line - 1]);
}

static void search(int substringLen,char* input,int numberOfRaws,int match_bool, char** text, int matches_num, int bufferSize) {
    /*system("CLS");*/
    printf("Enter text to search:\n");
    get_input(input, bufferSize);
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
