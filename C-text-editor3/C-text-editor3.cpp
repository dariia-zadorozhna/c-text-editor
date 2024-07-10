#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <windows.h>
#include <sys/stat.h>
using namespace std;


class CaesarCipher {
    typedef char* (*EncryptFunction)(char* text, int key);
    typedef char* (*DecryptFunction)(char* text, int key);
private:
    HINSTANCE hinstanceLibrary;
    EncryptFunction encryptFunction;
    DecryptFunction decryptFunction;
public:
    CaesarCipher(LPCWSTR path);
    ~CaesarCipher();
    void encrypt(char* text, int key);
    void decrypt(char* text, int key);
    char* message;
    LPCWSTR dllPath;
};

CaesarCipher::CaesarCipher(LPCWSTR path) : dllPath(path) {
    message = new char[256];
    hinstanceLibrary = LoadLibrary(dllPath);
    if (hinstanceLibrary == nullptr) {
        delete[] message;
        throw std::runtime_error("Failed to load the DLL.");
    }
    encryptFunction = (EncryptFunction)GetProcAddress(hinstanceLibrary, "encrypt");
    decryptFunction = (DecryptFunction)GetProcAddress(hinstanceLibrary, "decrypt");
    if (!encryptFunction || !decryptFunction) {
        hinstanceLibrary = nullptr;
        delete[] message;
        throw std::runtime_error("Failed to locate the functions in the DLL.");
    }
};
CaesarCipher::~CaesarCipher() {
    if (hinstanceLibrary) {
        FreeLibrary(hinstanceLibrary);
    }
};
void CaesarCipher::encrypt(char* text, int key) {
    if (encryptFunction) {
        message = encryptFunction(text, key);
        cout << "Encrypted message:" << message << endl;
    }
    else {
        cerr << "Encrypt function not available." << endl;
    }
}
void CaesarCipher::decrypt(char* text, int key) {
    if (decryptFunction) {
        message = decryptFunction(text, key);
        cout << "Decrypted message:" << message << endl;
    }
    else {
        cerr << "Decrypt function not available." << endl;
    }
}

class Text {
public:
    Text();
    ~Text();
    void appendText(char* input);
    void startNewLine();
    void saveToFile(char* input);
    void loadFromFile(char* input);
    void printToConsole();
    void search(char* input);
    void insert(int line, int index, char* input);
    void insertWithReplacement(int line, int index, char* input);
    void deleteCommand(bool trueOrFalse, int line, int index, int num);
    void cut(bool trueOrFalse, int line, int index, int num);
    void paste(bool trueOrFalse, int line, int index);
    void copy(bool trueOrFalse, int num, int line, int index);
    void save_state();
    void undo();
    void redo();

private:

