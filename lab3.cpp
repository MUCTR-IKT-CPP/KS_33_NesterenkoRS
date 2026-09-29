#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <Windows.h>

using namespace std;


enum CharacterClass {
    WARRIOR,
    MAGE,
    ROGUE,
    CLERIC
};


struct GameCharacter {
    string name;
    CharacterClass character_class;
    int level;
    int health;
    int mana;
    string* p_inventory; 
    int inventory_size;  
};


GameCharacter* createCharacters(int n);
void generateCharactersData(GameCharacter* p_characters, int n);
void deleteCharacters(GameCharacter* p_characters, int n);
void printCharacters(GameCharacter* p_characters, int n);
string classToString(CharacterClass char_class);
CharacterClass intToClass(int choice);
void searchByItem(GameCharacter* p_characters, int n, string target_item);
void findMinMaxLevelByClass(GameCharacter* p_characters, int n, CharacterClass target_class);
void analyzeInventory(GameCharacter* p_characters, int n);
void sortCharacters(GameCharacter* p_characters, int n);
void equipItemToClass(GameCharacter* p_characters, int n, CharacterClass target_class, string new_item);
void runMenu(GameCharacter* p_characters, int n);


int main() {
    SetConsoleOutputCP(CP_UTF8);
    
    int n;
    cout << "Введите количество персонажей (N): ";
    cin >> n;

    if (n <= 0) {
        cout << "Количество должно быть больше 0!" << endl;
        return 1;
    }


    GameCharacter* p_characters = createCharacters(n);
    generateCharactersData(p_characters, n);

    cout << "\n--- Сгенерированная база данных ---" << endl;
    printCharacters(p_characters, n);


    runMenu(p_characters, n);


    deleteCharacters(p_characters, n);

    return 0;
}

/**
 * Выделяет динамическую память под массив персонажей.
 *
 * @param n количество персонажей.
 * @return указатель на массив структур GameCharacter.
 */
GameCharacter* createCharacters(int n) {
    return new GameCharacter[n];
}

/**
 * Очищает динамическую память, выделенную под персонажей и их инвентарь.
 *
 * @param p_characters указатель на массив персонажей.
 * @param n количество персонажей.
 * @return ничего.
 */
void deleteCharacters(GameCharacter* p_characters, int n) {
    for (int i = 0; i < n; i++) {
        if (p_characters[i].inventory_size > 0) {
            delete[] p_characters[i].p_inventory; 
        }
    }
    delete[] p_characters; 
}

/**
 * Преобразует enum класса в удобную строку для вывода.
 *
 * @param char_class класс персонажа.
 * @return строковое представление класса.
 */
string classToString(CharacterClass char_class) {
    switch (char_class) {
    case WARRIOR: return "Воин";
    case MAGE: return "Маг";
    case ROGUE: return "Разбойник";
    case CLERIC: return "Жрец";
    default: return "Неизвестно";
    }
}

/**
 * Заполняет массив персонажей случайными сгенерированными данными в разумных пределах.
 *
 * @param p_characters указатель на массив персонажей.
 * @param n количество персонажей.
 * @return ничего.
 */
void generateCharactersData(GameCharacter* p_characters, int n) {
    srand((unsigned int)time(0));

    const string NAMES[] = { "Артас", "Джайна", "Утер", "Валира", "Тралл", "Гул'дан", "Иллидан", "Сильвана" };
    const string ITEMS[] = { "Меч", "Щит", "Зелье здоровья", "Зелье маны", "Посох", "Лук", "Кинжал", "Кольцо", "Амулет" };

    int names_count = 8;
    int items_count = 9;

    for (int i = 0; i < n; i++) {
        p_characters[i].name = NAMES[rand() % names_count];
        p_characters[i].character_class = static_cast<CharacterClass>(rand() % 4);
        p_characters[i].level = rand() % 60 + 1; 


        p_characters[i].health = p_characters[i].level * (rand() % 20 + 50);
        p_characters[i].mana = p_characters[i].level * (rand() % 10 + 20);


        p_characters[i].inventory_size = rand() % 6;

        if (p_characters[i].inventory_size > 0) {
            p_characters[i].p_inventory = new string[p_characters[i].inventory_size];
            for (int j = 0; j < p_characters[i].inventory_size; j++) {
                p_characters[i].p_inventory[j] = ITEMS[rand() % items_count];
            }
        }
        else {
            p_characters[i].p_inventory = nullptr;
        }
    }
}

/**
 * Выводит всех персонажей и их характеристики в консоль.
 *
 * @param p_characters указатель на массив персонажей.
 * @param n количество персонажей.
 * @return ничего.
 */
