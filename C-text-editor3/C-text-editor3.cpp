#include <stdio.h>
#include <stdlib.h> // for dynamic memory allocation
#include <string.h>
#define  _GNU_SOURCE
#define BUFFER_SIZE 256

typedef struct TextEditor {
    char** text;
    int numberOfRaws;
    int currentLineNum;

    // Methods
    void (*print_commands)();
    char* (*get_input)(char* input);
    void (*append_text)(char* input, struct TextEditor* editor);
    void (*start_new_line)(struct TextEditor* editor);
    void (*save_to_file)(char* input, struct TextEditor* editor);
    void (*load_from_file)(char* input, struct TextEditor* editor);
    void (*print_to_console)(struct TextEditor* editor);
    void (*search)(char* input, struct TextEditor* editor);
    void (*insert)(char* input, struct TextEditor* editor);
} TextEditor;

void print_commands();
char* get_input(char* input);
void append_text(char* input, TextEditor* editor);
void start_new_line(TextEditor* editor);
void save_to_file(char* input,TextEditor* editor);
void load_from_file(char* input, TextEditor* editor);
void print_to_console(TextEditor* editor);
void search(char* input, TextEditor* editor);
void insert(char* input, TextEditor* editor);

int main() {
    TextEditor editor = { 0 };
    editor.numberOfRaws = 1;
    editor.currentLineNum = 0;
    editor.text = (char**)calloc(editor.numberOfRaws, sizeof(char*));
    editor.text[0] = (char*)calloc(BUFFER_SIZE, sizeof(char));
    char* input = (char*)malloc(BUFFER_SIZE * sizeof(char));

    editor.print_commands = print_commands;
    editor.get_input = get_input;
    editor.append_text = append_text;
    editor.start_new_line = start_new_line;
    editor.save_to_file = save_to_file;
    editor.load_from_file = load_from_file;
    editor.print_to_console = print_to_console;
    editor.search = search;
    editor.insert = insert;

   /* if (input == NULL || text == NULL || text[0] == NULL) {
        printf("Memory allocation failed. Exiting...\n");
        free(input);
        free(text);
        return 1;
    }*/

    while (1) {
        editor.print_commands();
        editor.get_input(input);
        if (strlen(input) == 1)
        {
            switch (input[0]) {
            case '1':
                editor.append_text(input,&editor);
                break;
            case '2':
                editor.start_new_line(&editor); 
                break;
            case '3':
                editor.save_to_file(input, &editor);
                break;
            case '4':
                editor.load_from_file(input,&editor);
                break;
            case '5':
                editor.print_to_console(&editor);
                break;
            case '6':
                editor.insert(input,&editor);
                break;
            case '7':
                editor.search(input,&editor);
                break;
            case '8':
                system("CLS");
                printf("Exiting...\n");
                for (int i = 0; i < editor.numberOfRaws; i++)
                {
                    free(editor.text[i]);
                }
                free(input);
                free(editor.text);
                return 0;
            default:
                printf("Invalid command. Try again:\n");
            }
        }
        else {
            system("CLS");
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

char* get_input(char* input) {
    fgets(input, BUFFER_SIZE, stdin);
    input[strcspn(input, "\n")] = '\0';
    return input;
}

void append_text(char*input,TextEditor* editor) {
    int LineNewLength = 0;
    system("CLS");
    printf("Enter text to append:\n");
    get_input(input);
    LineNewLength = strlen(input) + strlen(editor->text[editor->currentLineNum]) + 1;
    editor->text[editor->currentLineNum] = (char*)realloc(editor->text[editor->currentLineNum], LineNewLength * sizeof(char));
    if (editor->text[editor->currentLineNum] == NULL) {
        printf("Memory allocation failed.\n");
    }
    strcat_s(editor->text[editor->currentLineNum], LineNewLength, input);
}

void start_new_line(TextEditor* editor) {
    system("CLS");
    printf("New line is started\n");
    editor->numberOfRaws++;
    editor->currentLineNum++;
    editor->text = (char**)realloc(editor->text, editor->numberOfRaws * sizeof(char*));
    editor->text[editor->currentLineNum] = (char*)calloc(BUFFER_SIZE, sizeof(char));
    if (editor->text == NULL || (editor->text)[editor->currentLineNum] == NULL) {
        printf("Memory allocation failed.\n");
    }
}

static void save_to_file(char* input, TextEditor* editor){
    system("CLS");
    printf("Enter the file name for saving: \n");
    get_input(input);

    FILE* file;

    fopen_s(&file, input, "w");
    if (file != NULL) {
        for (int i = 0; i < editor->numberOfRaws; i++) {
            fputs(editor->text[i], file);
            fputs("\n", file);
        }
    }
    fclose(file);
}

static void load_from_file(char* input, TextEditor* editor) {
    char* mystring = (char*)calloc(BUFFER_SIZE, sizeof(char));
    if (mystring == NULL) {
        printf("Memory allocation failed.");
        free(mystring);
    }
    system("CLS");
    printf("Enter the file name for loading:\n");
    get_input(input);

    FILE* file;

    fopen_s(&file, input, "r");
    if (file == NULL) {
        printf("Error opening file!");
    }
    else {
        while (fgets(mystring, BUFFER_SIZE, file))
        {
            mystring[strcspn(mystring, "\n")] = '\0';
            editor->text = (char**)realloc(editor->text, (editor->numberOfRaws + 1) * sizeof(char*));
            editor->text[editor->numberOfRaws] = (char*)calloc(BUFFER_SIZE, sizeof(char));
            if (editor->text == NULL || editor->text[editor->currentLineNum] == NULL) {
                printf("Memory allocation failed.\n");
            }
            strcpy_s(editor->text[editor->numberOfRaws], BUFFER_SIZE, mystring);
            editor->numberOfRaws++;
            editor->currentLineNum++;
        }
        fclose(file);
    }
}

void print_to_console(TextEditor* editor) {
    system("CLS");
    printf("The current text is:\n");
    for (int i = 0; i < editor->numberOfRaws; i++) {
        for (int j = 0; j < BUFFER_SIZE; j++)
        {
            if (editor->text[i][j] == '\0') {
                printf("\n");
                break;
            }
            printf("%c", editor->text[i][j]);
        }
    }
}

static void search(char* input,TextEditor* editor) {
    int matches_num = 0;
    int match_bool = 0;
    int substringLen = 0;
    system("CLS");
    printf("Enter text to search:\n");
    get_input(input);
    substringLen = strlen(input);
    for (int i = 0; i < editor->numberOfRaws; i++)
    {
        char* currentLine = editor->text[i];
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
}

static void insert(char* input, TextEditor* editor) {
    char* temporary = (char*)calloc(BUFFER_SIZE, sizeof(char));
    if (temporary == NULL) {
        printf("Memory allocation failed.");
        free(temporary);
    }
    system("CLS");
    int line, index;
    printf("Choose line and index:\n");
    if ((scanf_s("%d %d", &line, &index)) == 2) {
        while (getchar() != '\n') {
            continue;
        }

        if (line <= editor->numberOfRaws && line >= 0 && index >= 0) {
            printf("Enter text to insert:\n");
            get_input(input);
            strcpy_s(temporary, BUFFER_SIZE, editor->text[line - 1]);
            temporary[index] = '\0';
            strcat_s(temporary, BUFFER_SIZE, input);
            strcat_s(temporary, BUFFER_SIZE, editor->text[line - 1] + index);
            editor->text[line - 1] = (char*)calloc(BUFFER_SIZE, sizeof(char));
            strcpy_s(editor->text[line - 1], BUFFER_SIZE, temporary);
            printf("New line is: %s\n", editor->text[line - 1]);
        }
        else {
            printf("This line does not exist yet or the index is wrong.\n");
        }
    }
    else {
        printf("The entered line and index are incorrect.\n");
    }
}
