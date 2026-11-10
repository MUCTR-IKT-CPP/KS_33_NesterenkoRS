#include <iostream>
#include <string>
#include <Windows.h>

using namespace std;

const int DEFAULT_BUS_SIZE = 10;
const int BUS_GROWTH_FACTOR = 2;

/**
 * Интерфейс сообщения.
 */
class IMessage {
public:
    virtual string type() const = 0;
    virtual ~IMessage() {}
};

/**
 * Класс текстового сообщения.
 */
class TextMessage : public IMessage {
private:
    string _content;

public:
    TextMessage(string content) : _content(content) {}

    string type() const override {
        return "Text";
    }

    string getContent() const {
        return _content;
    }
};

/**
 * Интерфейс обработчика сообщений.
 */
class IMessageHandler {
public:
    virtual void handle(const IMessage& msg) = 0;
    virtual ~IMessageHandler() {}
};

/**
 * Политика логирования в консоль.
 */
class ConsoleLoggingPolicy {
public:
    void log(const IMessage& msg) const {
        cout << "Processing message: " << msg.type() << endl;
    }
};

/**
 * Тихая политика логирования.
 */
class SilentPolicy {
public:
    /**
     * Пустой метод логирования.
     *
     * @param msg обрабатываемое сообщение.
     * @return ничего.
     */
    void log(const IMessage& msg) const {}
};

/**
 * Шаблонный процессор сообщений, использующий выбранную политику логирования.
 */
template <typename Policy>
class MessageProcessor : public IMessageHandler {
private:
    Policy _policy;

public:
    void handle(const IMessage& msg) override {
        _policy.log(msg);
    }
};

/**
 * Шаблонный базовый класс обработчика, реализующий статический полиморфизм (CRTP).
 */
template <typename Derived>
class HandlerCRTP : public IMessageHandler {
public:
    void handle(const IMessage& msg) override {
        static_cast<Derived*>(this)->process(msg);
    }
};

/**
 * CRTP-обработчик, выводящий тип сообщения в формате JSON.
 */
class JsonHandler : public HandlerCRTP<JsonHandler> {
public:
    void process(const IMessage& msg) const {
        cout << "{ \"type\": \"" << msg.type() << "\" }" << endl;
    }
};

/**
 * Класс-агрегатор шины сообщений, управляющий динамическим массивом обработчиков.
 */
class MessageBus {
private:
    IMessageHandler** _p_handlers;
    int _count;
    int _size;

    /**
     * Клонирует обработчик для глубокого копирования с использованием dynamic_cast.
     *
     * @param p_source указатель на исходный обработчик.
     * @return указатель на новую копию обработчика.
     */
    IMessageHandler* _cloneHandler(const IMessageHandler* p_source) {
        if (p_source == nullptr) return nullptr;

        const MessageProcessor<ConsoleLoggingPolicy>* p_console =
            dynamic_cast<const MessageProcessor<ConsoleLoggingPolicy>*>(p_source);
        if (p_console != nullptr) {
            return new MessageProcessor<ConsoleLoggingPolicy>(*p_console);
        }

        const MessageProcessor<SilentPolicy>* p_silent =
            dynamic_cast<const MessageProcessor<SilentPolicy>*>(p_source);
        if (p_silent != nullptr) {
            return new MessageProcessor<SilentPolicy>(*p_silent);
        }

        const JsonHandler* p_json = dynamic_cast<const JsonHandler*>(p_source);
        if (p_json != nullptr) {
            return new JsonHandler(*p_json);
        }

        return nullptr;
    }

    /**
     * Освобождает динамическую память массива обработчиков.
     *
     * @return ничего.
     */
    void _clear() {
        if (_p_handlers != nullptr) {
            for (int i = 0; i < _count; i++) {
                delete _p_handlers[i];
            }
            delete[] _p_handlers;
            _p_handlers = nullptr;
        }
        _count = 0;
        _size = 0;
    }

    /**
     * Выполняет глубокое копирование данных из другого объекта MessageBus.
     *
     * @param other копируемый объект шины сообщений.
     * @return ничего.
     */
    void _copy(const MessageBus& other) {
        _size = other._size;
        _count = other._count;
        _p_handlers = new IMessageHandler * [_size];
        for (int i = 0; i < _count; i++) {
            _p_handlers[i] = _cloneHandler(other._p_handlers[i]);
        }
        for (int i = _count; i < _size; i++) {
            _p_handlers[i] = nullptr;
        }
    }