void printCharacters(GameCharacter* p_characters, int n) {
    for (int i = 0; i < n; i++) {
        cout << "[" << i + 1 << "] " << p_characters[i].name
            << " | Класс: " << classToString(p_characters[i].character_class)
            << " | Уровень: " << p_characters[i].level
            << " | HP: " << p_characters[i].health
            << " | MP: " << p_characters[i].mana << endl;

        cout << "    Инвентарь (" << p_characters[i].inventory_size << "): ";
        if (p_characters[i].inventory_size == 0) {
            cout << "Пусто";
        }
        else {
            for (int j = 0; j < p_characters[i].inventory_size; j++) {
                cout << p_characters[i].p_inventory[j] << (j < p_characters[i].inventory_size - 1 ? ", " : "");
            }
        }
        cout << "\n----------------------------------------\n";
    }
}

/**
 * Находит всех персонажей, у которых есть определенный предмет в инвентаре.
 *
 * @param p_characters указатель на массив персонажей.
 * @param n количество персонажей.
 * @param target_item предмет для поиска.
 * @return ничего.
 */
void searchByItem(GameCharacter* p_characters, int n, string target_item) {
    bool found_any = false;
    cout << "\nПерсонажи с предметом '" << target_item << "':" << endl;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < p_characters[i].inventory_size; j++) {
            if (p_characters[i].p_inventory[j] == target_item) {
                cout << "- " << p_characters[i].name << " (Уровень " << p_characters[i].level << ")" << endl;
                found_any = true;
                break; 
            }
        }
    }

    if (!found_any) cout << "Персонажей с таким предметом не найдено." << endl;
}

/**
 * Находит и выводит персонажей с максимальным и минимальным уровнем среди указанного класса.
 *
 * @param p_characters указатель на массив персонажей.
 * @param n количество персонажей.
 * @param target_class целевой класс для поиска.
 * @return ничего.
 */
void findMinMaxLevelByClass(GameCharacter* p_characters, int n, CharacterClass target_class) {
    int max_lvl = -1;
    int min_lvl = 61; 
    string max_name = "", min_name = "";

    bool class_exists = false;

    for (int i = 0; i < n; i++) {
        if (p_characters[i].character_class == target_class) {
            class_exists = true;
            if (p_characters[i].level > max_lvl) {
                max_lvl = p_characters[i].level;
                max_name = p_characters[i].name;
            }
            if (p_characters[i].level < min_lvl) {
                min_lvl = p_characters[i].level;
                min_name = p_characters[i].name;
            }
        }
    }

    if (class_exists) {
        cout << "\nСтатистика по классу " << classToString(target_class) << ":" << endl;
        cout << "Максимальный уровень: " << max_name << " (" << max_lvl << ")" << endl;
        cout << "Минимальный уровень: " << min_name << " (" << min_lvl << ")" << endl;
    }
    else {
        cout << "\nПерсонажей класса " << classToString(target_class) << " в базе нет." << endl;
    }
}

/**
 * Анализирует инвентари всех персонажей: считает уникальные предметы и находит самый частый.
 * Работает на базовых массивах без использования std::map.
 *
 * @param p_characters указатель на массив персонажей.
 * @param n количество персонажей.
 * @return ничего.
 */
void analyzeInventory(GameCharacter* p_characters, int n) {
    int total_items_ever = 0;
    for (int i = 0; i < n; i++) total_items_ever += p_characters[i].inventory_size;

    if (total_items_ever == 0) {
        cout << "\nУ всех персонажей пустые инвентари!" << endl;
        return;
    }

    string* p_unique_items = new string[total_items_ever];
    int* p_item_counts = new int[total_items_ever];
    int unique_count = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < p_characters[i].inventory_size; j++) {
            string current_item = p_characters[i].p_inventory[j];
            bool item_found = false;

            for (int k = 0; k < unique_count; k++) {
                if (p_unique_items[k] == current_item) {
                    p_item_counts[k]++;
                    item_found = true;
                    break;
                }
            }


            if (!item_found) {
                p_unique_items[unique_count] = current_item;
                p_item_counts[unique_count] = 1;
                unique_count++;
            }
        }
    }


    int max_count = 0;
    string most_popular = "";
    for (int k = 0; k < unique_count; k++) {
        if (p_item_counts[k] > max_count) {
            max_count = p_item_counts[k];
            most_popular = p_unique_items[k];
        }
    }

    cout << "\n--- Анализ инвентарей ---" << endl;
    cout << "Всего уникальных видов предметов: " << unique_count << endl;
    cout << "Самый частый предмет: " << most_popular << " (встречается " << max_count << " раз)" << endl;

    delete[] p_unique_items;
    delete[] p_item_counts;
}

