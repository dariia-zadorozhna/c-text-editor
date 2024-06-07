#include <cstdio>
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

    int bufferSize;
    int numberOfRaws;
    int currentLineNum;
    char* input;
    char** text;
};
TextEditor::TextEditor(int BufSize)
    :bufferSize(BufSize), numberOfRaws(1), currentLineNum(0) {
    try {
        input = new char[bufferSize];
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
        << "8 - Insert the text by line and symbol index with replacement\n"
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
        memcpy(newLine, text[currentLineNum], LineNewLength);
        delete[] text[currentLineNum];
        text[currentLineNum] = newLine;
    }
    catch (const bad_alloc& e) {
        cerr << "Memory reallocation failed: " << e.what() << ". Exiting..." << endl;
        exit(1);
    }
    if (text[currentLineNum] == NULL) {
        cout << "Memory allocation failed.\n";
    }
    strcat_s(text[currentLineNum], LineNewLength, input);
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
            strcpy_s(temporary, bufferSize, text[line - 1]);
            temporary[index] = '\0';
            strcat_s(temporary, bufferSize, input);
            strcat_s(temporary, bufferSize, text[line - 1] + index);
            text[line - 1] = new char[bufferSize];
            strcpy_s(text[line - 1], bufferSize, temporary);
            cout << "New line is: " << text[line - 1] << endl;
        }
        else {
            cout << "This line does not exist yet or the index is wrong.\n";
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



void TextEditor::run() {
    while (true) {
        print_commands();
        get_input();
        if (strlen(input) == 1)
        {
            switch (input[0]) {
            case '1':
                append_text();
                break;
            case '2':
                start_new_line();
                break;
            case '3':
                save_to_file();
                break;
            case '4':
                load_from_file();
                break;
            case '5':
                print_to_console();
                break;
            case '6':
                insert();
                break;
            case '7':
                search();
                break;
            case '8':
                insert_with_replacement();
                break;
            case '9':
                // implemented command
                break;
            case '10':
                // implemented command
                break;
            case '11':
                // implemented command
                break;
            case '12':
                // implemented command
                break;
            case '13':
                // implemented command
                break;
            case '14':
                // implemented command
                break;
            case '15':
                system("CLS");
                cout << "Exiting...\n";
                return;
            default:
                cout << "Invalid command. Try again:\n";
            }
        }
        else {
            system("CLS");
            cout << "The command is invalid. Try again!\n";
        }
    }
}

int main() {
    TextEditor editor(256);
    editor.run();
    return 0;

}
