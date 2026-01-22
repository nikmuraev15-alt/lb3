#include "RouteCipher.h"
#include <algorithm>
#include <cctype>
#include <locale>
#include <iostream>

RouteCipher::RouteCipher(int Key) {
    ValidateKey(Key);
    Columns = Key;
}

void RouteCipher::ValidateKey(int Key) {
    if (Key <= 0) {
        throw CipherError("Ключ должен быть положительным числом");
    }
    if (Key > 50) {
        throw CipherError("Слишком большой ключ");
    }
}

void RouteCipher::ValidateText(const std::wstring& Text) {
    if (Text.empty()) {
        throw CipherError("Текст не может быть пустым");
    }
}

// Убираем ТОЛЬКО пробелы и табуляции
std::wstring RouteCipher::CleanText(const std::wstring& Text) {
    std::wstring Result;
    for (wchar_t c : Text) {
        // Убираем только пробелы, табуляции, новые строки
        if (c != L' ' && c != L'\t' && c != L'\n' && c != L'\r') {
            Result += c;
        }
    }
    return Result;
}

std::wstring RouteCipher::ToUpperCase(const std::wstring& Text) {
    std::wstring Result = Text;
    
    for (wchar_t& c : Result) {
        // Английские буквы
        if (c >= L'a' && c <= L'z') {
            c = c - L'a' + L'A';
        }
        // Русские буквы
        else if (c >= L'а' && c <= L'я') {
            c = c - L'а' + L'А';
        }
        // Особые русские буквы
        else if (c == L'ё') {
            c = L'Ё';
        }
        // Все остальные символы (цифры, знаки препинания) остаются как есть
    }
    
    return Result;
}

std::wstring RouteCipher::PrepareText(const std::wstring& Text) {
    std::wstring Cleaned = CleanText(Text);
    if (Cleaned.empty()) {
        throw CipherError("После удаления пробелов текст пуст");
    }
    return ToUpperCase(Cleaned);
}

std::wstring RouteCipher::Encrypt(const std::wstring& Text) {
    ValidateText(Text);
    std::wstring PreparedText = PrepareText(Text);
    
    int TextLength = PreparedText.length();
    int Rows = (TextLength + Columns - 1) / Columns;
    
    // Создаем таблицу и заполняем по строкам слева направо
    std::vector<std::vector<wchar_t>> Table(Rows, std::vector<wchar_t>(Columns));
    
    int index = 0;
    for (int i = 0; i < Rows; i++) {
        for (int j = 0; j < Columns; j++) {
            if (index < TextLength) {
                Table[i][j] = PreparedText[index++];
            } else {
                // Заполняем оставшиеся ячейки специальным символом
                Table[i][j] = L'X';
            }
        }
    }
    
    // Читаем по столбцам сверху вниз, справа налево
    std::wstring Result;
    for (int j = Columns - 1; j >= 0; j--) {
        for (int i = 0; i < Rows; i++) {
            Result += Table[i][j];
        }
    }
    
    return Result;
}

std::wstring RouteCipher::Decrypt(const std::wstring& Text) {
    ValidateText(Text);
    std::wstring PreparedText = PrepareText(Text);
    
    int TextLength = PreparedText.length();
    int Rows = (TextLength + Columns - 1) / Columns;
    
    // Создаем пустую таблицу
    std::vector<std::vector<wchar_t>> Table(Rows, std::vector<wchar_t>(Columns));
    
    // Заполняем таблицу по столбцам сверху вниз, справа налево
    int index = 0;
    for (int j = Columns - 1; j >= 0; j--) {
        for (int i = 0; i < Rows; i++) {
            if (index < TextLength) {
                Table[i][j] = PreparedText[index++];
            } else {
                Table[i][j] = L'X';
            }
        }
    }
    
    // Читаем по строкам слева направо
    std::wstring Result;
    for (int i = 0; i < Rows; i++) {
        for (int j = 0; j < Columns; j++) {
            Result += Table[i][j];
        }
    }
    
    // Убираем добавленные символы 'X' в конце
    while (!Result.empty() && Result.back() == L'X') {
        Result.pop_back();
    }
    
    return Result;
}
