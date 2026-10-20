#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <Windows.h>
#include <algorithm>
#include <cmath>
#include <clocale>

using namespace std;


const int MIN_COLLECTION_SIZE = 5;
const int MAX_COLLECTION_SIZE = 15;
const int ADVANCED_TEST_SIZE = 10;

const double DEFAULT_SIDE = 1.0;
const double DEFAULT_TRIANGLE_SIDE = 3.0;

const double SQUARE_MIN_SIDE = 1.0;
const double SQUARE_MAX_SIDE = 10.0;

const double RECT_MIN_SIDE = 1.0;
const double RECT_MAX_WIDTH = 8.0;
const double RECT_MAX_HEIGHT = 6.0;

const double TRIANGLE_MIN_SIDE = 3.0;
const double TRIANGLE_MAX_SIDE = 7.0;

/**
 * Вспомогательная функция для округления числа до 2 знаков после запятой.
 *
 * @param value исходное вещественное число.
 * @return число, округленное до сотых.
 */
double roundToTwoDecimals(double value) {
    return round(value * 100.0) / 100.0;
}

/**
 * Вспомогательная функция для генерации случайного числа типа double.
 *
 * @param min минимальное значение.
 * @param max максимальное значение.
 * @return случайное вещественное число в диапазоне [min, max].
 */
double getRandomDouble(double min, double max) {
    double f = (double)rand() / RAND_MAX;
    return min + f * (max - min);
}

/**
 * Перечисление типов фигур для реализации статического полиморфизма.
 */
enum class ShapeType {
    NONE,
    SQUARE,
    RECTANGLE,
    TRIANGLE
};

/**
 * Базовый класс Фигура.
 * Предоставляет защищенные поля размеров и общий интерфейс для наследников.
 */
class Shape {
protected:
    double side_a;
    double side_b;
    double side_c;
    ShapeType shape_type;

    Shape(ShapeType type) : shape_type(type), side_a(0.0), side_b(0.0), side_c(0.0) {}

public:
    double getSideA() const { return side_a; }
    double getSideB() const { return side_b; }
    double getSideC() const { return side_c; }
    ShapeType getType() const { return shape_type; }

    /**
     * Сеттер первой стороны.
     */
    void setSideA(double a) {
        if (a <= 0.0) {
            cout << "Ошибка валидации: размер должен быть > 0!" << endl;
            return;
        }
        if (shape_type == ShapeType::TRIANGLE) {
            if (!(a + side_b > side_c && a + side_c > side_b && side_b + side_c > a)) {
                cout << "Ошибка: изменение стороны сломает неравенство треугольника!" << endl;
                return;
            }
        }
        this->side_a = a;
    }

    /**
     * Сеттер второй стороны.
     */
    void setSideB(double b) {
        if (b <= 0.0) {
            cout << "Ошибка валидации: размер должен быть > 0!" << endl;
            return;
        }
        if (shape_type == ShapeType::TRIANGLE) {
            if (!(side_a + b > side_c && side_a + side_c > b && b + side_c > side_a)) {
                cout << "Ошибка: изменение стороны сломает неравенство треугольника!" << endl;
                return;
            }
        }
        this->side_b = b;
    }

    /**
     * Сеттер третьей стороны.
     */
    void setSideC(double c) {
        if (c <= 0.0) {
            cout << "Ошибка валидации: размер должен быть > 0!" << endl;
            return;
        }
        if (shape_type == ShapeType::TRIANGLE) {
            if (!(side_a + side_b > c && side_a + c > side_b && side_b + c > side_a)) {
                cout << "Ошибка: изменение стороны сломает неравенство треугольника!" << endl;
                return;
            }
        }
        this->side_c = c;
    }

    // Прототипы методов (реализация ниже)
    double calculateArea() const;
    double calculatePerimeter() const;
    static bool compareByArea(const Shape* p_first, const Shape* p_second);
    friend ostream& operator<<(ostream& os, const Shape& shape);
};

/**
 * Дочерний класс Квадрат.
 */
