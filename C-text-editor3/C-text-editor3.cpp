﻿#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;

class TextEditor
{
public:
    TextEditor(int bufSize);
    ~TextEditor();

    void run();
private:
    void print_commands();
    void get_input();
    void append_text();
    void start_new_line();
    void save_to_file();
    void load_from_file();
    void print_to_console();
    void search();
    void insert();
    void insert_with_replacement();
    void delete_command();
    void cut();
    void paste();
    void copy();

    int bufferSize;
    int numberOfRaws;
    int currentLineNum;
    int counter;
    char* input;
    char* cut_copy_paste;
    char** text;
    int cut_copy_paste_index;
};
TextEditor::TextEditor(int BufSize)
    :bufferSize(BufSize), numberOfRaws(1), currentLineNum(0), counter(0), cut_copy_paste_index(0) {
    try {
        input = new char[bufferSize];
        cut_copy_paste = new char[bufferSize];
        text = new char* [numberOfRaws];
        text[0] = new char[bufferSize];
        input[0] = '\0';
        text[0][0] = '\0';
    }
    catch (const bad_alloc& e) {
        cerr << "Memory allocation failed: " << e.what() << ". Exiting..." << endl;
        exit(1);
    }
}
TextEditor::~TextEditor() {
    for (int i = 0; i < numberOfRaws; i++) {
        delete[] text[i];
    }
    delete[] input;
    delete[] text;
}

void TextEditor::print_commands() {
    cout << "Choose a command:\n"
        << "1 - Append text symbols to the end\n"
        << "2 - Start the new line\n"   
        << "3 - Save text to the file\n"
        << "4 - Load text from the file\n"
        << "5 - Print the current text to console\n"
        << "6 - Insert the text by line and symbol index\n"
        << "7 - Search\n"
        << "8 - Delete\n"
        << "9 - Undo\n"
        << "11 - Cut\n"
        << "12 - Paste\n"
        << "13 - Copy\n"
        << "14 - Insert the text by line and symbol index with replacement\n"
        << "15 - Exit\n";
}

void TextEditor::get_input() {
    fgets(input, bufferSize, stdin);
    input[strcspn(input, "\n")] = '\0';

}

void TextEditor::append_text() {
    int LineNewLength = 0;
    system("CLS");
    cout << "Enter text to append:\n";
    get_input();

    LineNewLength = strlen(input) + strlen(text[currentLineNum]) + 1;

    try {
        char* newLine = new char[LineNewLength];
        strcpy_s(newLine, LineNewLength, text[currentLineNum]); 
        strcat_s(newLine, LineNewLength, input); 
        delete[] text[currentLineNum];
        text[currentLineNum] = newLine;
    }
    catch (const bad_alloc& e) {
        cerr << "Memory reallocation failed: " << e.what() << ". Exiting..." << endl;
        exit(1);
    }
}

void TextEditor::start_new_line() {
    system("CLS");
    cout << "New line is started\n";
    numberOfRaws++;
    currentLineNum++;
    try {
        char** newText = new char*[numberOfRaws];
        int size = 0;
        for (int i = 0; i < currentLineNum; i++) {
            while (text[i][size] != '\0') {
                ++size;
            }
            newText[i] = new char[size + 1];  
            memcpy(newText[i], text[i], size);
            newText[i][size] = '\0';
            delete[] text[i];
        }
        newText[currentLineNum] = new char[bufferSize];
        newText[currentLineNum][0] = '\0';
        delete[] text;
        text = newText;
        
    }   
    catch (const bad_alloc& e) {
        cerr << "Memory reallocation failed: " << e.what() << ". Exiting..." << endl;
        exit(1);
    }
}

void TextEditor::save_to_file(){
    system("CLS");
    cout << "Enter the file name for saving: \n";
    get_input();

    FILE* file;

    fopen_s(&file, input, "w");
    if (file != NULL) {
        for (int i = 0; i < numberOfRaws; i++) {
            fputs(text[i], file);
            fputs("\n", file);
        }
    }
    fclose(file);
    file = NULL;
}

