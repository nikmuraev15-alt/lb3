#include "modAlphaCipher.h"
#include <iostream>
#include <cwctype>

// Конструктор
modAlphaCipher::modAlphaCipher(const std::wstring& skey)
{
    // Инициализируем алфавит (33 буквы С Ё)
    for (size_t i = 0; i < numAlpha.size(); i++) {
        alphaNum[numAlpha[i]] = i;
    }
    key = convert(getValidKey(skey));
}

// Шифрование
std::wstring modAlphaCipher::encrypt(const std::wstring& open_text)
{
    std::wstring valid_text = getValidOpenText(open_text);
    std::vector<int> work = convert(valid_text);
    int alpha_size = numAlpha.size();
    
    for (size_t i = 0; i < work.size(); i++) {
        int shift = key[i % key.size()];
        work[i] = (work[i] + shift + 1) % alpha_size;
    }
    return convert(work);
}

// Дешифрование  
std::wstring modAlphaCipher::decrypt(const std::wstring& cipher_text)
{
    std::wstring valid_text = getValidCipherText(cipher_text);
    std::vector<int> work = convert(valid_text);
    int alpha_size = numAlpha.size();
    
    for (size_t i = 0; i < work.size(); i++) {
        int shift = key[i % key.size()];
        work[i] = (work[i] - shift - 1 + alpha_size * 2) % alpha_size;
    }
    return convert(work);
}

std::vector<int> modAlphaCipher::convert(const std::wstring& s)
{
    std::vector<int> result;
    for (auto c : s) {
        if (c != L' ') {
            result.push_back(alphaNum[c]);
        }
    }
    return result;
}

std::wstring modAlphaCipher::convert(const std::vector<int>& v)
{
    std::wstring result;
    for (auto i : v) {
        result.push_back(numAlpha[i]);
    }
    return result;
}
// Валидация ключа
std::wstring modAlphaCipher::getValidKey(const std::wstring& s)
{
    if (s.empty()) {
        throw cipher_error("Пустой ключ");
    }
    
    std::wstring tmp;
    for (auto c : s) {
        if (iswspace(c)) {
            throw cipher_error("Ключ содержит пробелы");
        }
        if (iswdigit(c)) {
            throw cipher_error("Ключ содержит цифры");
        }
        if (iswpunct(c)) {
            throw cipher_error("Ключ содержит знаки пунктуации");
        }
        if (iswalpha(c)) {
            wchar_t upper_c = towupper(c);
            if (alphaNum.find(upper_c) != alphaNum.end()) {
                tmp.push_back(upper_c);
            } else {
                throw cipher_error("Ключ содержит не русские буквы");
            }
        } else {
            throw cipher_error("Ключ содержит недопустимые символы");
        }
    }
    
    if (tmp.empty()) {
        throw cipher_error("Ключ не содержит русских букв");
    }
    
    return tmp;
}
// Валидация открытого текста
std::wstring modAlphaCipher::getValidOpenText(const std::wstring& s)
{
    if (s.empty()) {
        throw cipher_error("Пустой открытый текст");
    }
    
    std::wstring tmp;
    for (auto c : s) {
        if (iswalpha(c)) {
            wchar_t upper_c = towupper(c);
            if (alphaNum.find(upper_c) != alphaNum.end()) {
                tmp.push_back(upper_c);
            } else {
                throw cipher_error("Текст содержит не русские буквы");
            }
        } else if (iswspace(c)) {
            tmp.push_back(c);
        } else {
            throw cipher_error("Текст содержит недопустимые символы (только русские буквы и пробелы)");
        }
    }
    
    bool has_letters = false;
    for (auto c : tmp) {
        if (iswalpha(c)) {
            has_letters = true;
            break;
        }
    }
    
    if (!has_letters) {
        throw cipher_error("Текст не содержит русских букв");
    }
    
    return tmp;
}
// Валидация зашифрованного текста
std::wstring modAlphaCipher::getValidCipherText(const std::wstring& s)
{
    if (s.empty()) {
        throw cipher_error("Пустой шифротекст");
    }
    
    for (auto c : s) {
        if (iswspace(c)) {
            throw cipher_error("Шифротекст содержит пробелы");
        }
        if (alphaNum.find(c) == alphaNum.end()) {
            throw cipher_error("Шифротекст содержит недопустимые символы");
        }
    }
    
    return s;
}