class Square : public Shape {
public:
    Square(double side) : Shape(ShapeType::SQUARE) {
        if (side > 0.0) {
            this->side_a = side;
        }
        else {
            cout << "Ошибка валидации: сторона квадрата должна быть > 0! Установлено 1.0." << endl;
            this->side_a = DEFAULT_SIDE;
        }
    }

    double calculateArea() const {
        return side_a * side_a;
    }

    double calculatePerimeter() const {
        return 4.0 * side_a;
    }

    friend ostream& operator<<(ostream& os, const Square& square) {
        os << "Квадрат {сторона=" << roundToTwoDecimals(square.side_a)
            << "} S=" << roundToTwoDecimals(square.calculateArea())
            << " P=" << roundToTwoDecimals(square.calculatePerimeter());
        return os;
    }
};

/**
 * Дочерний класс Прямоугольник.
 */
class Rectangle : public Shape {
public:
    Rectangle(double width, double height) : Shape(ShapeType::RECTANGLE) {
        if (width > 0.0 && height > 0.0) {
            this->side_a = width;
            this->side_b = height;
        }
        else {
            cout << "Ошибка валидации: стороны прямоугольника должны быть > 0! Установлено 1.0." << endl;
            this->side_a = DEFAULT_SIDE;
            this->side_b = DEFAULT_SIDE;
        }
    }

    double calculateArea() const {
        return side_a * side_b;
    }

    double calculatePerimeter() const {
        return 2.0 * (side_a + side_b);
    }

    friend ostream& operator<<(ostream& os, const Rectangle& rect) {
        os << "Прямоугольник {стороны=" << roundToTwoDecimals(rect.side_a) << ", " << roundToTwoDecimals(rect.side_b)
            << "} S=" << roundToTwoDecimals(rect.calculateArea())
            << " P=" << roundToTwoDecimals(rect.calculatePerimeter());
        return os;
    }
};

/**
 * Дочерний класс Треугольник.
 */
class Triangle : public Shape {
public:
    Triangle(double a, double b, double c) : Shape(ShapeType::TRIANGLE) {
        if (a > 0.0 && b > 0.0 && c > 0.0 && (a + b > c) && (a + c > b) && (b + c > a)) {
            this->side_a = a;
            this->side_b = b;
            this->side_c = c;
        }
        else {
            cout << "Ошибка: некорректные стороны треугольника! Установлены стороны 3.0." << endl;
            this->side_a = DEFAULT_TRIANGLE_SIDE;
            this->side_b = DEFAULT_TRIANGLE_SIDE;
            this->side_c = DEFAULT_TRIANGLE_SIDE;
        }
    }

    double calculatePerimeter() const {
        return side_a + side_b + side_c;
    }

    double calculateArea() const {
        double half_p = calculatePerimeter() / 2.0;
        return sqrt(half_p * (half_p - side_a) * (half_p - side_b) * (half_p - side_c));
    }

    friend ostream& operator<<(ostream& os, const Triangle& triangle) {
        os << "Треугольник {стороны=" << roundToTwoDecimals(triangle.side_a) << ", "
            << roundToTwoDecimals(triangle.side_b) << ", " << roundToTwoDecimals(triangle.side_c)
            << "} S=" << roundToTwoDecimals(triangle.calculateArea())
            << " P=" << roundToTwoDecimals(triangle.calculatePerimeter());
        return os;
    }
};



double Shape::calculateArea() const {
    switch (shape_type) {
    case ShapeType::SQUARE: return ((Square*)this)->calculateArea();
    case ShapeType::RECTANGLE: return ((class Rectangle*)this)->calculateArea();
    case ShapeType::TRIANGLE: return ((Triangle*)this)->calculateArea();
    default: return 0.0;
    }
}

double Shape::calculatePerimeter() const {
    switch (shape_type) {
    case ShapeType::SQUARE: return ((Square*)this)->calculatePerimeter();
    case ShapeType::RECTANGLE: return ((class Rectangle*)this)->calculatePerimeter();
    case ShapeType::TRIANGLE: return ((Triangle*)this)->calculatePerimeter();
    default: return 0.0;
    }
}

