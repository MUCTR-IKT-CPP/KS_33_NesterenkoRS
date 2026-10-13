#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <Windows.h>
#include <algorithm>

using namespace std;


/**
 * Запускает интерактивное консольное меню для управления коллекцией данных.
 *
 * @return ничего.
 */
void showMenu();



struct TextDocument {
	int pages;
};

struct Spreadsheet {
	int rows;
	int columns;
};

struct Image {
	int width;
	int height;
};

enum class DataType {
	NONE,
	TEXT,
	SPREADSHEET,
	IMAGE
};

class DataNode {
private:
	void* _p_data;
	DataType _type;
	string _name;

	void _clear() {
		switch (this->_type) {
		case DataType::TEXT:
			delete (TextDocument*)this->_p_data;
			break;
		case DataType::SPREADSHEET:
			delete (Spreadsheet*)this->_p_data;
			break;
		case DataType::IMAGE:
			delete (Image*)this->_p_data;
			break;
		case DataType::NONE:
			break;
		}
	}

	void _copyFrom(const DataNode& data_node) {
		switch (data_node._type) {
		case DataType::TEXT:
			this->_p_data = new TextDocument{ *(TextDocument*)data_node._p_data };
			break;
		case DataType::SPREADSHEET:
			this->_p_data = new Spreadsheet{ *(Spreadsheet*)data_node._p_data };
			break;
		case DataType::IMAGE:
			this->_p_data = new Image{ *(Image*)data_node._p_data };
			break;
		case DataType::NONE:
			this->_p_data = nullptr;
			break;
		}
	}

public:
	/**
	 * Конструктор по умолчанию. Инициализирует пустой узел.
	 */
	DataNode() : _p_data(nullptr), _type(DataType::NONE), _name("") {}

	/**
	 * Конструктор с параметрами.
	 *
	 * @param p_data указатель на выделенную память с данными.
	 * @param type тип сохраняемых данных.
	 * @param name имя узла.
	 */
	DataNode(void* p_data, DataType type, string name)
		: _p_data(p_data), _type(type), _name(name) {
	}

	/**
	 * Конструктор копирования.
	 *
	 * @param data_node узел для копирования (глубокое копирование).
	 */
	DataNode(const DataNode& data_node) : _type(data_node._type), _name(data_node._name) {
		_copyFrom(data_node);
	}

	/**
	 * Оператор копирующего присваивания.
	 *
	 * @param data_node узел для копирования.
	 * @return ссылка на текущий объект.
	 */
	DataNode& operator=(const DataNode& data_node) {
		if (this == &data_node) return *this;

		_clear();
		_copyFrom(data_node);

		this->_name = data_node._name;
		this->_type = data_node._type;

		return *this;
	}

	/**
	 * Конструктор перемещения.
	 *
	 * @param data_node узел, ресурсы которого будут перемещены.
	 */
	DataNode(DataNode&& data_node) noexcept
		: _p_data(data_node._p_data), _type(data_node._type), _name(move(data_node._name)) {
		data_node._p_data = nullptr;
		data_node._type = DataType::NONE;
		data_node._name = "";
	}

	/**
	 * Оператор перемещающего присваивания.
	 *
	 * @param data_node узел, ресурсы которого будут перемещены.
	 * @return ссылка на текущий объект.
	 */
	DataNode& operator=(DataNode&& data_node) noexcept {
		if (this == &data_node) return *this;

		_clear();

		this->_name = move(data_node._name);
		this->_type = data_node._type;
		this->_p_data = data_node._p_data;

		data_node._p_data = nullptr;
		data_node._type = DataType::NONE;

		return *this;
	}

	/**
	 * Деструктор. Освобождает динамическую память узла.
	 */
	~DataNode() {
		_clear();
	}

	/**
	 * Возвращает тип данных, хранящихся в узле.
	 *
	 * @return перечисление DataType, соответствующее типу данных.
	 */
	DataType getType() const { return this->_type; }

	/**
	 * Возвращает имя узла.
	 *
	 * @return строка с именем узла.
	 */
	string getName() const { return this->_name; }