    /**
     * Расширяет динамический массив при заполнении.
     *
     * @return ничего.
     */
    void _resize() {
        const int new_size = (_size > 0) ? (_size * BUS_GROWTH_FACTOR) : DEFAULT_BUS_SIZE;
        IMessageHandler** p_new_handlers = new IMessageHandler * [new_size];

        for (int i = 0; i < _count; i++) {
            p_new_handlers[i] = _p_handlers[i];
        }
        for (int i = _count; i < new_size; i++) {
            p_new_handlers[i] = nullptr;
        }

        delete[] _p_handlers;
        _p_handlers = p_new_handlers;
        _size = new_size;
    }

public:
    MessageBus() : _p_handlers(new IMessageHandler* [DEFAULT_BUS_SIZE]), _count(0), _size(DEFAULT_BUS_SIZE) {
        for (int i = 0; i < _size; i++) {
            _p_handlers[i] = nullptr;
        }
    }

    MessageBus(int size) : _count(0) {
        if (size > 0) {
            _size = size;
        }
        else {
            cout << "Размер не может быть <= 0, установлено " << DEFAULT_BUS_SIZE << endl;
            _size = DEFAULT_BUS_SIZE;
        }

        _p_handlers = new IMessageHandler * [_size];
        for (int i = 0; i < _size; i++) {
            _p_handlers[i] = nullptr;
        }
    }

    MessageBus(const MessageBus& other) : _p_handlers(nullptr), _count(0), _size(0) {
        _copy(other);
    }

    MessageBus& operator=(const MessageBus& other) {
        if (this == &other) return *this;
        _clear();
        _copy(other);
        return *this;
    }

    MessageBus(MessageBus&& other) noexcept :
        _p_handlers(other._p_handlers), _count(other._count), _size(other._size) {
        other._p_handlers = nullptr;
        other._count = 0;
        other._size = 0;
    }

    MessageBus& operator=(MessageBus&& other) noexcept {
        if (this == &other) return *this;
        _clear();

        _p_handlers = other._p_handlers;
        _count = other._count;
        _size = other._size;

        other._p_handlers = nullptr;
        other._count = 0;
        other._size = 0;
        return *this;
    }

    ~MessageBus() {
        _clear();
    }

    /**
     * Подписывает новый обработчик на получение сообщений.
     *
     * @param p_handler указатель на добавляемый обработчик.
     * @return ничего.
     */
    void subscribe(IMessageHandler* p_handler) {
        if (p_handler == nullptr) return;
        if (_count >= _size) {
            _resize();
        }
        _p_handlers[_count] = p_handler;
        _count++;
    }

    /**
     * Публикует сообщение и вызывает метод handle() у всех подписанных обработчиков.
     *
     * @param msg публикуемое сообщение.
     * @return ничего.
     */
    void publish(const IMessage& msg) const {
        if (_count == 0) {
            cout << "В шине нет подписанных обработчиков." << endl;
            return;
        }
        for (int i = 0; i < _count; i++) {
            _p_handlers[i]->handle(msg);
        }
    }

    /**
     * Возвращает текущее количество подписанных обработчиков.
     *
     * @return количество обработчиков.
     */
    int getCount() const {
        return _count;
    }
};

void runDemo();
void runMenu(MessageBus& bus);

int main() {
    setlocale(LC_ALL, "Russian");

    MessageBus main_bus(DEFAULT_BUS_SIZE);
    runMenu(main_bus);

    return 0;
}

/**
 * Интерактивное меню для тестирования системы обработки сообщений.
 *
 * @param bus ссылка на объект шины сообщений.
 * @return ничего.
 */
void runMenu(MessageBus& bus) {
    int choice = 0;
    do {
        cout << "\n=== МЕНЮ MESSAGE BUS ===" << endl;
        cout << "1. Подписать ConsoleLoggingPolicy процессор" << endl;
        cout << "2. Подписать SilentPolicy процессор" << endl;
        cout << "3. Подписать JsonHandler (CRTP)" << endl;
        cout << "4. Опубликовать TextMessage в шину" << endl;
        cout << "0. Выход" << endl;
        cout << "Ваш выбор: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            bus.subscribe(new MessageProcessor<ConsoleLoggingPolicy>());
            cout << "Обработчик с ConsoleLoggingPolicy добавлен. Всего подписчиков: " << bus.getCount() << endl;
            break;
        }
        case 2: {
            bus.subscribe(new MessageProcessor<SilentPolicy>());
            cout << "Обработчик с SilentPolicy добавлен. Всего подписчиков: " << bus.getCount() << endl;
            break;
        }
        case 3: {
            bus.subscribe(new JsonHandler());
            cout << "CRTP-обработчик JsonHandler добавлен. Всего подписчиков: " << bus.getCount() << endl;
            break;
        }
        case 4: {
            string text_input = "";
            cout << "Введите текст сообщения: ";
            cin.ignore();
            getline(cin, text_input);
            TextMessage user_msg(text_input);
            cout << "\n--- Реакция подписчиков шины ---" << endl;
            bus.publish(user_msg);
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




