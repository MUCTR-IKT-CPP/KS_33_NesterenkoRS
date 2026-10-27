#include <iostream>
#include <string>
#include <Windows.h>

using namespace std;

const int DEFAULT_DEPT_SIZE = 10;

class Employee {
protected:
    int id_employee;
    string name;

    Employee(int id_emp, string emp_name) :
        id_employee(id_emp), name(emp_name) {
    }

public:
    virtual double calculateSalary() const = 0;
    virtual void printInfo() const = 0;

    virtual ~Employee() {}

    int getId() const { return id_employee; }
};

class IBonus {
public:
    virtual void applyBonus(double amount) = 0;

    virtual ~IBonus() {}
};

class FullTimeEmployee : public Employee, public IBonus {
private:
    double _monthly_salary;
    double _bonus_amount;

public:
    FullTimeEmployee(int id_emp, string emp_name, double monthly_salary, double bonus_amount = 0.0) :
        Employee(id_emp, emp_name), _monthly_salary(monthly_salary), _bonus_amount(bonus_amount) {
    }

    double calculateSalary() const override {
        return _monthly_salary + _bonus_amount;
    }

    void printInfo() const override {
        cout << "ID: " << id_employee << " | Имя: " << name
            << " | Тип: Full-Time | Оклад: " << _monthly_salary
            << " | Бонус: " << _bonus_amount
            << " | Итого ЗП: " << calculateSalary() << endl;
    }

    void applyBonus(double amount) override {
        if (amount > 0.0) {
            _bonus_amount += amount;
        }
    }
};

class PartTimeEmployee : public Employee {
private:
    double _hourly_rate;
    int _hours_worked;

public:
    PartTimeEmployee(int id_emp, string emp_name, double hourly_rate, int hours_worked) :
        Employee(id_emp, emp_name), _hourly_rate(hourly_rate), _hours_worked(hours_worked) {
    }

    double calculateSalary() const override {
        return _hourly_rate * _hours_worked;
    }

    void printInfo() const override {
        cout << "ID: " << id_employee << " | Имя: " << name
            << " | Тип: Part-Time | Ставка/час: " << _hourly_rate
            << " | Часов: " << _hours_worked
            << " | Итого ЗП: " << calculateSalary() << endl;
    }
};

class ContractEmployee : public Employee, public IBonus {
private:
    double _contract_amount;
    double _bonus_amount;

public:
    ContractEmployee(int id_emp, string emp_name, double contract_amount, double bonus_amount = 0.0) :
        Employee(id_emp, emp_name), _contract_amount(contract_amount), _bonus_amount(bonus_amount) {
    }

    double calculateSalary() const override {
        return _contract_amount + _bonus_amount;
    }

    void printInfo() const override {
        cout << "ID: " << id_employee << " | Имя: " << name
            << " | Тип: Contract | Контракт: " << _contract_amount
            << " | Бонус: " << _bonus_amount
            << " | Итого ЗП: " << calculateSalary() << endl;
    }

    void applyBonus(double amount) override {
        if (amount > 0.0) {
            _bonus_amount += amount;
        }
    }
};

class Department {
private:
    Employee** _p_employees;
    int _count;
    int _size;

    Employee* _cloneEmployee(const Employee* p_source) {
        if (p_source == nullptr) return nullptr;

        const FullTimeEmployee* p_ft = dynamic_cast<const FullTimeEmployee*>(p_source);
        if (p_ft != nullptr) {
            return new FullTimeEmployee(*p_ft);
        }

        const PartTimeEmployee* p_pt = dynamic_cast<const PartTimeEmployee*>(p_source);
        if (p_pt != nullptr) {
            return new PartTimeEmployee(*p_pt);
        }

        const ContractEmployee* p_ce = dynamic_cast<const ContractEmployee*>(p_source);
        if (p_ce != nullptr) {
            return new ContractEmployee(*p_ce);
        }

        return nullptr;
    }

    void _clear() {
        if (_p_employees != nullptr) {
            for (int i = 0; i < _count; i++) {
                delete _p_employees[i];
            }
            delete[] _p_employees;
            _p_employees = nullptr;
        }
        _count = 0;
        _size = 0;
    }

    void _copy(const Department& other) {
        _size = other._size;
        _count = other._count;
        _p_employees = new Employee * [_size];
        for (int i = 0; i < _count; i++) {
            _p_employees[i] = _cloneEmployee(other._p_employees[i]);
        }
        for (int i = _count; i < _size; i++) {
            _p_employees[i] = nullptr;
        }
    }

public:
    Department() : _p_employees(new Employee* [DEFAULT_DEPT_SIZE]), _count(0), _size(DEFAULT_DEPT_SIZE) {
        for (int i = 0; i < _size; i++) {
            _p_employees[i] = nullptr;
        }
    }

    Department(int size) : _count(0) {
        if (size > 0) {
            _size = size;
        }
        else {
            cout << "Размер не может быть <= 0, установлено " << DEFAULT_DEPT_SIZE << endl;
            _size = DEFAULT_DEPT_SIZE;
        }

        _p_employees = new Employee * [_size];
        for (int i = 0; i < _size; i++) {
            _p_employees[i] = nullptr;
        }
    }