void TextEditor::load_from_file() {
    char* mystring = new char[bufferSize];
    system("CLS");
    cout << "Enter the file name for loading:\n";
    get_input();

    FILE* file;

    fopen_s(&file, input, "r");
    if (file == NULL) {
        cout << "Error opening file!";
    }
    else {
        while (fgets(mystring, bufferSize, file))
        {
            numberOfRaws++;
            currentLineNum++;

            mystring[strcspn(mystring, "\n")] = '\0';

            try {
                char** newText = new char* [numberOfRaws];
                int size = 0;
                for (int i = 0; i < currentLineNum; i++) {
                    while (text[i][size] != '\0') {
                        ++size;
                    }
                    newText[i] = new char[size + 1];
                    memcpy(newText[i], text[i], size);
                    newText[i][size] = '\0';
                    delete[] text[i];
                }
                newText[currentLineNum] = new char[bufferSize];
                newText[currentLineNum][0] = '\0';
                memcpy(newText[currentLineNum], mystring, bufferSize);
                delete[] text;
                text = newText;

            }
            catch (const bad_alloc& e) {
                cerr << "Memory reallocation failed: " << e.what() << ". Exiting..." << endl;
                exit(1);
            }
        }
        fclose(file);
        file = NULL;
    }
}

void TextEditor::print_to_console() {
    system("CLS");
    cout << "The current text is:\n";
    for (int i = 0; i < numberOfRaws; i++) {
        
            cout << text[i] << endl;
        }
}

void TextEditor::search() {
    int matchesNum = 0;
    int matchBool = 0;
    int substringLen = 0;
    system("CLS");
    cout << "Enter text to search:\n";
    get_input();
    substringLen = strlen(input);
    for (int i = 0; i < numberOfRaws; i++)
    {
        char* currentLine = text[i];
        int lineLen = strlen(currentLine);

        for (int j = 0; j <= lineLen - substringLen; j++)
        {
            matchBool = 1;
            for (int l = 0; l < substringLen; l++)
            {
                if (currentLine[j + l] != input[l])
                {
                    matchBool = 0;
                    break;
                }
            }
            if (matchBool) {
                matchesNum += 1;
                cout << "Text is present at this position:"<< i << " " <<j<<endl;
            }
        }
    }
    cout << "Number of matches:" << matchesNum << endl;
}

void TextEditor::insert() {
    char* temporary = new char[bufferSize];
    system("CLS");
    int line, index;
    cout <<"Choose line and index:\n";
    if ((scanf_s("%d %d", &line, &index)) == 2) {
        while (getchar() != '\n') {
            continue;
        }

        if (line <= numberOfRaws && line >= 0 && index >= 0) {
            cout << "Enter text to insert:\n";
            get_input();
            strcpy_s(temporary, bufferSize, text[line]);
            temporary[index] = '\0';
            strcat_s(temporary, bufferSize, input);
            strcat_s(temporary, bufferSize, text[line] + index);
            text[line] = new char[bufferSize];
            strcpy_s(text[line], bufferSize, temporary);
            cout << "New line is: " << text[line] << endl;
        }
        else {
            cout << "The entered line and index are incorrect. \n";
        }
    }
    else {
        cout << "The entered line and index are incorrect.\n";
    }
}

void TextEditor::insert_with_replacement() {
    system("CLS");
    int line, index = 0;
    int size = 0;
    cout << "Choose line and index:\n";
    if ((scanf_s("%d %d", &line, &index)) == 2) {
        while (getchar() != '\n') {
            continue;
        }
        if (line <= numberOfRaws && line >= 0 && index >= 0) {
            cout << "Enter text to insert:\n";
            get_input();
        }   
    }
    for (int i = 0; i < strlen(input); i++) 
    {
        text[line][index + i] = input[i];
    }
}

