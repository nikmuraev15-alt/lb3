#include "routeCipher.h"
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <vector>
#include <cctype>
#include <sstream>
#include <limits>

// Конструктор: инициализация ключа
RouteCipher::RouteCipher(int key)
{
    columns = getValidKey(key);
}

// Метод шифрования
std::string RouteCipher::encrypt(const std::string& text)
{
    std::string validText = getValidOpenText(text);
    
    // Дополнительная проверка соотношения ключа и длины текста
    validateKeyTextRatio(validText.size());
    
    int original_length = validText.size();
    int rows = (original_length + columns - 1) / columns;

    // Создаем таблицу и заполняем ее по строкам
    std::vector<std::vector<char>> table(rows, std::vector<char>(columns, ' '));
    int index = 0;
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (index < original_length) {
                table[i][j] = validText[index++];
            }
        }
    }

    // Читаем по столбцам справа налево, сверху вниз
    std::string result;
    for (int j = columns - 1; j >= 0; j--) {
        for (int i = 0; i < rows; i++) {
            if (table[i][j] != ' ') {
                result += table[i][j];
            }
        }
    }
    
    return result;
}

// Метод дешифрования
std::string RouteCipher::decrypt(const std::string& text)
{
    std::string validText = getValidCipherText(text);
    
    // Дополнительная проверка соотношения ключа и длины текста
    validateKeyTextRatio(validText.size());
    
    int encrypted_length = validText.size();
    int rows = (encrypted_length + columns - 1) / columns;
    
    // Восстанавливаем оригинальную длину текста
    int original_length = encrypted_length;
    
    // Создаем таблицу для дешифрования
    std::vector<std::vector<char>> table(rows, std::vector<char>(columns, ' '));
    
    // Сначала определяем, какие ячейки были заполнены в оригинальной таблице
    std::vector<std::vector<bool>> filled(rows, std::vector<bool>(columns, false));
    int original_index = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (original_index < original_length) {
                filled[i][j] = true;
                original_index++;
            }
        }
    }
    
    // Заполняем таблицу по столбцам справа налево, сверху вниз
    int encrypted_index = 0;
    for (int j = columns - 1; j >= 0; j--) {
        for (int i = 0; i < rows; i++) {
            if (filled[i][j] && encrypted_index < encrypted_length) {
                table[i][j] = validText[encrypted_index++];
            }
        }
    }

    // Читаем таблицу по строкам слева направо, сверху вниз
    std::string result;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (filled[i][j]) {
                result += table[i][j];
            }
        }
    }
    
    return result;
}

// Валидация ключа
int RouteCipher::getValidKey(int key)
{
    if (key <= 0) {
        throw cipher_error("Ключ должен быть положительным числом");
    }
    if (key > 50) { // Уменьшил максимальный ключ для безопасности
        throw cipher_error("Слишком большой ключ (максимум 50)");
    }
    if (key == 1) {
        throw cipher_error("Ключ не может быть равен 1 (отсутствует перестановка)");
    }
    return key;
}

// Проверка соотношения ключа и длины текста
void RouteCipher::validateKeyTextRatio(int textLength)
{
    if (columns >= textLength) {
        throw cipher_error("Ключ не должен быть больше или равен длине текста");
    }
    
    // Рекомендуемое соотношение: ключ ≤ 1/3 длины текста для хорошей криптостойкости
    int recommendedMax = textLength / 3;
    if (columns > recommendedMax && textLength > 10) {
        throw cipher_error("Для лучшей безопасности ключ должен быть не больше 1/3 длины текста. "
                          "Максимум для этого текста: " + std::to_string(recommendedMax));
    }
    
    // Минимальное количество строк должно быть хотя бы 2 для нормального перемешивания
    int rows = (textLength + columns - 1) / columns;
    if (rows < 2) {
        throw cipher_error("Слишком большой ключ для данного текста. "
                          "Уменьшите ключ чтобы получить хотя бы 2 строки в таблице");
    }
}

// Валидация открытого текста
std::string RouteCipher::getValidOpenText(const std::string& text)
{
    if (text.empty()) {
        throw cipher_error("Пустой открытый текст");
    }
    
    std::string result;
    
    for (char c : text) {
        if (isalpha(c)) {
            result += toupper(c);
        }
    }
    
    if (result.empty()) {
        throw cipher_error("Текст не содержит английские буквы");
    }
    
    // Минимальная длина текста для данного шифра
    if (result.length() < 4) {
        throw cipher_error("Текст слишком короткий для шифрования (минимум 4 символа)");
    }
    
    return result;
}

// Валидация шифротекста
std::string RouteCipher::getValidCipherText(const std::string& text)
{
    if (text.empty()) {
        throw cipher_error("Пустой шифротекст");
    }
    
    for (char c : text) {
        if (!isalpha(c)) {
            throw cipher_error("Шифротекст содержит недопустимые символы(должны быть только английские буквы)");
        }
        if (islower(c)) {
            throw cipher_error("Шифротекст должен содержать только прописные буквы");
        }
    }
    
    // Минимальная длина шифротекста
    if (text.length() < 4) {
        throw cipher_error("Шифротекст слишком короткий");
    }
    
    return text;
}