	/**
	 * Выводит информацию о внутреннем состоянии узла в консоль.
	 *
	 * @return ничего.
	 */
	void print() const {
		cout << "Узел: " << this->_name << " | Тип: ";

		switch (this->_type) {
		case DataType::TEXT:
			cout << "TEXT (Страниц: " << ((TextDocument*)this->_p_data)->pages << ")";
			break;
		case DataType::SPREADSHEET:
			cout << "SPREADSHEET (Строк: " << ((Spreadsheet*)this->_p_data)->rows
				<< ", Столбцов: " << ((Spreadsheet*)this->_p_data)->columns << ")";
			break;
		case DataType::IMAGE:
			cout << "IMAGE (Ширина: " << ((Image*)this->_p_data)->width
				<< ", Высота: " << ((Image*)this->_p_data)->height << ")";
			break;
		case DataType::NONE:
			cout << "NONE";
			break;
		}
		cout << endl;
	}
};

class DataCollection {
private:
	int _size;
	DataNode* _p_array;

	void _clear() {
		delete[] this->_p_array;
	}

	void _copyFrom(const DataCollection& other) {
		this->_p_array = new DataNode[other._size];
		copy(other._p_array, other._p_array + other._size, this->_p_array);
	}

public:
	/**
	 * Конструктор по умолчанию. Создает пустую коллекцию.
	 */
	DataCollection() : _size(0), _p_array(nullptr) {}

	/**
	 * Конструктор с параметрами. Выделяет память под заданное число узлов.
	 *
	 * @param size количество выделяемых ячеек в массиве.
	 */
	DataCollection(int size) : _size(size), _p_array(new DataNode[size]) {}

	/**
	 * Конструктор копирования.
	 *
	 * @param other коллекция для копирования.
	 */
	DataCollection(const DataCollection& other) : _size(other._size) {
		_copyFrom(other);
	}

	/**
	 * Оператор копирующего присваивания.
	 *
	 * @param other коллекция для копирования.
	 * @return ссылка на текущий объект коллекции.
	 */
	DataCollection& operator=(const DataCollection& other) {
		if (this == &other) return *this;

		_clear();

		this->_size = other._size;
		_copyFrom(other);

		return *this;
	}

	/**
	 * Конструктор перемещения.
	 *
	 * @param other коллекция, ресурсы которой будут перемещены.
	 */
	DataCollection(DataCollection&& other) noexcept
		: _size(other._size), _p_array(other._p_array) {
		other._size = 0;
		other._p_array = nullptr;
	}

	/**
	 * Оператор перемещающего присваивания.
	 *
	 * @param other коллекция, ресурсы которой будут перемещены.
	 * @return ссылка на текущий объект коллекции.
	 */
	DataCollection& operator=(DataCollection&& other) noexcept {
		if (this == &other) return *this;

		_clear();

		this->_size = other._size;
		this->_p_array = other._p_array;

		other._size = 0;
		other._p_array = nullptr;

		return *this;
	}

	/**
	 * Деструктор. Освобождает память, выделенную под массив.
	 */
	~DataCollection() {
		_clear();
	}

	/**
	 * Добавляет новый узел данных в конец коллекции с перевыделением памяти.
	 *
	 * @param new_node узел, копируемый в коллекцию.
	 * @return ничего.
	 */
	void add(const DataNode& new_node) {
		DataNode* p_new_array = new DataNode[this->_size + 1];
		copy(this->_p_array, this->_p_array + this->_size, p_new_array);

		p_new_array[this->_size] = new_node;

		delete[] this->_p_array;

		this->_p_array = p_new_array;
		this->_size++;
	}

	/**
	 * Выводит текущую статистику и содержимое всей коллекции в консоль.
	 *
	 * @return ничего.
	 */
	void print() const {
		cout << "\n--- Статистика коллекции ---" << endl;
		cout << "Всего элементов: " << this->_size << endl;
		for (int i = 0; i < this->_size; i++) {
			cout << "[" << i << "] ";
			this->_p_array[i].print();
		}
		cout << "----------------------------\n" << endl;
	}

	/**
	 * Выполняет поиск узлов по типу и выводит их в консоль.
	 *
	 * @param type искомый тип данных.
	 * @return ничего.
	 */
	void searchByType(DataType type) const {
		cout << "\n--- Результаты поиска ---" << endl;
		bool found = false;
		for (int i = 0; i < this->_size; i++) {
			if (this->_p_array[i].getType() == type) {
				this->_p_array[i].print();
				found = true;
			}
		}
		if (!found) cout << "Элементы такого типа не найдены." << endl;
		cout << "-------------------------\n" << endl;
	}

