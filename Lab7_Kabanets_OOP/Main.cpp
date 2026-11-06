#include <iostream>
#include <cmath>
#include <exception>
#include <string>
#include <windows.h>


class EmptyComplexException {};

//як незалежний клас із полями-параметрами функції
class ParamComplexException {
private:
    double x, y;
    std::string msg;
public:
    ParamComplexException(double valX, double valY, std::string message)
        : x(valX), y(valY), msg(message) {
    }

    void printDetails() const {
        std::cerr << "Помилка для пара-значень (" << x << ", " << y << "): " << msg << "\n";
    }
};
class StandardDerivedComplexException : public std::exception {
private:
    std::string msg;
    double x, y;
public:
    StandardDerivedComplexException(double valX, double valY, const std::string& message)
        : x(valX), y(valY), msg(message) {
    }

    const char* what() const noexcept override {
        return msg.c_str();
    }
};

// Структури даних 
struct ComplexAlgebraic {
    double x;
    double y;
};

struct ComplexTrigonometric {
    double radius;
    double angle;
};

// Без специфікації виключень
ComplexTrigonometric convertToTrigonometric1(ComplexAlgebraic c) {
    if (c.x == 0.0 && c.y == 0.0) {
        throw StandardDerivedComplexException(c.x, c.y, "Модуль нульового числа невизначений (спадкоємець exception)");
    }
    double r = std::sqrt(c.x * c.x + c.y * c.y);
    double ang = std::atan2(c.y, c.x);
    return { r, ang };
}
// Зі специфікацією throw() (порожній список — не генерує виключень)
ComplexTrigonometric convertToTrigonometric2(ComplexAlgebraic c) throw() {
    double r = std::sqrt(c.x * c.x + c.y * c.y);
    double ang = std::atan2(c.y, c.x);
    return { r, ang };
}

// конкретною специфікацією стандартним виключенням
ComplexTrigonometric convertToTrigonometric3(ComplexAlgebraic c) {
    if (c.x == 0.0 && c.y == 0.0) {
        throw std::invalid_argument("Стандартне виключення: нульові параметри не допускаються.");
    }
    double r = std::sqrt(c.x * c.x + c.y * c.y);
    double ang = std::atan2(c.y, c.x);
    return { r, ang };
}

// Специфікація із власним реалізованим виключенням (використаємо клас з полями)
ComplexTrigonometric convertToTrigonometric4(ComplexAlgebraic c) {
    if (c.x == 0.0 && c.y == 0.0) {
        throw ParamComplexException(c.x, c.y, "Власне виключення з полями: кут невизначений.");
    }
    double r = std::sqrt(c.x * c.x + c.y * c.y);
    double ang = std::atan2(c.y, c.x);
    return { r, ang };
}
// Додаткова функція для демонстрації порожнього власного класу виключення
ComplexTrigonometric convertToTrigonometricEmptyEx(ComplexAlgebraic c) {
    if (c.x == 0.0 && c.y == 0.0) {
        throw EmptyComplexException();
    }
    return { 0, 0 };
}
// Демонстрація обробки
int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    ComplexAlgebraic validNum{ 3.0, 4.0 };
    ComplexAlgebraic invalidNum{ 0.0, 0.0 };

    std::cout << "Демонстрація роботи\n\n";

    // Спадкоємець від стандартного виключення
    try {
        convertToTrigonometric1(invalidNum);
    }
    catch (const StandardDerivedComplexException& e) {
        std::cerr << "Тест 1 Спіймано спадкоємця std::exception: " << e.what() << "\n";
    }

    // Стандартне виключення (std::invalid_argument)
    try {
        convertToTrigonometric3(invalidNum);
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Тест 2 Спіймано стандартне виключення: " << e.what() << "\n";
    }

    //Власне виключення з полями-параметрами
    try {
        convertToTrigonometric4(invalidNum);
    }
    catch (const ParamComplexException& e) {
        std::cerr << "Тест 3 Спіймано кастомний клас з полями:\n  -> ";
        e.printDetails();
    }

    // Власне виключення — порожній клас
    try {
        convertToTrigonometricEmptyEx(invalidNum);
    }
    catch (const EmptyComplexException&) {
        std::cerr << "Тест 4 Спіймано кастомне порожнє виключення (EmptyComplexException).\n";
    }

    // виконання валідного числа
    try {
        auto res = convertToTrigonometric1(validNum);
        std::cout << "\nТест 5 Успіх! (3 + 4i) -> Радіус = " << res.radius << ", Кут = " << res.angle << " рад.\n";
    }
    catch (...) {
        std::cerr << "Помилка у валідному тесті.\n";
    }

    return 0;
}