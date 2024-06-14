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
    void print_commands(); //works correctly
    void get_input(); // works correctly
    void append_text(); // works correctly
    void start_new_line(); // works correctly
    void save_to_file(); // works correctly
    void load_from_file(); // works correctly but can be optimised
    void print_to_console(); // works correctly
    void search(); // works correctly
    void insert(); // works correctly
    void insert_with_replacement(); // works when the text is inserted in the middle of the line, but falls when I try to insert more text then the capacity of the line
    void delete_command(); // seems to be correct
    void cut(); // works correctly
    void paste(); // works correctly
    void copy(); // works correctly
    void save_state(); // works correctly
    void undo(); // works correctly
    void redo(); // works correctly

    int bufferSize;
    int numberOfRows;
    int currentLineNum;
    int counter;
    char* input;
    char* cut_copy_paste;
    char** text;
    char* numberOfRowsHistory;
    int cut_copy_paste_index;
    char*** history;
    int historyIndex;
};
TextEditor::TextEditor(int BufSize)
    :bufferSize(BufSize), numberOfRows(1), currentLineNum(0), counter(0), cut_copy_paste_index(0), historyIndex(0){
    try {
        input = new char[bufferSize];
        cut_copy_paste = new char[bufferSize];
        text = new char* [numberOfRows];
        text[0] = new char[bufferSize];
        numberOfRowsHistory = new char[4];
        input[0] = '\0';
        text[0][0] = '\0';

        history = new char** [4]; 
        for (int i = 0; i < 4; ++i) {
            history[i] = nullptr;
        }
    }
    catch (const bad_alloc& e) {
        cerr << "Memory allocation failed: " << e.what() << ". Exiting..." << endl;
        exit(1);
    }
}
TextEditor::~TextEditor() {
    for (int i = 0; i < numberOfRows; i++) {
        delete[] text[i];
    }
    delete[] input;
    delete[] text;
    delete[] cut_copy_paste;
    for (int i = 0; i < 3; ++i) {
        if (history[i] != nullptr) {
            //for (int j = 0; j < numberOfRows; ++j) {
            //    delete[] history[i][j];
            //}
            delete[] history[i];
        }
    }
    delete[] history;
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
        << "10 - Redo\n"
        << "11 - Cut\n"
        << "12 - Paste\n"
        << "13 - Copy\n"
        << "14 - Insert with replacement\n"
        << "15 - Exit\n";
}

void TextEditor::get_input() {
    fgets(input, bufferSize, stdin);
    input[strcspn(input, "\n")] = '\0';

}


void TextEditor::append_text() {
    system("CLS");
    int LineNewLength = 0;
    cout << "Enter text to append:\n";
    get_input();

    LineNewLength = strlen(input) + strlen(text[currentLineNum]) + 1;

    try {
        char* temporary = new char[LineNewLength];
        strcpy_s(temporary, LineNewLength, text[currentLineNum]); 
        strcat_s(temporary, LineNewLength, input); 
        delete[] text[currentLineNum];
        text[currentLineNum] = temporary;
    }
    catch (const bad_alloc& e) {
        cerr << "Memory reallocation failed: " << e.what() << ". Exiting..." << endl;
        exit(1);
    }
    save_state();
}

