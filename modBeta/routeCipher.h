#pragma once
#include <string>
#include <vector>
#include <stdexcept>

// Класс-исключение для ошибок шифрования
class cipher_error : public std::invalid_argument {
public:
    explicit cipher_error(const std::string& what_arg) : 
        std::invalid_argument(what_arg) {}
    explicit cipher_error(const char* what_arg) : 
        std::invalid_argument(what_arg) {}
};

class RouteCipher
{
private:
    int columns;  // Количество столбцов (ключ)
    
    // Валидация ключа
    int getValidKey(int key);
    
    // Проверка соотношения ключа и текста
    void validateKeyTextRatio(int textLength);
    
    // Валидация текста
    std::string getValidOpenText(const std::string& text);
    std::string getValidCipherText(const std::string& text);
    
public:
    RouteCipher() = delete;  // Запрет конструктора по умолчанию
    
    // Конструктор с ключом
    RouteCipher(int key);
    
    // Методы шифрования/дешифрования
    std::string encrypt(const std::string& text);   // Шифрование
    std::string decrypt(const std::string& text);   // Дешифрование
};