bool Shape::compareByArea(const Shape* p_first, const Shape* p_second) {
    if (p_first == nullptr || p_second == nullptr) return false;
    // Сортировка по возрастанию площади (если первая больше, меняем местами)
    return p_first->calculateArea() > p_second->calculateArea();
}

ostream& operator<<(ostream& os, const Shape& shape) {
    switch (shape.shape_type) {
    case ShapeType::SQUARE: os << (const Square&)shape; break;
    case ShapeType::RECTANGLE: os << (const class Rectangle&)shape; break;
    case ShapeType::TRIANGLE: os << (const Triangle&)shape; break;
    default: os << "Неизвестная фигура"; break;
    }
    return os;
}

/**
 * Класс-менеджер коллекции фигур.
 * Управляет динамическим массивом указателей и реализует Правило пяти.
 */
class GeometricCollection {
private:
    Shape** _p_shapes;
    int _size;
    int _count;

    /**
     * Вспомогательный метод для клонирования фигуры при глубоком копировании.
     * @param p_source указатель на копируемую фигуру.
     * @return новый указатель на копию фигуры.
     */
    Shape* _cloneShape(const Shape* p_source) {
        if (!p_source) return nullptr;
        switch (p_source->getType()) {
        case ShapeType::SQUARE: return new Square(*(const Square*)p_source);
        case ShapeType::RECTANGLE: return new class Rectangle(*(const class Rectangle*)p_source);
        case ShapeType::TRIANGLE: return new Triangle(*(const Triangle*)p_source);
        default: return nullptr;
        }
    }

    /**
     * Вспомогательный метод для очистки динамической памяти массива.
     */
    void _clear() {
        if (this->_p_shapes != nullptr) {
            for (int i = 0; i < this->_count; i++) {
                if (this->_p_shapes[i] != nullptr) {
                    switch (this->_p_shapes[i]->getType()) {
                    case ShapeType::SQUARE: delete (Square*)this->_p_shapes[i]; break;
                    case ShapeType::RECTANGLE: delete (class Rectangle*)this->_p_shapes[i]; break;
                    case ShapeType::TRIANGLE: delete (Triangle*)this->_p_shapes[i]; break;
                    default: break;
                    }
                }
            }
            delete[] this->_p_shapes;
            this->_p_shapes = nullptr;
        }
        this->_count = 0;
        this->_size = 0;
    }

    /**
     * Вспомогательный метод для глубокого копирования коллекции.
     */
    void _copy(const GeometricCollection& collection) {
        this->_size = collection._size;
        this->_count = collection._count;
        this->_p_shapes = new Shape * [this->_size];
        for (int i = 0; i < this->_count; i++) {
            this->_p_shapes[i] = _cloneShape(collection._p_shapes[i]);
        }
    }

public:
    /** Конструктор по умолчанию. */
    GeometricCollection() : _size(ADVANCED_TEST_SIZE), _count(0), _p_shapes(new Shape* [ADVANCED_TEST_SIZE]) {}

    /** Конструктор с параметром. */
    GeometricCollection(int size) {
        this->_size = (size >= MIN_COLLECTION_SIZE && size <= MAX_COLLECTION_SIZE) ? size : ADVANCED_TEST_SIZE;
        this->_count = 0;
        this->_p_shapes = new Shape * [this->_size];
    }

    /** Конструктор копирования. */
    GeometricCollection(const GeometricCollection& other) {
        _copy(other);
    }

    /** Оператор копирующего присваивания. */
    GeometricCollection& operator=(const GeometricCollection& other) {
        if (this == &other) return *this;
        _clear();
        _copy(other);
        return *this;
    }

    /** Конструктор перемещения. */
    GeometricCollection(GeometricCollection&& other) noexcept
        : _size(other._size), _count(other._count), _p_shapes(other._p_shapes) {
        other._p_shapes = nullptr;
        other._count = 0;
        other._size = 0;
    }

    /** Оператор перемещающего присваивания. */
    GeometricCollection& operator=(GeometricCollection&& other) noexcept {
        if (this == &other) return *this;
        _clear();
        this->_size = other._size;
        this->_count = other._count;
        this->_p_shapes = other._p_shapes;

        other._p_shapes = nullptr;
        other._count = 0;
        other._size = 0;
        return *this;
    }

