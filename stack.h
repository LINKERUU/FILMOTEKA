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

    // Итератор
    class Iterator {
    public:
        explicit Iterator(Stack<T>& stack);

        // Разадресация
        T operator*() const;

        // Префиксный инкремент
        Iterator& operator++();

        // Проверка на неравенство
        bool operator!=(const Iterator& other) const;

    private:
        Stack<T>* stackPtr;
    };

    // Создать итератор
    Iterator begin();
    Iterator end();

private:
    std::stack<T> items;
};

// Реализация методов Stack
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

// Реализация методов Iterator
template <typename T>
Stack<T>::Iterator::Iterator(Stack<T>& stack) : stackPtr(&stack) {}

template <typename T>
T Stack<T>::Iterator::operator*() const {
    if (!stackPtr->hasNext()) {
        throw std::out_of_range("No element to dereference");
    }
    return stackPtr->front();
}

template <typename T>
typename Stack<T>::Iterator& Stack<T>::Iterator::operator++() {
    if (!stackPtr->hasNext()) {
        throw std::out_of_range("No more elements to iterate");
    }
    stackPtr->next();
    return *this;
}

template <typename T>
bool Stack<T>::Iterator::operator!=(const Iterator& other) const {
    return stackPtr->hasNext();
}

// Методы для получения итераторов
template <typename T>
typename Stack<T>::Iterator Stack<T>::begin() {
    return Iterator(*this);
}

template <typename T>
typename Stack<T>::Iterator Stack<T>::end() {
    return Iterator(*this); // В конце итерации стек будет пустым
}

#endif // STACK_H
