#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

struct NodeLO {
    int data;
    NodeLO* next;
};

// Добавление элемента в голову списка
void addHeadLO(NodeLO*& head, int value) {
    NodeLO* newNodeLO = new NodeLO; // создаем новый узел со значением value
    newNodeLO->data = value;
    newNodeLO->next = head; // указываем на head
    head = newNodeLO; // обновляем head
}

void addTailLO(NodeLO*& head, int value) {
    NodeLO* newNodeLO = new NodeLO; // создается новый узел с value
    newNodeLO->data = value;
    newNodeLO->next = nullptr; // указывает на nullptr тк он последний

    if (!head) { // если список пуст то head указывает на новый узел
        head = newNodeLO;
    } else {
        NodeLO* temp = head; 
        while (temp->next) { // иначе перебираем узлы и вставляем новый узел в конец
            temp = temp->next;
        }
        temp->next = newNodeLO;
    }
}

void deleteHeadLO(NodeLO*& head) {
    if (!head) return; // список пуст — удалять нечего
    NodeLO* temp = head; // иначе temp указывает на текущий head
    head = head->next; // обновляем head и удаляем temp
    delete temp;
}

void deleteTailLO(NodeLO*& head) {
    if (!head) return;
    if (!head->next) { // если список состоит из 1 узла удаляем head
        delete head;
        head = nullptr;
        return;
    }
    NodeLO* temp = head;
    while (temp->next->next) { // иначе находим последний элемент и устанавливаем указатель на nullptr
        temp = temp->next;
    }
    delete temp->next;
    temp->next = nullptr;
}

bool deleteByValueLO(NodeLO*& head, int value) {
    if (!head) return false; // если список пуст, вернуть false

    // проверка нужно ли удалить голову
    if (head->data == value) {
        deleteHeadLO(head); // предполагается что deleteHeadLO уже обрабатывает голову
        return true; // удаление успешно
    }

    NodeLO* current = head;
    while (current->next && current->next->data != value) {
        current = current->next;
    }

    // если значение найдено, удалить узел
    if (current->next) {
        NodeLO* temp = current->next;
        current->next = current->next->next;
        delete temp; // освобождаем память
        return true; // удаление успешно
    }

    return false; // значение не найдено
}

NodeLO* searchLO(NodeLO* head, int value) {
    NodeLO* current = head;
    while (current) { // идем по списку пока не найдем узел с value
        if (current->data == value) {
            return current; // возвращаем узел
        }
        current = current->next; 
    }
    return nullptr; // если не найдет то возвращаем nullpt
}

void printListLO(NodeLO* head) {
    while (head) {
        cout << head->data << " ";
        head = head->next;
    }
    cout << endl;
}

void clearListLO(NodeLO*& head) {
    while (head) {
        deleteHeadLO(head);
    }
}

void writeToFileLO(NodeLO* head, const string& filename) {
    ofstream file(filename);
    if (!file) {
        cout << "Не удалось открыть файл для записи.\n";
        return;
    }
    NodeLO* temp = head;
    while (temp) {
        file << temp->data << endl;
        temp = temp->next;
    }
    file.close();
}

void readFromFileLO(NodeLO*& head, const string& filename) {
    ifstream file(filename);
    if (!file) {
        cout << "Не удалось открыть файл для чтения.\n";
        return;
    }

    clearListLO(head);

    int value;
    while (file >> value) {
        addTailLO(head, value);
    }
    file.close();
}

// Выполнение команды
void executeCommand(NodeLO*& head, const string& query) {
    stringstream ss(query);
    string command;
    ss >> command;

    if (command == "addhead") {
        int value;
        ss >> value;
        addHeadLO(head, value);
    } else if (command == "addtail") {
        int value;
        ss >> value;
        addTailLO(head, value);
    } else if (command == "deletehead") {
        deleteHeadLO(head);
    } else if (command == "deletetail") {
        deleteTailLO(head);
    } else if (command == "deletebyvalue") {
        int value;
        ss >> value;
        if (!deleteByValueLO(head, value)) {
            cout << "Элемент не найден.\n";
        }
    } else if (command == "search") {
        int value;
        ss >> value;
        NodeLO* result = searchLO(head, value);
        if (result) {
            cout << "Элемент найден: " << result->data << endl;
        } else {
            cout << "Элемент не найден.\n";
        }
    } else if (command == "print") {
        printListLO(head);
    } else {
        cerr << "Неизвестная команда: " << command << endl;
    }
}

// Основная функция
int main(int argc, char* argv[]) {
    if (argc < 5) {
        cerr << "Usage: ./dbms --file <filename> --query '<command>'" << endl;
        return 1;
    }

    string filename;
    string query;

    // Разбор аргументов
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) {
            filename = argv[++i];
        } else if (arg == "--query" && i + 1 < argc) {
            query = argv[++i];
        }
    }

    if (filename.empty() || query.empty()) {
        cerr << "Ошибка: Укажите файл и запрос." << endl;
        return 1;
    }

    NodeLO* head = nullptr;

    // Загрузка данных из файла
    readFromFileLO(head, filename);

    // Выполнение команды
    executeCommand(head, query);

    // Сохранение изменений в файл
    writeToFileLO(head, filename);

    // Очистка памяти
    clearListLO(head);

    return 0;
}