    /** Деструктор. */
    ~GeometricCollection() {
        _clear();
    }

    /**
     * Добавляет фигуру в коллекцию.
     * @param p_new_shape указатель на новую фигуру.
     */
    void addShape(Shape* p_new_shape) {
        if (!p_new_shape) return;
        if (this->_count >= this->_size) {
            cout << "Коллекция заполнена! Максимум элементов: " << this->_size << endl;
            return;
        }
        this->_p_shapes[this->_count] = p_new_shape;
        this->_count++;
    }

    /**
     * Удаляет фигуру из коллекции по индексу со смещением оставшихся элементов.
     * @param index позиция фигуры в массиве.
     */
    void removeShapeByIndex(int index) {
        if (index < 0 || index >= this->_count) {
            cout << "Ошибка: неверный индекс!" << endl;
            return;
        }

        switch (this->_p_shapes[index]->getType()) {
        case ShapeType::SQUARE: delete (Square*)this->_p_shapes[index]; break;
        case ShapeType::RECTANGLE: delete (class Rectangle*)this->_p_shapes[index]; break;
        case ShapeType::TRIANGLE: delete (Triangle*)this->_p_shapes[index]; break;
        default: break;
        }

        for (int i = index; i < this->_count - 1; i++) {
            this->_p_shapes[i] = this->_p_shapes[i + 1];
        }
        this->_count--;
    }

    /**
     * Генерирует случайные фигуры и заполняет ими коллекцию.
     * @param count желаемое количество случайных фигур.
     */
    void generateRandomShapes(int count) {
        _clear();
        this->_size = (count >= MIN_COLLECTION_SIZE && count <= MAX_COLLECTION_SIZE) ? count : ADVANCED_TEST_SIZE;
        this->_p_shapes = new Shape * [this->_size];

        for (int i = 0; i < this->_size; i++) {
            int type = rand() % 3;
            Shape* p_new = nullptr;

            if (type == 0) {
                p_new = new Square(getRandomDouble(SQUARE_MIN_SIDE, SQUARE_MAX_SIDE));
            }
            else if (type == 1) {
                p_new = new class Rectangle(getRandomDouble(RECT_MIN_SIDE, RECT_MAX_WIDTH),
                    getRandomDouble(RECT_MIN_SIDE, RECT_MAX_HEIGHT));
            }
            else {
                double a, b, c;
                do {
                    a = getRandomDouble(TRIANGLE_MIN_SIDE, TRIANGLE_MAX_SIDE);
                    b = getRandomDouble(TRIANGLE_MIN_SIDE, TRIANGLE_MAX_SIDE);
                    c = getRandomDouble(TRIANGLE_MIN_SIDE, TRIANGLE_MAX_SIDE);
                } while (!(a + b > c && a + c > b && b + c > a)); 
                p_new = new Triangle(a, b, c);
            }
            addShape(p_new);
        }
    }

    /**
     * Считает суммарную площадь всех сохраненных фигур.
     * @return общая площадь.
     */
    double calculateTotalArea() const {
        double total_area = 0.0;
        for (int i = 0; i < this->_count; i++) {
            total_area += this->_p_shapes[i]->calculateArea();
        }
        return total_area;
    }

    /**
     * Выводит информацию обо всех фигурах в терминал.
     */
    void printAll() const {
        cout << "\n--- Фигуры в коллекции ---" << endl;
        for (int i = 0; i < this->_count; i++) {
            cout << "[" << i << "] " << *(this->_p_shapes[i]) << endl;
        }
        cout << "Общая площадь всех фигур: " << roundToTwoDecimals(calculateTotalArea()) << endl;
        cout << "--------------------------\n" << endl;
    }

    /**
     * Сортирует массив по площади с использованием статического метода базового класса.
     */
    void sortByArea() {
        for (int i = 0; i < this->_count - 1; i++) {
            for (int j = 0; j < this->_count - i - 1; j++) {
                if (Shape::compareByArea(this->_p_shapes[j], this->_p_shapes[j + 1])) {
                    Shape* p_temp = this->_p_shapes[j];
                    this->_p_shapes[j] = this->_p_shapes[j + 1];
                    this->_p_shapes[j + 1] = p_temp;
                }
            }
        }
    }

