#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

const int tableSize = 10; // Размер таблицы (можно изменить)

// Структура для узлов цепочек
struct Node {
    string key;
    string value;
    Node* next = nullptr;
};

class Hash {
private:
    Node* table[tableSize]; // Массив указателей на узлы

    // Хеш-функция
    int hashFunction(const string& key) {
        int hash = 0;
        for (char ch : key) {
            hash += ch; // Суммируем ASCII значения символов
        }
        return hash % tableSize; // Возвращаем индекс в пределах размера таблицы
    }

public:
    // Конструктор
    Hash() {
        for (int i = 0; i < tableSize; i++) {
            table[i] = nullptr; // Инициализируем массив нулями
        }
    }

    // Деструктор для очистки памяти
    ~Hash() {
        clearH();
    }

    // Добавление элемента
    void insertH(const string& key, const string& value) {
        int index = hashFunction(key); // вычисляем индекс ключа
        Node* newNode = new Node{key, value, nullptr};

        if (!table[index]) {
            table[index] = newNode; // Если ячейка пуста, добавляем новый узел
        } else { // Если ячейка уже содержит цепочку
            Node* current = table[index]; 
            while (current) {
                if (current->key == key) {  // Ищем узел с совпадающим ключом
                    current->value = value; // Обновляем значение, если ключ уже существует
                    delete newNode; // Удаляем временный узел
                    return;
                }
                if (!current->next) { // если ключ не найден
                    current->next = newNode; // Добавляем новый узел в конец цепочки
                    return;
                }
                current = current->next;
            }
        }
    }

    // Получение значения по ключу
    string getH(const string& key) {
        int index = hashFunction(key); // Вычисляем индекс ключа
        Node* current = table[index];
        while (current) { // ищем ключ в цепочке
            if (current->key == key) {
                return current->value; // Возвращаем значение, если ключ найден
            }
            current = current->next;
        }
        return "Ключ не найден"; // Если ключ не найден
    }

    // Удаление элемента по ключу
    void removeH(const string& key) {
        int index = hashFunction(key);
        Node* current = table[index];
        Node* previous = nullptr;

        while (current) { // проходим по цепочке
            if (current->key == key) { // если находим узел
                if (previous) {
                    previous->next = current->next; // Удаляем узел из цепочки
                } else {
                    table[index] = current->next; // Если это первый узел обновляем указатель
                }
                delete current; // Освобождаем память
                return;
            }
            previous = current; // если узел в середине или конце перенастраиваем указатели соседей
            current = current->next;
        }
        cout << "Ключ не найден для удаления" << endl; // Если ключ не найден
    }

    // Вывод всех элементов хеш-таблицы
    void printTableH() {
        for (int i = 0; i < tableSize; i++) {
            if (table[i]) {
                cout << "[" << i << "]: ";
                Node* current = table[i];
                while (current) {
                    cout << "{" << current->key << ": " << current->value << "} ";
                    current = current->next;
                }
                cout << endl;
            } else {
                cout << "[" << i << "]: пусто" << endl;
            }
        }
    }

    // Запись хеш-таблицы в файл
    void writeToFileH(const string& filename) {
        ofstream file(filename);
        if (!file) {
            cout << "Не удалось открыть файл для записи." << endl;
            return;
        }

        for (int i = 0; i < tableSize; i++) {
            Node* current = table[i];
            while (current) {
                file << current->key << " " << current->value << endl;
                current = current->next;
            }
        }

        file.close();
    }

    // Чтение хеш-таблицы из файла
    void readFromFileH(const string& filename) {
        ifstream file(filename);
        if (!file) {
            cout << "Не удалось открыть файл для чтения." << endl;
            return;
        }

        clearH(); // Очищаем таблицу перед загрузкой новых данных

        string key, value;
        while (file >> key >> value) {
            insertH(key, value);
        }

        file.close();
    }

    // Очистка таблицы
    void clearH() {
        for (int i = 0; i < tableSize; i++) {
            Node* current = table[i];
            while (current) {
                Node* temp = current;
                current = current->next;
                delete temp; // Освобождаем память
            }
            table[i] = nullptr; // Обнуляем указатели
        }
    }
};

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

    Hash hashTable;

    // Загрузка хеш-таблицы из файла
    hashTable.readFromFileH(filename);

    // Выполнение команды
    if (query.find("insert") == 0) {
        size_t spacePos = query.find(" ");
        if (spacePos == string::npos || query.find(" ", spacePos + 1) == string::npos) {
            cerr << "Ошибка: Неправильный формат команды insert." << endl;
            return 1;
        }
        string key = query.substr(7, query.find(" ", 7) - 7);
        string value = query.substr(query.find(" ", 7) + 1);
        hashTable.insertH(key, value);
    } else if (query.find("remove") == 0) {
        string key = query.substr(7); // Получаем ключ после "remove "
        hashTable.removeH(key);
    } else if (query.find("get") == 0) {
        string key = query.substr(4); // Получаем ключ после "get "
        cout << hashTable.getH(key) << endl;
    } else if (query == "print") {
        hashTable.printTableH();
    } else {
        cerr << "Неизвестная команда: " << query << endl;
    }

    // Сохранение изменений в файл
    hashTable.writeToFileH(filename);

    return 0;
}
