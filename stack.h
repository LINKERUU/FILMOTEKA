#ifndef STACK_H
#define STACK_H

#include <stack>
#include <stdexcept>

template <typename T>
class Stack {
public:
    // Добавить элемент в стек
    void add(const T& item);

    // Получить верхний элемент
    T front() const;

    // Проверить, есть ли еще элементы
    bool hasNext() const;

    // Удалить верхний элемент (переход к следующему)
    void next();

    // Получить количество элементов в стеке
    size_t size() const;

    size_t reset();

private:
    std::stack<T> items; // Внутренний стек
};

// Реализация шаблонных методов должна быть в этом же файле

template <typename T>
void Stack<T>::add(const T& item) {
    items.push(item);
}

template <typename T>
T Stack<T>::front() const {
    if (items.empty()) {
        throw std::out_of_range("Stack is empty");
    }
    return items.top();
}

template <typename T>
bool Stack<T>::hasNext() const {
    return !items.empty();
}

template <typename T>
void Stack<T>::next() {
    if (items.empty()) {
        throw std::out_of_range("No more elements");
    }
    items.pop();
}

template <typename T>
size_t Stack<T>::size() const {
    return items.size();
}

template <typename T>
size_t Stack<T>::reset() {
    return items.empty();
}

#endif // STACK_H