    /**
     * Ищет и возвращает указатель на фигуру с самым большим периметром.
     * @return указатель на искомую фигуру.
     */
    Shape* findMaxPerimeterShape() const {
        if (this->_count == 0) return nullptr;

        Shape* p_max_shape = this->_p_shapes[0];
        double max_perimeter = p_max_shape->calculatePerimeter();

        for (int i = 1; i < this->_count; i++) {
            double current_perimeter = this->_p_shapes[i]->calculatePerimeter();
            if (current_perimeter > max_perimeter) {
                max_perimeter = current_perimeter;
                p_max_shape = this->_p_shapes[i];
            }
        }
        return p_max_shape;
    }


    void runAdvancedTest() {
        generateRandomShapes(ADVANCED_TEST_SIZE);

        cout << "\n1. Исходная коллекция:" << endl;
        printAll();

        cout << "\n2. Сортируем фигуры по площади..." << endl;
        sortByArea();

        cout << "\nОтсортированный список:" << endl;
        for (int i = 0; i < this->_count; i++) {
            cout << "Позиция #" << (i + 1) << " | " << *(this->_p_shapes[i]) << endl;
        }

        cout << "\n3. Поиск фигуры с максимальным периметром..." << endl;
        Shape* p_max = findMaxPerimeterShape();
        if (p_max != nullptr) {
            cout << "Рекордсмен: " << *p_max << endl;
        }
        cout << "====================================\n" << endl;
    }
};

/**
 * Интерактивное консольное меню для управления коллекцией.
 * @param collection ссылка на объект коллекции.
 */
void runMenu(GeometricCollection& collection) {
    int choice;
    do {
        cout << "\n=== МЕНЮ КОЛЛЕКЦИИ ФИГУР ===" << endl;
        cout << "1. Запустить усложненный тест (Генерация 10 фигур, сортировка, макс. периметр)" << endl;
        cout << "2. Сгенерировать случайные фигуры (от 5 до 15)" << endl;
        cout << "3. Вывести все фигуры и общую площадь" << endl;
        cout << "4. Добавить фигуру вручную" << endl;
        cout << "5. Удалить фигуру по индексу" << endl;
        cout << "0. Выход" << endl;
        cout << "Ваш выбор: ";
        cin >> choice;

        switch (choice) {
        case 1:
            collection.runAdvancedTest();
            break;
        case 2: {
            int count;
            cout << "Введите желаемое количество фигур (5-15): ";
            cin >> count;
            collection.generateRandomShapes(count);
            cout << "Случайная коллекция успешно сгенерирована!" << endl;
            break;
        }
        case 3:
            collection.printAll();
            break;
        case 4: {
            int type;
            cout << "Выберите фигуру (1 - Квадрат, 2 - Прямоугольник, 3 - Треугольник): ";
            cin >> type;

            if (type == 1) {
                double a;
                cout << "Введите сторону квадрата: ";
                cin >> a;
                collection.addShape(new Square(a));
            }
            else if (type == 2) {
                double a, b;
                cout << "Введите ширину и высоту через пробел: ";
                cin >> a >> b;
                collection.addShape(new class Rectangle(a, b));
            }
            else if (type == 3) {
                double a, b, c;
                cout << "Введите три стороны треугольника через пробел: ";
                cin >> a >> b >> c;
                collection.addShape(new Triangle(a, b, c));
            }
            else {
                cout << "Неверный тип фигуры!" << endl;
            }
            break;
        }
        case 5: {
            int idx;
            cout << "Введите индекс фигуры для удаления: ";
            cin >> idx;
            collection.removeShapeByIndex(idx);
            cout << "Попытка удаления завершена." << endl;
            break;
        }
        case 0:
            cout << "Завершение работы программы..." << endl;
            break;
        default:
            cout << "Неверный выбор. Попробуйте снова." << endl;
            break;
        }
    } while (choice != 0);
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand((unsigned int)time(0));

    GeometricCollection collection;
    runMenu(collection);

    return 0;
}