void TextEditor::start_new_line() { 
    system("CLS");
    cout << "New line is started\n";
    numberOfRows++;
    currentLineNum++;
    try {
        char** newText = new char*[numberOfRows];
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
    save_state();
}

void TextEditor::save_to_file(){
    system("CLS");
    cout << "Enter the file name for saving: \n";
    get_input();

    FILE* file;

    fopen_s(&file, input, "w");
    if (file != NULL) {
        for (int i = 0; i < numberOfRows; i++) {
            fputs(text[i], file);
            fputs("\n", file);
        }
    }
    fclose(file);
    file = NULL;
}

void TextEditor::load_from_file() {
    system("CLS");
    cout << "Enter the file name for loading:\n";
    get_input();

    FILE* file;

   
    if (fopen_s(&file, input, "r") != 0 || file == NULL) {
        cout << "Error opening file!";
        return;
    }

    char* temporary = new char[bufferSize];

    while (fgets(temporary, bufferSize, file))
    {
        numberOfRows++;
        currentLineNum++;

        temporary[strcspn(temporary, "\n")] = '\0';

        try {
            char** newText = new char* [numberOfRows];
            int size = 0;
            for (int i = 0; i < (currentLineNum); i++) {
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
            memcpy(newText[currentLineNum], temporary, bufferSize);
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

void TextEditor::print_to_console() {
    system("CLS");
    cout << "The current text is:\n";
    for (int i = 0; i < numberOfRows; i++) {
        
            cout << text[i] << endl;
        }
}

void TextEditor::search() {
    system("CLS");
    cout << "Enter text to search:\n";
    get_input();

    int matchesNum = 0;
    int matchBool = 0;
    int substringLen = 0;

    substringLen = strlen(input);

    for (int i = 0; i < numberOfRows; i++)
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
                cout << "Text is present at this position:" << i << " " << j << endl;
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

        if (line <= numberOfRows && line >= 0 && index >= 0) {
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
    save_state();
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
        if (line <= numberOfRows && line >= 0 && index >= 0) {
            cout << "Enter text to insert:\n";
            get_input();
        }   
    }
    for (int i = 0; i < strlen(input); i++) 
    {
        text[line][index + i] = input[i];
    }
    save_state();
}

void TextEditor::delete_command() {
    system("CLS");
    int line, index, num = 0;
    cout << "Choose line, index and number of symbols:\n";
    if ((scanf_s("%d %d %d", &line, &index, &num)) == 3) {
        while (getchar() != '\n') {
            continue;
        }
        if (line <= numberOfRows && line >= 0 && index >= 0 && num <= strlen(text[line]) - index) {
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
            cout << "The entered line, index or number of symbols are incorrect.\n";
        }
    }
    else {
        cout << "The entered line, index or number of symbols are incorrect.\n";
    }
    save_state();
}

void TextEditor::cut() {
    system("CLS");
    int line, index, num = 0;
    cout << "Choose line, index and number of symbols:\n";
    if ((scanf_s("%d %d %d", &line, &index, &num)) == 3) {
        while (getchar() != '\n') {
            continue;
        }

        if (line <= numberOfRows && line >= 0 && index >= 0 && strlen(text[line]) >= num) {
            // I created the counter to know where the end of the string is
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
            cout << "The entered line, index or number of symbols are incorrect.\n";
        }
    }
    else {
        cout << "The entered line, index or number of symbols are incorrect.\n";
    }
    save_state();
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
        if (line <= numberOfRows && line >= 0 && index >= 0 && cut_copy_paste != NULL && index <= strlen(text[line])) {
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
    save_state();
}

void TextEditor::copy() {
    int line, index, num = 0;
    system("CLS");
    cout << "Choose line, index and number of symbols:\n";
    if ((scanf_s("%d %d %d", &line, &index, &num)) == 3) {
        while (getchar() != '\n') {
            continue;
        }
        if (line <= numberOfRows && line >= 0 && index >= 0 && num <= strlen(text[line]) - index)
        {
            while (cut_copy_paste_index != num) {
                cut_copy_paste[cut_copy_paste_index] = text[line][index + cut_copy_paste_index];
                cut_copy_paste_index++;
            }
            cut_copy_paste[cut_copy_paste_index] = '\0';
            cut_copy_paste_index = 0;
        }
        else {
            cout << "The entered line, index or number of symbols are incorrect.\n";
        }
    }
    else {
        cout << "The entered line, index or number of symbols are incorrect.\n";
    }
}

void TextEditor::save_state() {
    if (historyIndex == 4) {
        delete[] history[0];

        history[0] = history[1];
        history[1] = history[2];
        history[2] = history[3];
        history[3] = nullptr;

        historyIndex = 3;
    }
    history[historyIndex] = new char*[numberOfRows];
    for (int i = 0; i < numberOfRows; i++) {
        history[historyIndex][i] = new char[bufferSize];
        strcpy_s(history[historyIndex][i], bufferSize, text[i]);
    }
    numberOfRowsHistory[historyIndex] = numberOfRows;
    historyIndex++;
}

void TextEditor::undo() { 
    system("CLS");
    if (historyIndex > 0) {
        --historyIndex;
        for (int i = 0; i < numberOfRows; ++i) {
            delete[] text[i];
        }
        delete[] text;

        numberOfRows = numberOfRowsHistory[historyIndex - 1]; //changing the current number of rows to the new number
        currentLineNum = numberOfRows - 1;
        text = new char* [numberOfRows];
        for (int i = 0; i < numberOfRows; ++i) {
            text[i] = new char[bufferSize];
            strcpy_s(text[i], bufferSize, history[historyIndex-1][i]);
        }
    }
}

void TextEditor::redo() {
    system("CLS");
    if (historyIndex < 4 && history[historyIndex + 1] != nullptr) {
        for (int i = 0; i < numberOfRows; ++i) {
            delete[] text[i];
        }
        delete[] text;

        numberOfRows = numberOfRowsHistory[historyIndex];
        currentLineNum = numberOfRows - 1;
        text = new char* [numberOfRows];
        for (int i = 0; i < numberOfRows; ++i) {
            text[i] = new char[bufferSize];
            strcpy_s(text[i], bufferSize, history[historyIndex][i]);
        }
    }
    historyIndex++;
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
                undo();
                break;
            case 10:
                redo();
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
