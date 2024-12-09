#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Структура узла стека
struct NodeS {
    string data;
    NodeS* next;
};

// Структура стека
struct Stack {
    NodeS* head = nullptr; // Указатель на вершину стека
};

// Добавление элемента в стек (push)
void pushS(Stack* stack, const string& data) {
    NodeS* newNode = new NodeS{data, stack->head};
    stack->head = newNode;
}

// Удаление элемента из стека (pop)
string popS(Stack* stack) {
    if (stack->head == nullptr) {
        cout << "Стек пуст!" << endl;
        return ""; // Возвращаем пустую строку, если стек пуст
    }

    NodeS* temp = stack->head;
    string data = temp->data; // Сохраняем данные удаляемого элемента
    stack->head = stack->head->next; // Обновляем голову стека
    delete temp; // Освобождаем память
    return data; // Возвращаем данные
}

// Функция для вывода всех элементов стека
void printStackS(Stack* stack) {
    if (stack->head == nullptr) {
        cout << "Стек пуст!" << endl;
        return;
    }

    NodeS* current = stack->head;

    // Вывод элементов от верхнего к нижнему
    while (current != nullptr) {
        cout << current->data << " ";
        current = current->next;
    }
    cout << endl;
}

// Освобождение памяти стека
void clearStack(Stack* stack) {
    while (stack->head) {
        popS(stack);
    }
}

// Запись стека в файл
void writeToFileS(const Stack& stack, const string& filename) {
    ofstream file(filename);
    if (!file) {
        cout << "Не удалось открыть файл для записи." << endl;
        return;
    }

    NodeS* current = stack.head;
    while (current) {
        file << current->data << endl; // Записываем данные строки в файл
        current = current->next;
    }
    file.close();
}

// Чтение стека из файла
void readFromFileS(Stack* stack, const string& filename) {
    ifstream file(filename);
    if (!file) {
        cout << "Не удалось открыть файл для чтения." << endl;
        return;
    }

    clearStack(stack); // Очищаем стек перед загрузкой новых данных

    string line;
    while (getline(file, line)) {
        pushS(stack, line); // Добавляем строки из файла в стек
    }
    file.close();
}

// --- Основная программа ---
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

    Stack stack;

    // Загрузка стека из файла
    readFromFileS(&stack, filename);

    // Выполнение команды
    if (query.find("push") == 0) {
        string data = query.substr(5); // Получаем данные после "push "
        pushS(&stack, data);
    } else if (query == "pop") {
        cout << "Извлечено: " << popS(&stack) << endl;
    } else if (query == "print") {
        printStackS(&stack);
    } else {
        cerr << "Неизвестная команда: " << query << endl;
    }

    // Сохранение изменений в файл
    writeToFileS(stack, filename);

    // Очистка памяти
    clearStack(&stack);

    return 0;
}
