#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

struct NodeQ {
    int data;
    NodeQ* next;
};

struct Queue {
    NodeQ* head;
    NodeQ* tail;

    Queue() : head(nullptr), tail(nullptr) {}
};

// --- Функции для работы с очередью ---

// Добавление элемента в очередь (push)
void pushQ(Queue& queue, int value) {
    NodeQ* newNode = new NodeQ{value, nullptr};
    if (!queue.tail) { // Если очередь пустая
        queue.head = newNode; // новый узел и начало и конец
        queue.tail = newNode;
    } else {
        queue.tail->next = newNode; // обновляется next текущего tail
        queue.tail = newNode; // tail указывает на новый узел
    }
}

// Удаление элемента из очереди (pop)
int popQ(Queue& queue) {
    if (!queue.head) {
        cout << "Очередь пуста!" << endl;
        return -1; // Возвращаем -1, если очередь пуста
    }

    NodeQ* temp = queue.head; // head в temp
    int value = temp->data; // первый элемент в переменную value
    queue.head = queue.head->next; // Обновляется указатель head

    if (!queue.head) { // Если очередь стала пустой
        queue.tail = nullptr;
    }

    delete temp; // Освобождаем память
    return value;
}

// Чтение (вывод всех элементов очереди)
void printQueue(const Queue& queue) {
    if (!queue.head) {
        cout << "Очередь пуста.\n";
        return;
    }
    NodeQ* current = queue.head;
    while (current) {
        cout << current->data << " ";
        current = current->next;
    }
    cout << endl;
}

// Освобождение памяти
void clearQueue(Queue& queue) {
    while (queue.head) {
        popQ(queue);
    }
}

// Функция для записи очереди в файл
void writeToFileQ(const Queue& queue, const string& filename) {
    ofstream file(filename);
    if (!file) {
        cerr << "Ошибка: Не удалось открыть файл для записи: " << filename << endl;
        return;
    }
    NodeQ* temp = queue.head;
    while (temp) {
        file << temp->data << endl;
        temp = temp->next;
    }
    file.close();
}

// Функция для чтения очереди из файла
void readFromFileQ(Queue& queue, const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Ошибка: Не удалось открыть файл для чтения: " << filename << endl;
        return;
    }

    clearQueue(queue); // Очищаем очередь перед загрузкой новых данных

    int value;
    while (file >> value) {
        pushQ(queue, value);
    }
    file.close();
}

// --- Выполнение команды ---
void executeCommand(Queue& queue, const string& command) {
    stringstream ss(command);
    string action;
    ss >> action;

    if (action == "push") {
        int value;
        ss >> value;
        pushQ(queue, value);
    } else if (action == "pop") {
        int poppedValue = popQ(queue);
        if (poppedValue != -1) {
            cout << "Удалён элемент: " << poppedValue << endl;
        }
    } else if (action == "print") {
        printQueue(queue);
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
        cerr << "Ошибка: Укажите файл и запрос." << endl;
        return 1;
    }

    Queue queue;

    // Загрузка очереди из файла
    readFromFileQ(queue, filename);

    // Выполнение команды
    executeCommand(queue, query);

    // Сохранение изменений в файл
    writeToFileQ(queue, filename);

    // Очистка памяти
    clearQueue(queue);

    return 0;
}