    Department(const Department& other) : _p_employees(nullptr), _count(0), _size(0) {
        _copy(other);
    }

    Department& operator=(const Department& other) {
        if (this == &other) return *this;
        _clear();
        _copy(other);
        return *this;
    }

    Department(Department&& other) noexcept :
        _p_employees(other._p_employees), _count(other._count), _size(other._size) {
        other._p_employees = nullptr;
        other._count = 0;
        other._size = 0;
    }

    Department& operator=(Department&& other) noexcept {
        if (this == &other) return *this;
        _clear();

        _p_employees = other._p_employees;
        _count = other._count;
        _size = other._size;

        other._p_employees = nullptr;
        other._count = 0;
        other._size = 0;
        return *this;
    }

    ~Department() {
        _clear();
    }

    void addEmployee(Employee* p_emp) {
        if (p_emp == nullptr) return;
        if (_count >= _size) {
            cout << "Отдел переполнен! Сотрудник не добавлен." << endl;
            delete p_emp;
            return;
        }
        _p_employees[_count] = p_emp;
        _count++;
    }

    void removeEmployee(int id) {
        int index = -1;
        for (int i = 0; i < _count; i++) {
            if (_p_employees[i]->getId() == id) {
                index = i;
                break;
            }
        }

        if (index == -1) {
            cout << "Сотрудник с ID " << id << " не найден!" << endl;
            return;
        }

        delete _p_employees[index];
        for (int i = index; i < _count - 1; i++) {
            _p_employees[i] = _p_employees[i + 1];
        }
        _p_employees[_count - 1] = nullptr;
        _count--;
        cout << "Сотрудник с ID " << id << " удален." << endl;
    }

    double totalSalaryBudget() const {
        double total = 0.0;
        for (int i = 0; i < _count; i++) {
            total += _p_employees[i]->calculateSalary();
        }
        return total;
    }

    void printAll() const {
        if (_count == 0) {
            cout << "Отдел пуст." << endl;
            return;
        }
        for (int i = 0; i < _count; i++) {
            _p_employees[i]->printInfo();
        }
    }

    void applyBonusToAll(double amount) {
        for (int i = 0; i < _count; i++) {
            IBonus* p_bonus_emp = dynamic_cast<IBonus*>(_p_employees[i]);
            if (p_bonus_emp != nullptr) {
                p_bonus_emp->applyBonus(amount);
            }
        }
    }
};

void runMenu(Department& dept) {
    int choice = 0;
    do {
        cout << "\n=== СИСТЕМА УЧЕТА СОТРУДНИКОВ ===" << endl;
        cout << "1. Добавить Full-Time сотрудника" << endl;
        cout << "2. Добавить Part-Time сотрудника" << endl;
        cout << "3. Добавить Contract сотрудника" << endl;
        cout << "4. Удалить сотрудника по ID" << endl;
        cout << "5. Вывести всех сотрудников" << endl;
        cout << "6. Посчитать общий фонд зарплаты" << endl;
        cout << "7. Выдать бонус сотрудникам (IBonus)" << endl;
        cout << "0. Выход" << endl;
        cout << "Ваш выбор: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            int id = 0;
            string name = "";
            double salary = 0.0;
            cout << "Введите ID, Имя и Месячный оклад: ";
            cin >> id >> name >> salary;
            dept.addEmployee(new FullTimeEmployee(id, name, salary));
            break;
        }
        case 2: {
            int id = 0;
            string name = "";
            double rate = 0.0;
            int hours = 0;
            cout << "Введите ID, Имя, Почасовую ставку и Часы: ";
            cin >> id >> name >> rate >> hours;
            dept.addEmployee(new PartTimeEmployee(id, name, rate, hours));
            break;
        }
        case 3: {
            int id = 0;
            string name = "";
            double contract = 0.0;
            cout << "Введите ID, Имя и Сумму контракта: ";
            cin >> id >> name >> contract;
            dept.addEmployee(new ContractEmployee(id, name, contract));
            break;
        }
        case 4: {
            int id = 0;
            cout << "Введите ID сотрудника для удаления: ";
            cin >> id;
            dept.removeEmployee(id);
            break;
        }
        case 5: {
            dept.printAll();
            break;
        }
        case 6: {
            cout << "Общий фонд зарплаты отдела: " << dept.totalSalaryBudget() << endl;
            break;
        }
        case 7: {
            double bonus = 0.0;
            cout << "Введите сумму бонуса: ";
            cin >> bonus;
            dept.applyBonusToAll(bonus);
            cout << "Бонус начислен всем подходящим сотрудникам!" << endl;
            break;
        }
        case 0: {
            cout << "Завершение работы программы..." << endl;
            break;
        }
        default: {
            cout << "Неверный выбор. Попробуйте снова." << endl;
            break;
        }
        }
    } while (choice != 0);
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    Department dept(DEFAULT_DEPT_SIZE);
    runMenu(dept);

    return 0;
}