    int bufferSize;
    int numberOfRows;
    int currentLineNum;
    int counter;
    char* cut_copy_paste;
    char** text;
    char* numberOfRowsHistory;
    int cut_copy_paste_index;
    char*** history;
    int historyIndex;
    FILE* file;
    FILE* inputFilePTR;
    FILE* outputFilePTR;
};
Text::Text()
    :bufferSize(256), numberOfRows(1), currentLineNum(0), counter(0), cut_copy_paste_index(0), historyIndex(0) {
    try {
        cut_copy_paste = new char[bufferSize];
        text = new char* [numberOfRows];
        text[0] = new char[bufferSize];
        numberOfRowsHistory = new char[4];
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
Text::~Text() {
    for (int i = 0; i < numberOfRows; i++) {
        delete[] text[i];
    }
    delete[] text;
    delete[] cut_copy_paste;
    for (int i = 0; i < 3; ++i) {
        if (history[i] != nullptr) {
            for (int j = 0; j < numberOfRowsHistory[i]; ++j) {
                delete[] history[i][j];
            }
            delete[] history[i];
        }
    }
    delete[] history;
}
void Text::appendText(char* input) {

    int LineNewLength = 0;
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

void Text::startNewLine() {
    numberOfRows++;
    currentLineNum++;
    try {
        char** newText = new char* [numberOfRows];
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

void Text::saveToFile(char* input) {
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

void Text::loadFromFile(char* input) {
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

void Text::printToConsole() {
    for (int i = 0; i < numberOfRows; i++) {

        cout << text[i] << endl;
    }
}

void Text::search(char* input) {
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

void Text::insert(int line, int index, char* input) {
    char* temporary = new char[bufferSize];

    strcpy_s(temporary, bufferSize, text[line]);
    temporary[index] = '\0';
    strcat_s(temporary, bufferSize, input);
    strcat_s(temporary, bufferSize, text[line] + index);
    text[line] = new char[bufferSize];
    strcpy_s(text[line], bufferSize, temporary);
    cout << "New line is: " << text[line] << endl;

    save_state();
}

void Text::insertWithReplacement(int line, int index, char* input) {
    if (strlen(text[line]) >= index + strlen(input)) {
        for (int i = 0; i < strlen(input); i++)
        {
            text[line][index + i] = input[i];
        }
    }
    else {
        cout << "The input text is too large for this line:(\n";
    }

    save_state();
}

void Text::deleteCommand(bool trueOrFalse, int line, int index, int num) {
    if (trueOrFalse == true) {
        while (text[line][counter] != '\0') {
            counter++;
        }
        while (index != counter) {
            text[line][index] = text[line][index + num];
            index++;
        }
        counter = 0;
        save_state();
    }
}

void Text::cut(bool trueOrFalse, int line, int index, int num) {
    if (trueOrFalse == true) {
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
        save_state();
    }
}

void Text::paste(bool trueOrFalse, int line, int index) {
    char* temporary = new char[bufferSize];
    if (trueOrFalse == true) {
        strcpy_s(temporary, bufferSize, text[line]);
        temporary[index] = '\0';
        strcat_s(temporary, bufferSize, cut_copy_paste);
        strcat_s(temporary, bufferSize, text[line] + index);
        delete[] text[line];
        text[line] = new char[bufferSize];
        strcpy_s(text[line], bufferSize, temporary);
        cout << "New line is: " << text[line] << endl;
    }
    delete[] temporary;
    save_state();
}

void Text::copy(bool trueOrFalse, int num, int line, int index) {
    if (trueOrFalse == true) {
        while (cut_copy_paste_index != num) {
            cut_copy_paste[cut_copy_paste_index] = text[line][index + cut_copy_paste_index];
            cut_copy_paste_index++;
        }
        cut_copy_paste[cut_copy_paste_index] = '\0';
        cut_copy_paste_index = 0;
    }
}

void Text::save_state() {
    if (historyIndex == 4) {
        delete[] history[0];

        history[0] = history[1];
        history[1] = history[2];
        history[2] = history[3];
        history[3] = nullptr;

        historyIndex = 3;
    }
    history[historyIndex] = new char* [numberOfRows];
    for (int i = 0; i < numberOfRows; i++) {
        history[historyIndex][i] = new char[bufferSize];
        strcpy_s(history[historyIndex][i], bufferSize, text[i]);
    }
    numberOfRowsHistory[historyIndex] = numberOfRows;
    historyIndex++;
}
// no interaction with user
void Text::undo() {
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
            strcpy_s(text[i], bufferSize, history[historyIndex - 1][i]);
        }
    }
}
// no interaction with user
void Text::redo() {
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
// no interaction with user


class TextEditor
{
public:
    TextEditor();
    ~TextEditor();

    char* input;
    int line, index, num, key;
    FILE* file;
    char* inputFile;
    char* outputFile;
    char* textFromFile;
    bool trueOrFalse;

    Text mytext;

    void printCommands(); 
    void get_input(); 
    void forAppend();
    void forStartNewLine();
    void forPrintToConsole();
    void forSearch();
    void forInsert();
    void forInsertWithReplacement();
    void forDelete();
    void forCut();
    void forPaste();
    void forCopy();
    void encryptORdecrypt(const char* encryptORdecryptInput);
    void forSaveToFile();
    void forLoadFromFile();
    void run();
    
private:
    int bufferSize;
};
TextEditor::TextEditor() :bufferSize(256) {
    try {
        input = new char[bufferSize];
        input[0] = '\0';
        textFromFile = new char[bufferSize];
    }

    catch (const bad_alloc& e) {
        cerr << "Memory allocation failed: " << e.what() << ". Exiting..." << endl;
        exit(1);
    }
}
TextEditor::~TextEditor() {

    delete[] input;
    delete[] textFromFile;
}

void TextEditor::printCommands() {
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
        << "15 - Exit\n"
        << "16 - Encrypt\n"
        << "17 - Decrypt\n";
}

void TextEditor::get_input() {
    fgets(input, bufferSize, stdin);
    input[strcspn(input, "\n")] = '\0';
}

void TextEditor::forAppend() {
    system("CLS");
    cout << "Enter text to append:\n";
    get_input();
    mytext.appendText(input);
}

void TextEditor::forStartNewLine() {
    system("CLS");
    cout << "New line is started\n";
    mytext.startNewLine();
}

void TextEditor::forPrintToConsole() {
    system("CLS");
    cout << "The current text is:\n";
    mytext.printToConsole();
}

void TextEditor::forSearch() {
    system("CLS");
    cout << "Enter text to search:\n";
    get_input();
    mytext.search(input);
}

void TextEditor::forInsert() {
    system("CLS");
    cout << "Choose line and index:\n";
    if ((scanf_s("%d %d", &line, &index)) == 2) {
        while (getchar() != '\n') {
            continue;
        }

        if (line >= 0 && index >= 0) {
            cout << "Enter text to insert:\n";
            get_input();
        }
        else {
            cout << "The entered line and index are incorrect. \n";
        }
    }
    else {
        cout << "The entered line and index are incorrect.\n";
    }
    mytext.insert(line, index, input);
}

void TextEditor::forInsertWithReplacement() {
    system("CLS");
    int size = 0;
    cout << "Choose line and index:\n";
    if ((scanf_s("%d %d", &line, &index)) == 2) {
        while (getchar() != '\n') {
            continue;
        }
        if (line >= 0 && index >= 0) {
            cout << "Enter text to insert:\n";
            get_input();
        }
    }
    mytext.insertWithReplacement(line, index, input);
}

void TextEditor::forDelete() {
    system("CLS");
    cout << "Choose line, index and number of symbols:\n";
    if ((scanf_s("%d %d %d", &line, &index, &num)) == 3) {
        while (getchar() != '\n') {
            continue;
        }
        if (line >= 0 && index >= 0) {
            trueOrFalse = true;
        }
        else {
            cout << "The entered line, index or number of symbols are incorrect.\n";
            trueOrFalse = false;
        }
    }
    else {
        cout << "The entered line, index or number of symbols are incorrect.\n";
        trueOrFalse = false;
    }
    mytext.deleteCommand(trueOrFalse, line, index, num);
}

void TextEditor::forCut() {
    system("CLS");
    cout << "Choose line, index and number of symbols:\n";
    if ((scanf_s("%d %d %d", &line, &index, &num)) == 3) {
        while (getchar() != '\n') {
            continue;
        }
        if (line >= 0 && index >= 0) {
            trueOrFalse = true;
        }
        else {
            cout << "The entered line, index or number of symbols are incorrect.\n";
            trueOrFalse = false;
        }
    }
    else {
        cout << "The entered line, index or number of symbols are incorrect.\n";
        trueOrFalse = false;
    }
    mytext.cut(trueOrFalse,line,index,num);
}

void TextEditor::forPaste() {
    system("CLS");
    cout << "Choose line and index:\n";
    if ((scanf_s("%d %d", &line, &index)) == 2) {
        while (getchar() != '\n') {
            continue;
        }
        if (line >= 0 && index >= 0) {
            trueOrFalse = true;
        }
        else {
            cout << "The entered line and index are incorrect.\n";
            trueOrFalse = false;
        }
    }
    else {
        cout << "The entered line and index are incorrect.\n";
        trueOrFalse = false;
    }
    mytext.paste(trueOrFalse, line, index);
}

void TextEditor::forCopy() {
    system("CLS");
    cout << "Choose line, index and number of symbols:\n";
    if ((scanf_s("%d %d %d", &line, &index, &num)) == 3) {
        while (getchar() != '\n') {
            continue;
        }
        if (line >= 0 && index >= 0) {
            trueOrFalse = true;
        }
        else {
            cout << "The entered line, index or number of symbols are incorrect.\n";
            trueOrFalse = false;
        }
    }
    else {
        cout << "The entered line, index or number of symbols are incorrect.\n";
        trueOrFalse = false;
    }
    mytext.copy(trueOrFalse, num, line, index);
}

void TextEditor::forSaveToFile(){
    system("CLS");
    cout << "Enter the file name for saving: \n";
    get_input();
    mytext.saveToFile(input);
}

void TextEditor::forLoadFromFile() {
    system("CLS");
    cout << "Enter the file name for loading: \n";
    get_input();
    mytext.loadFromFile(input);
}

bool fileExists(const char* filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

void TextEditor::encryptORdecrypt(const char* encryptORdecryptInput) {
    system("CLS");
    cout << "Enter the input file:" << endl;
    get_input();
    inputFile = input;
    if (!fileExists(inputFile)) {
        std::cout << "Error: File does not exist!" << std::endl;
        return;
    }

    if (fopen_s(&file, inputFile, "r") != 0 || file == NULL) {
        cout << "Error opening file!" << endl;
    }

    char* temporary = new char[bufferSize];
    textFromFile[0] = '\0';

    while (fgets(temporary, bufferSize, file)) {
        temporary[strcspn(temporary, "\n")] = '\0';
        strcat_s(textFromFile, bufferSize, temporary);
    }

    fclose(file);

    cout << "Enter the key:" << endl;
    get_input();
    key = atoi(input);

    try {
        CaesarCipher cipher(L"C:\\Users\\Дарія\\source\\repos\\C_text_editor\\C-text-editor3\\CaesarDLL.dll");
        if (encryptORdecryptInput == "Encrypt") {
            cipher.encrypt(textFromFile, key);
        }
        else if (encryptORdecryptInput == "Decrypt") {
            cipher.decrypt(textFromFile, key);
        }

        cout << "Enter the output file:" << endl;
        get_input();
        outputFile = input;

        if (fopen_s(&file, outputFile, "w") != 0 || file == NULL) {
            cout << "Error opening file!" << endl;
        }
        fputs(cipher.message, file);
        fclose(file);

        delete[] temporary;
    }
    catch (const std::runtime_error& e) {
        std::wcerr << L"Runtime error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::wcerr << L"Exception: " << e.what() << std::endl;
    }
    catch (...) {
        std::wcerr << L"Unknown error occurred." << std::endl;
    }
    
}

void TextEditor::run() {
    while (true) {
        printCommands();
        get_input();
        int command = atoi(input);
        switch (command) {
        case 1:
            forAppend();
            break;
        case 2:
            forStartNewLine();
            break;
        case 3:
            forSaveToFile();
            break;
        case 4:
            forLoadFromFile();
            break;
        case 5:
            forPrintToConsole();
            break;
        case 6:
            forInsert();
            break;
        case 7:
            forSearch();
            break;
        case 8:
            forDelete();
            break;
        case 9:
            mytext.undo();
            break;
        case 10:
            mytext.redo();
            break;
        case 11:
            forCut();
            break;
        case 12:
            forPaste();
            break;
        case 13:
            forCopy();
            break;
        case 14:
            forInsertWithReplacement();
            break;
        case 15:
            system("CLS");
            cout << "Exiting...\n";
            return;
        case 16:
            encryptORdecrypt("Encrypt");
            break;
        case 17:
            encryptORdecrypt("Decrypt");
            break;
        default:
            cout << "Invalid command. Try again:\n";
        }
    }
}

int main() {
    TextEditor editor;
    editor.run();
    
}