/**
 * Сортирует персонажей по уровню (по убыванию), а при совпадении - по размеру инвентаря.
 * Реализовано алгоритмом сортировки пузырьком.
 *
 * @param p_characters указатель на массив персонажей.
 * @param n количество персонажей.
 * @return ничего.
 */
void sortCharacters(GameCharacter* p_characters, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            bool should_swap = false;


            if (p_characters[j].level < p_characters[j + 1].level) {
                should_swap = true;
            }

            else if (p_characters[j].level == p_characters[j + 1].level) {
                if (p_characters[j].inventory_size < p_characters[j + 1].inventory_size) {
                    should_swap = true;
                }
            }


            if (should_swap) {
                GameCharacter temp = p_characters[j];
                p_characters[j] = p_characters[j + 1];
                p_characters[j + 1] = temp;
            }
        }
    }
    cout << "\nПерсонажи отсортированы!" << endl;
}

/**
 * Усложненный вариант: Экипирует указанный предмет всем персонажам заданного класса.
 * Выделяет новую память под увеличенный массив, копирует старые вещи и добавляет новую.
 *
 * @param p_characters указатель на массив персонажей.
 * @param n количество персонажей.
 * @param target_class класс, которому выдаем предмет.
 * @param new_item название нового предмета.
 * @return ничего.
 */
void equipItemToClass(GameCharacter* p_characters, int n, CharacterClass target_class, string new_item) {
    int equipped_count = 0;

    for (int i = 0; i < n; i++) {
        if (p_characters[i].character_class == target_class) {
            int old_size = p_characters[i].inventory_size;


            string* p_new_inventory = new string[old_size + 1];


            for (int j = 0; j < old_size; j++) {
                p_new_inventory[j] = p_characters[i].p_inventory[j];
            }

            p_new_inventory[old_size] = new_item;

            if (old_size > 0) {
                delete[] p_characters[i].p_inventory;
            }

            p_characters[i].p_inventory = p_new_inventory;
            p_characters[i].inventory_size = old_size + 1;

            equipped_count++;
        }
    }

    cout << "\nПредмет '" << new_item << "' выдан персонажам класса " << classToString(target_class)
        << " (" << equipped_count << " шт.)" << endl;
}

/**
 * Вспомогательная функция для конвертации ввода пользователя в Enum.
 */
CharacterClass intToClass(int choice) {
    switch (choice) {
    case 1: return WARRIOR;
    case 2: return MAGE;
    case 3: return ROGUE;
    case 4: return CLERIC;
    default: return WARRIOR;
    }
}

/**
 * Главное интерактивное меню программы.
 *
 * @param p_characters указатель на массив персонажей.
 * @param n количество персонажей.
 * @return ничего.
 */
void runMenu(GameCharacter* p_characters, int n) {
    int choice;
    do {
        cout << "\n--- МЕНЮ ---" << endl;
        cout << "1. Показать всех персонажей" << endl;
        cout << "2. Поиск персонажей по предмету" << endl;
        cout << "3. Макс/Мин уровень по классу" << endl;
        cout << "4. Анализ всех инвентарей (уникальные и частые предметы)" << endl;
        cout << "5. Отсортировать базу (по уровню и размеру инвентаря)" << endl;
        cout << "6. УСЛОЖНЕНИЕ: Выдать предмет всему классу" << endl;
        cout << "0. Выход" << endl;
        cout << "Ваш выбор: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            printCharacters(p_characters, n);
            break;
        }
        case 2: {
            string item;
            cout << "Введите название предмета (например: Щит, Лук): ";
            cin.ignore();
            getline(cin, item);
            searchByItem(p_characters, n, item);
            break;
        }
        case 3: {
            int class_choice;
            cout << "Выберите класс (1-Воин, 2-Маг, 3-Разбойник, 4-Жрец): ";
            cin >> class_choice;
            findMinMaxLevelByClass(p_characters, n, intToClass(class_choice));
            break;
        }
        case 4: {
            analyzeInventory(p_characters, n);
            break;
        }
        case 5: {
            sortCharacters(p_characters, n);
            printCharacters(p_characters, n);
            break;
        }
        case 6: {
            int class_choice;
            string item;
            cout << "Выберите класс (1-Воин, 2-Маг, 3-Разбойник, 4-Жрец): ";
            cin >> class_choice;
            cout << "Введите название предмета (например: Эпический меч): ";
            cin.ignore();
            getline(cin, item);
            equipItemToClass(p_characters, n, intToClass(class_choice), item);
            break;
        }
        case 0:
            cout << "Выход из программы..." << endl;
            break;
        default:
            cout << "Неверный выбор. Попробуйте еще раз." << endl;
            break;
        }
    } while (choice != 0);
}