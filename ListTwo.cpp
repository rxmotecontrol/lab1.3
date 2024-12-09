#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

struct NodeLO {
    int data;
    NodeLO* next;
};

// --- Функции для работы со списком ---

// Добавление элемента в начало
void addHeadLO(NodeLO*& head, int value) {
    NodeLO* newNode = new NodeLO{value, head};
    head = newNode;
}

// Добавление элемента в конец
void addTailLO(NodeLO*& head, int value) {
    NodeLO* newNode = new NodeLO{value, nullptr};
    if (!head) {
        head = newNode;
        return;
    }
    NodeLO* temp = head;
    while (temp->next) temp = temp->next;
    temp->next = newNode;
}

// Удаление элемента с головы
void deleteHeadLO(NodeLO*& head) {
    if (!head) return;
    NodeLO* temp = head;
    head = head->next;
    delete temp;
}

// Удаление элемента с хвоста
void deleteTailLO(NodeLO*& head) {
    if (!head) return;
    if (!head->next) {
        delete head;
        head = nullptr;
        return;
    }
    NodeLO* temp = head;
    while (temp->next->next) temp = temp->next;
    delete temp->next;
    temp->next = nullptr;
}

// Удаление элемента по значению
bool deleteByValueLO(NodeLO*& head, int value) {
    if (!head) return false;
    if (head->data == value) {
        deleteHeadLO(head);
        return true;
    }
    NodeLO* temp = head;
    while (temp->next && temp->next->data != value) temp = temp->next;
    if (temp->next) {
        NodeLO* toDelete = temp->next;
        temp->next = temp->next->next;
        delete toDelete;
        return true;
    }
    return false;
}

// Поиск элемента
NodeLO* searchLO(NodeLO* head, int value) {
    while (head) {
        if (head->data == value) return head;
        head = head->next;
    }
    return nullptr;
}

// Печать списка
void printListLO(NodeLO* head) {
    if (!head) {
        cout << "Список пуст.\n";
        return;
    }
    while (head) {
        cout << head->data << " ";
        head = head->next;
    }
    cout << endl;
}

// Очистка списка
void clearListLO(NodeLO*& head) {
    while (head) deleteHeadLO(head);
}

// Запись списка в файл
void writeToFileLO(NodeLO* head, const string& filename) {
    ofstream file(filename);
    if (!file) {
        cerr << "Ошибка: Не удалось открыть файл для записи: " << filename << endl;
        return;
    }
    while (head) {
        file << head->data << endl;
        head = head->next;
    }
    file.close();
}

// Чтение списка из файла
void readFromFileLO(NodeLO*& head, const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Ошибка: Не удалось открыть файл для чтения: " << filename << endl;
        return;
    }
    clearListLO(head);
    int value;
    while (file >> value) addTailLO(head, value);
    file.close();
}

// --- Выполнение команды ---
void executeCommand(NodeLO*& head, const string& command) {
    stringstream ss(command);
    string action;
    ss >> action;

    if (action == "addhead") {
        int value;
        ss >> value;
        addHeadLO(head, value);
    } else if (action == "addtail") {
        int value;
        ss >> value;
        addTailLO(head, value);
    } else if (action == "deletehead") {
        deleteHeadLO(head);
    } else if (action == "deletetail") {
        deleteTailLO(head);
    } else if (action == "deletebyvalue") {
        int value;
        ss >> value;
        if (!deleteByValueLO(head, value)) {
            cout << "Элемент с данным значением не найден.\n";
        }
    } else if (action == "search") {
        int value;
        ss >> value;
        NodeLO* result = searchLO(head, value);
        cout << (result ? "Элемент найден: " + to_string(result->data) : "Элемент не найден.") << endl;
    } else if (action == "print") {
        printListLO(head);
    } else {
        cerr << "Неизвестная команда: " << action << endl;
    }
}

// --- Основная функция ---
int main(int argc, char* argv[]) {
    if (argc < 5) {
        cerr << "Использование: ./program --file <filename> --query '<command>'" << endl;
        return 1;
    }

    string filename, query;

    // Парсинг аргументов
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) {
            filename = argv[++i];
        } else if (arg == "--query" && i + 1 < argc) {
            query = argv[++i];
        }
    }

    if (filename.empty() || query.empty()) {
        cerr << "Ошибка: Недостаточно аргументов. Проверьте --file и --query.\n";
        return 1;
    }

    NodeLO* head = nullptr;

    // Загрузка списка из файла
    readFromFileLO(head, filename);

    // Выполнение команды
    executeCommand(head, query);

    // Сохранение изменений в файл
    writeToFileLO(head, filename);

    // Очистка памяти
    clearListLO(head);

    return 0;
}
