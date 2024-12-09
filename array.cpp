#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// Структура для массива
struct Array {
    int* data;       // Указатель на значение
    int size;        // Текущий размер массива
    int capacity;    // Максимальная вместимость массива
};

// Функции для работы с массивом
void Ainit(Array& arr, int cap) {
    arr.data = new int[cap]; // выделяется память
    arr.size = 0; // текущий размер 0
    arr.capacity = cap; // устанавливается емкость
}

// Освобождение памяти
void Adestroy(Array& arr) {
    delete[] arr.data;
    arr.data = nullptr;
    arr.size = 0;
    arr.capacity = 0;
}

// Добавление элемента в конец массива
void Aappend(Array& arr, int value) {
    if (arr.size >= arr.capacity) { // проверка на заполненность
        arr.capacity *= 2; // увеличение емкости массива
        int* newData = new int[arr.capacity]; // создается новый массив
        for (int i = 0; i < arr.size; ++i) { // и туда копируются данные из старого
            newData[i] = arr.data[i];
        }
        delete[] arr.data;
        arr.data = newData; // перезначаем указатель на новый массив
    }
    arr.data[arr.size++] = value; // добавляем новый элемент по индексу
}

// Вставка элемента по индексу
void Ainsert(Array& arr, int index, int value) {
    if (index < 0 || index > arr.size) { // проверка допустимости индекса
        cerr << "Индекс за пределами допустимого" << endl;
        return;
    }
    if (arr.size >= arr.capacity) { // увеличение емкости при необходимости
        arr.capacity *= 2;
        int* newData = new int[arr.capacity];
        for (int i = 0; i < arr.size; ++i) {
            newData[i] = arr.data[i];
        }
        delete[] arr.data;
        arr.data = newData;
    }
    for (int i = arr.size; i > index; --i) { // сдвиг элементов вправо
        arr.data[i] = arr.data[i - 1]; // копируем элементы из позиции i-1 в позицию i
    }
    arr.data[index] = value; // вставка по индексу
    arr.size++; // увеличение размера
}

int Aget(const Array& arr, int index) {
    if (index < 0 || index >= arr.size) {
        cerr << "Индекс за пределами допустимого" << endl;
        return -1;
    }
    return arr.data[index];
}

void Aremove(Array& arr, int index) {
    if (index < 0 || index >= arr.size) {
        cerr << "Индекс за пределами допустимого" << endl;
        return;
    }
    for (int i = index; i < arr.size - 1; ++i) { // сдвиг элементов влево
        arr.data[i] = arr.data[i + 1];
    }
    arr.size--; // уменьшаем размер
}

void Areplace(Array& arr, int index, int value) {
    if (index < 0 || index >= arr.size) {
        cerr << "Индекс за пределами допустимого" << endl;
        return;
    }
    arr.data[index] = value;
}

int Alength(const Array& arr) {
    return arr.size;
}

void printArray(const Array& arr) {
    for (int i = 0; i < arr.size; ++i) {
        cout << arr.data[i] << " ";
    }
    cout << endl;
}

void writeToFileArray(const Array& arr, const string& filename) {
    ofstream file(filename);
    if (!file) {
        cerr << "Не удалось открыть файл для записи." << endl;
        return;
    }
    for (int i = 0; i < arr.size; ++i) {
        file << arr.data[i] << endl;
    }
    file.close();
    cout << "Данные успешно записаны в файл: " << filename << endl;
}

void loadFromFileArray(Array& arr, const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Не удалось открыть файл для чтения." << endl;
        return;
    }
    int value;
    while (file >> value) {
        Aappend(arr, value);
    }
    file.close();
    cout << "Данные успешно загружены из файла: " << filename << endl;
}

// Выполнение команды
void executeCommand(Array& arr, const string& query) {
    stringstream ss(query);
    string command;
    ss >> command;

    if (command == "append") {
        int value;
        ss >> value;
        Aappend(arr, value);
    } else if (command == "ainsert") {
        int index, value;
        ss >> index >> value;
        Ainsert(arr, index, value);
    } else if (command == "aget") {
        int index;
        ss >> index;
        cout << "Элемент по индексу " << index << ": " << Aget(arr, index) << endl;
    } else if (command == "aremov") {
        int index;
        ss >> index;
        Aremove(arr, index);
    } else if (command == "areplace") {
        int index, value;
        ss >> index >> value;
        Areplace(arr, index, value);
    } else if (command == "print") {
        printArray(arr);
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

    // Работа с массивом
    Array arr;
    Ainit(arr, 10);

    // Загрузка из файла
    loadFromFileArray(arr, filename);

    // Выполнение команды
    executeCommand(arr, query);

    // Сохранение в файл
    writeToFileArray(arr, filename);

    // Очистка памяти
    Adestroy(arr);

    return 0;
}