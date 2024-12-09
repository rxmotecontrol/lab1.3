#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>

using namespace std;

// Структура узла AVL-дерева
struct AVLNode {
    int data = 0;
    AVLNode* left = nullptr;
    AVLNode* right = nullptr;
    int balance = 0;
};

// --- Основные функции работы с AVL-деревом ---

// Вычисление высоты узла
int HeightAVL(AVLNode* root) {
    if (root == nullptr) return 0;
    return max(HeightAVL(root->left), HeightAVL(root->right)) + 1;
}

// Установка баланса узла
void SetBalanceAVL(AVLNode* root) {
    if (root != nullptr) {
        root->balance = HeightAVL(root->left) - HeightAVL(root->right);
    }
}

// Правый поворот
void TurnRightAVL(AVLNode*& root) {
    AVLNode* leftSubtree = root->left;
    AVLNode* leftSubtreeRightSubtree = leftSubtree->right;

    leftSubtree->right = root;
    root->left = leftSubtreeRightSubtree;

    SetBalanceAVL(root);
    SetBalanceAVL(leftSubtree);

    root = leftSubtree;
}

// Левый поворот
void TurnLeftAVL(AVLNode*& root) {
    AVLNode* rightSubtree = root->right;
    AVLNode* rightSubtreeLeftSubtree = rightSubtree->left;

    rightSubtree->left = root;
    root->right = rightSubtreeLeftSubtree;

    SetBalanceAVL(root);
    SetBalanceAVL(rightSubtree);

    root = rightSubtree;
}

// Вставка узла
void InsertAVL(AVLNode*& root, int key) {
    if (root == nullptr) {
        root = new AVLNode();
        root->data = key;
        return;
    }

    if (key < root->data) {
        InsertAVL(root->left, key);
    } else if (key > root->data) {
        InsertAVL(root->right, key);
    } else {
        return;
    }

    SetBalanceAVL(root);

    if (root->balance > 1) {
        if (key < root->left->data) {
            TurnRightAVL(root);
        } else {
            TurnLeftAVL(root->left);
            TurnRightAVL(root);
        }
    }

    if (root->balance < -1) {
        if (key > root->right->data) {
            TurnLeftAVL(root);
        } else {
            TurnRightAVL(root->right);
            TurnLeftAVL(root);
        }
    }
}

// Удаление узла
AVLNode* MinValueAVLNode(AVLNode* root) {
    AVLNode* current = root;
    while (current && current->left) {
        current = current->left;
    }
    return current;
}

void DeleteAVLNode(AVLNode*& root, int key) {
    if (!root) return;

    if (key < root->data) {
        DeleteAVLNode(root->left, key);
    } else if (key > root->data) {
        DeleteAVLNode(root->right, key);
    } else {
        if (!root->left || !root->right) {
            AVLNode* temp = root->left ? root->left : root->right;
            if (!temp) {
                temp = root;
                root = nullptr;
            } else {
                *root = *temp;
            }
            delete temp;
        } else {
            AVLNode* temp = MinValueAVLNode(root->right);
            root->data = temp->data;
            DeleteAVLNode(root->right, temp->data);
        }
    }

    if (!root) return;

    SetBalanceAVL(root);

    if (root->balance > 1) {
        if (root->left && root->left->balance >= 0) {
            TurnRightAVL(root);
        } else {
            TurnLeftAVL(root->left);
            TurnRightAVL(root);
        }
    }

    if (root->balance < -1) {
        if (root->right && root->right->balance <= 0) {
            TurnLeftAVL(root);
        } else {
            TurnRightAVL(root->right);
            TurnLeftAVL(root);
        }
    }
}

// Симметричный обход дерева
void InOrderAVL(AVLNode* root) {
    if (root) {
        InOrderAVL(root->left);
        cout << root->data << " ";
        InOrderAVL(root->right);
    }
}

// --- Запись и чтение дерева ---

void writeNodeToFile(AVLNode* node, ofstream& file, int depth) {
    if (!node) return;

    writeNodeToFile(node->right, file, depth + 1);

    for (int i = 0; i < depth; i++) {
        file << "    ";
    }
    file << node->data << endl;

    writeNodeToFile(node->left, file, depth + 1);
}

void writeToFileAVL(AVLNode* root, const string& filename) {
    ofstream file(filename);
    if (!file) {
        cerr << "Ошибка: Не удалось открыть файл для записи." << endl;
        return;
    }
    if (!root) {
        file << "Дерево пусто." << endl;
    } else {
        writeNodeToFile(root, file, 0);
    }
    file.close();
}

void readFromFileAVL(AVLNode*& root, const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Ошибка: Не удалось открыть файл для чтения." << endl;
        return;
    }
    int value;
    while (file >> value) {
        InsertAVL(root, value);
    }
    file.close();
}

// --- Очистка дерева ---
void clearAVL(AVLNode*& root) {
    if (root) {
        clearAVL(root->left);
        clearAVL(root->right);
        delete root;
        root = nullptr;
    }
}

// --- Основная программа ---
int main(int argc, char* argv[]) {
    if (argc < 5) {
        cerr << "Использование: ./dbms --file <filename> --query '<command>'" << endl;
        return 1;
    }

    string filename, query;

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

    AVLNode* root = nullptr;

    readFromFileAVL(root, filename);

    if (query.find("insert") == 0) {
        int value = stoi(query.substr(7));
        InsertAVL(root, value);
    } else if (query.find("delete") == 0) {
        int value = stoi(query.substr(7));
        DeleteAVLNode(root, value);
    } else if (query == "print") {
        InOrderAVL(root);
        cout << endl;
    } else if (query == "clear") {
        clearAVL(root);
    } else if (query == "height") {
        cout << "Высота дерева: " << HeightAVL(root) << endl;
    } else {
        cerr << "Неизвестная команда: " << query << endl;
    }

    writeToFileAVL(root, filename);
    clearAVL(root);

    return 0;
}