	/**
	 * Удаляет первое вхождение узла с заданным именем и перераспределяет память массива.
	 *
	 * @param name имя удаляемого узла.
	 * @return ничего.
	 */
	void removeByName(const string& name) {
		int index_to_remove = -1;

		for (int i = 0; i < this->_size; i++) {
			if (this->_p_array[i].getName() == name) {
				index_to_remove = i;
				break;
			}
		}

		if (index_to_remove == -1) {
			cout << "Ошибка: Элемент '" << name << "' не найден для удаления." << endl;
			return;
		}

		if (this->_size == 1) {
			delete[] this->_p_array;
			this->_p_array = nullptr;
			this->_size = 0;
			cout << "Последний элемент '" << name << "' удален. Коллекция пуста." << endl;
			return;
		}

		DataNode* p_new_array = new DataNode[this->_size - 1];

		copy(this->_p_array, this->_p_array + index_to_remove, p_new_array);
		copy(this->_p_array + index_to_remove + 1, this->_p_array + this->_size, p_new_array + index_to_remove);

		delete[] this->_p_array;
		this->_p_array = p_new_array;
		this->_size--;

		cout << "Элемент '" << name << "' успешно удален." << endl;
	}

	/**
	 * Сохраняет (выводит) коллекцию в текстовом формате CSV.
	 *
	 * @return ничего.
	 */
	void saveToText() const {
		cout << "\n=== ЭКСПОРТ В ТЕКСТОВЫЙ ФОРМАТ ===" << endl;
		for (int i = 0; i < this->_size; i++) {
			cout << this->_p_array[i].getName() << ";";
			switch (this->_p_array[i].getType()) {
			case DataType::TEXT: cout << "TEXT"; break;
			case DataType::SPREADSHEET: cout << "SPREADSHEET"; break;
			case DataType::IMAGE: cout << "IMAGE"; break;
			case DataType::NONE: cout << "NONE"; break;
			}
			cout << endl;
		}
		cout << "==================================\n" << endl;
	}

	/**
		 * Возвращает текущее количество элементов в коллекции.
		 *
		 * @return целое число, размер массива.
		 */
	int getSize() const { return this->_size; }
};

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	showMenu();
	DataCollection* a = new DataCollection();
	return 0;
}

void showMenu() {
	DataCollection collection;
	int choice;

	while (true) {
		cout << "\n=== МЕНЮ УПРАВЛЕНИЯ КОЛЛЕКЦИЕЙ ===" << endl;
		cout << "1. Добавить элементы (демонстрация)" << endl;
		cout << "2. Вывести статистику коллекции" << endl;
		cout << "3. Найти элементы по типу" << endl;
		cout << "4. Удалить элемент по имени" << endl;
		cout << "5. Сохранить в текст (экспорт)" << endl;
		cout << "0. Выход" << endl;
		cout << "Выберите действие: ";

		cin >> choice;

		switch (choice) {
		case 1: {
			if (collection.getSize() > 0) {
				cout << "Ошибка: Демонстрационные данные можно добавить только в пустую коллекцию!" << endl;
				break;
			}

			TextDocument* text = new TextDocument{ 25 };
			DataNode node1(text, DataType::TEXT, "Report");

			Image* img = new Image{ 1920, 1080 };
			DataNode node2(img, DataType::IMAGE, "Wallpaper");

			collection.add(node1);
			collection.add(node2);
			cout << "Тестовые элементы успешно добавлены!" << endl;
			break;
		}
		case 2: {
			collection.print();
			break;
		}
		case 3: {
			cout << "Ищем все изображения (IMAGE):" << endl;
			collection.searchByType(DataType::IMAGE);
			break;
		}
		case 4: {
			string name_to_remove;
			cout << "Введите имя для удаления: ";
			cin >> name_to_remove;
			collection.removeByName(name_to_remove);
			break;
		}
		case 5: {
			collection.saveToText();
			break;
		}
		case 0: {
			cout << "Завершение работы программы. До свидания!" << endl;
			return;
		}
		default: {
			cout << "Неверный ввод, попробуйте снова." << endl;
			break;
		}
		}
	}
}