void TextEditor::delete_command() {
    system("CLS");
    int line, index, num = 0;
    //int counter = 0;              
    cout << "Choose line, index and number of symbols:\n";
    if ((scanf_s("%d %d %d", &line, &index, &num)) == 3) {
        while (getchar() != '\n') {
            continue;
        }
        if (line <= numberOfRaws && line >= 0 && index >= 0) {
            while (text[line][counter] != '\0') {
                counter++;
            }
            while (index != counter) {
                text[line][index] = text[line][index + num];
                index++;
            }
            counter = 0;
        }
        else {
            cout << "The entered line and index are incorrect.\n";
        }
    }
    else {
        cout << "The entered line and index are incorrect.\n";
    }
}

void TextEditor::cut() {
    system("CLS");
    int line, index, num = 0;
    cout << "Choose line, index and number of symbols:\n";
    if ((scanf_s("%d %d %d", &line, &index, &num)) == 3) {
        while (getchar() != '\n') {
            continue;
        }

        if (line <= numberOfRaws && line >= 0 && index >= 0) {
            while (text[line][counter] != '\0') {
                counter++;
            }
            while (index != counter) {
                if (cut_copy_paste_index != num) {
                    cut_copy_paste[cut_copy_paste_index] = text[line][index];
                    cut_copy_paste_index++;
                }
                else {
                    cut_copy_paste[cut_copy_paste_index] = '\0';
                }
                text[line][index] = text[line][index + num];
                index++;
            }
            cut_copy_paste_index = 0;
            counter = 0;
        }
        else {
            cout << "The entered line and index are incorrect.\n";
        }
    }
    else {
        cout << "The entered line and index are incorrect.\n";
    }
}

void TextEditor::paste() {
    char* temporary = new char[bufferSize];
    system("CLS");
    int line, index;
    cout << "Choose line and index:\n";
    if ((scanf_s("%d %d", &line, &index)) == 2) {
        while (getchar() != '\n') {
            continue;
        }

        if (line <= numberOfRaws && line >= 0 && index >= 0 && cut_copy_paste != NULL) {
            strcpy_s(temporary, bufferSize, text[line]);
            temporary[index] = '\0';
            strcat_s(temporary, bufferSize, cut_copy_paste);
            strcat_s(temporary, bufferSize, text[line] + index);
            delete[] text[line]; 
            text[line] = new char[bufferSize];
            strcpy_s(text[line], bufferSize, temporary);
            cout << "New line is: " << text[line] << endl;
        }
        else {
            cout << "The entered line and index are incorrect.\n";
        }
    }
    else {
        cout << "The entered line and index are incorrect.\n";
    }
    delete[] temporary;
}

void TextEditor::copy() {
    int line, index, num = 0;
    system("CLS");
    cout << "Choose line, index and number of symbols:\n";
    if ((scanf_s("%d %d %d", &line, &index, &num)) == 3) {
        while (getchar() != '\n') {
            continue;
        }
        if (line <= numberOfRaws && line >= 0 && index >= 0)
        {
            while (cut_copy_paste_index != num) {
                cut_copy_paste[cut_copy_paste_index] = text[line][index + cut_copy_paste_index];
                cut_copy_paste_index++;
            }
            cut_copy_paste[cut_copy_paste_index] = '\0';
            cut_copy_paste_index = 0;
        }
        else {
            cout << "The entered line and index are incorrect.\n";
        }
    }
    else {
        cout << "The entered line and index are incorrect.\n";
    }
}



void TextEditor::run() {
    while (true) {
        print_commands();
        get_input();
        int command = atoi(input);
        switch (command) {
            case 1:
                append_text();
                break;
            case 2:
                start_new_line();
                break;
            case 3:
                save_to_file();
                break;
            case 4:
                load_from_file();
                break;
            case 5:
                print_to_console();
                break;
            case 6:
                insert();
                break;
            case 7:
                search();
                break;
            case 8:
                delete_command();
                break;
            case 9:
                // implemented command
                break;
            case 10:
                // implemented command
                break;
            case 11:
                cut();
                break;
            case 12:
                paste();
                break;
            case 13:
                copy();
                break;
            case 14:
                insert_with_replacement();
                break;
            case 15:
                system("CLS");
                cout << "Exiting...\n";
                return;
            default:
                cout << "Invalid command. Try again:\n";
        }
    }
}

int main() {
    TextEditor editor(256);
    editor.run();
    return 0;

}
