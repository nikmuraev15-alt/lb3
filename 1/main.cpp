#include "modAlphaCipher.h"
#include <iostream>
#include <string>
#include <locale>

using namespace std;

void PrintTestResults() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    cout << "ЛАБОРАТОРНАЯ РАБОТА №3: МОДУЛЬНЫЕ ТЕСТЫ" << endl;
    cout << "Русский шифр Гронсфельда" << endl;
    cout << "========================================" << endl;
    
    int passed = 0;
    int total = 0;
    
    // 1. Тесты конструктора
    cout << "\n1. Тесты конструктора:" << endl;
    
    // 1.1 Валидный ключ
    try {
        total++;
        modAlphaCipher cipher("КЛЮЧ");
        cout << "✓ 1.1 Ключ='КЛЮЧ' - ОК" << endl;
        passed++;
    } catch (...) {
        cout << "✗ 1.1 Ключ='КЛЮЧ' - ОШИБКА" << endl;
    }
    
    // 1.2 Ключ с пробелами
    try {
        total++;
        modAlphaCipher cipher("К Л Ю Ч");
        string result = cipher.encrypt("ТЕСТ");
        cout << "✓ 1.2 Ключ с пробелами - ОК" << endl;
        passed++;
    } catch (const cipher_error& e) {
        cout << "✓ 1.2 Ключ с пробелами - ОК: " << e.what() << endl;
        passed++;
    } catch (...) {
        cout << "✗ 1.2 Ключ с пробелами - ОШИБКА" << endl;
    }
    
    // 1.3 Пустой ключ
    try {
        total++;
        modAlphaCipher cipher("");
        cout << "✗ 1.3 Пустой ключ - ОШИБКА (должно быть исключение)" << endl;
    } catch (const cipher_error& e) {
        cout << "✓ 1.3 Пустой ключ - ОК: " << e.what() << endl;
        passed++;
    } catch (...) {
        cout << "✗ 1.3 Пустой ключ - НЕВЕРНОЕ ИСКЛЮЧЕНИЕ" << endl;
    }
    
    // 1.4 Ключ с английскими буквами
    try {
        total++;
        modAlphaCipher cipher("ABC");
        cout << "✗ 1.4 Ключ 'ABC' - ОШИБКА (должно быть исключение)" << endl;
    } catch (const cipher_error& e) {
        cout << "✓ 1.4 Ключ 'ABC' - ОК: " << e.what() << endl;
        passed++;
    } catch (...) {
        cout << "✗ 1.4 Ключ 'ABC' - НЕВЕРНОЕ ИСКЛЮЧЕНИЕ" << endl;
    }
    
    // 2. Тесты шифрования
    cout << "\n2. Тесты шифрования:" << endl;
    
    // 2.1 Простое шифрование
    try {
        total++;
        modAlphaCipher cipher("АБВ");
        string result = cipher.encrypt("ПРИВЕТ");
        cout << "✓ 2.1 Encrypt('ПРИВЕТ') - ОК" << endl;
        passed++;
    } catch (const cipher_error& e) {
        cout << "✗ 2.1 Encrypt('ПРИВЕТ') - ОШИБКА: " << e.what() << endl;
    }
    
    // 2.2 Текст с пробелами
    try {
        total++;
        modAlphaCipher cipher("КЛЮЧ");
        string textWithSpaces = "ПРИВЕТ МИР";
        string textWithoutSpaces = "ПРИВЕТМИР";
        
        string encrypted1 = cipher.encrypt(textWithSpaces);
        string encrypted2 = cipher.encrypt(textWithoutSpaces);
        
        if (encrypted1 == encrypted2) {
            cout << "✓ 2.2 Текст с пробелами - ОК (пробелы удалены)" << endl;
            passed++;
        } else {
            cout << "✗ 2.2 Текст с пробелами - ОШИБКА" << endl;
        }
    } catch (const cipher_error& e) {
        cout << "✗ 2.2 Текст с пробелами - ОШИБКА: " << e.what() << endl;
    }
    
    // 2.3 Текст короче ключа
    try {
        total++;
        modAlphaCipher cipher("ДЛИННЫЙКЛЮЧ");
        string result = cipher.encrypt("АБ");
        cout << "✓ 2.3 Текст короче ключа - ОК" << endl;
        passed++;
    } catch (const cipher_error& e) {
        cout << "✗ 2.3 Текст короче ключа - ОШИБКА: " << e.what() << endl;
    }
    
    // 2.4 Пустой текст
    try {
        total++;
        modAlphaCipher cipher("КЛЮЧ");
        cipher.encrypt("");
        cout << "✗ 2.4 Пустой текст - ОШИБКА (должно быть исключение)" << endl;
    } catch (const cipher_error& e) {
        cout << "✓ 2.4 Пустой текст - ОК: " << e.what() << endl;
        passed++;
    } catch (...) {
        cout << "✗ 2.4 Пустой текст - НЕВЕРНОЕ ИСКЛЮЧЕНИЕ" << endl;
    }
    
    // 2.5 Текст с английскими буквами
    try {
        total++;
        modAlphaCipher cipher("КЛЮЧ");
        cipher.encrypt("HELLO");
        cout << "✗ 2.5 Текст 'HELLO' - ОШИБКА (должно быть исключение)" << endl;
    } catch (const cipher_error& e) {
        cout << "✓ 2.5 Текст 'HELLO' - ОК: " << e.what() << endl;
        passed++;
    } catch (...) {
        cout << "✗ 2.5 Текст 'HELLO' - НЕВЕРНОЕ ИСКЛЮЧЕНИЕ" << endl;
    }
    
    // 3. Тесты дешифрования
    cout << "\n3. Тесты дешифрования:" << endl;
    
    // 3.1 Полный цикл шифрование-дешифрование
    try {
        total++;
        modAlphaCipher cipher("СЕКРЕТ");
        string original = "ПРИВЕТМИР";
        string encrypted = cipher.encrypt(original);
        string decrypted = cipher.decrypt(encrypted);
        
        if (decrypted == original) {
            cout << "✓ 3.1 Полный цикл - ОК" << endl;
            passed++;
        } else {
            cout << "✗ 3.1 Полный цикл - ОШИБКА" << endl;
        }
    } catch (const cipher_error& e) {
        cout << "✗ 3.1 Полный цикл - ОШИБКА: " << e.what() << endl;
    }
    
    // 3.2 Дешифрование с нулевым ключом (ААА)
    try {
        total++;
        modAlphaCipher cipher("ААА");
        string encrypted = cipher.encrypt("ТЕКСТ");
        string decrypted = cipher.decrypt(encrypted);
        
        if (decrypted == "ТЕКСТ") {
            cout << "✓ 3.2 Ключ 'ААА' - ОК" << endl;
            passed++;
        } else {
            cout << "✗ 3.2 Ключ 'ААА' - ОШИБКА" << endl;
        }
    } catch (const cipher_error& e) {
        cout << "✗ 3.2 Ключ 'ААА' - ОШИБКА: " << e.what() << endl;
    }
    
    // 3.3 Дешифрование недопустимого текста
    try {
        total++;
        modAlphaCipher cipher("КЛЮЧ");
        cipher.decrypt("ABC");
        cout << "✗ 3.3 Дешифрование 'ABC' - ОШИБКА (должно быть исключение)" << endl;
    } catch (const cipher_error& e) {
        cout << "✓ 3.3 Дешифрование 'ABC' - ОК: " << e.what() << endl;
        passed++;
    } catch (...) {
        cout << "✗ 3.3 Дешифрование 'ABC' - НЕВЕРНОЕ ИСКЛЮЧЕНИЕ" << endl;
    }
    
    // 3.4 Пустой шифротекст
    try {
        total++;
        modAlphaCipher cipher("КЛЮЧ");
        cipher.decrypt("");
        cout << "✗ 3.4 Пустой шифротекст - ОШИБКА (должно быть исключение)" << endl;
    } catch (const cipher_error& e) {
        cout << "✓ 3.4 Пустой шифротекст - ОК: " << e.what() << endl;
        passed++;
    } catch (...) {
        cout << "✗ 3.4 Пустой шифротекст - НЕВЕРНОЕ ИСКЛЮЧЕНИЕ" << endl;
    }
    
    // 4. Пограничные случаи
    cout << "\n4. Пограничные случаи:" << endl;
    
    // 4.1 Ключ из одного символа
    try {
        total++;
        modAlphaCipher cipher("Б");
        string encrypted = cipher.encrypt("А");
        string decrypted = cipher.decrypt(encrypted);
        
        if (decrypted == "А") {
            cout << "✓ 4.1 Ключ из одного символа - ОК" << endl;
            passed++;
        } else {
            cout << "✗ 4.1 Ключ из одного символа - ОШИБКА" << endl;
        }
    } catch (const cipher_error& e) {
        cout << "✗ 4.1 Ключ из одного символа - ОШИБКА: " << e.what() << endl;
    }
    
    // 4.2 Максимальный сдвиг
    try {
        total++;
        modAlphaCipher cipher("Я");
        string encrypted = cipher.encrypt("А");
        string decrypted = cipher.decrypt(encrypted);
        
        if (decrypted == "А") {
            cout << "✓ 4.2 Максимальный сдвиг - ОК" << endl;
            passed++;
        } else {
            cout << "✗ 4.2 Максимальный сдвиг - ОШИБКА" << endl;
        }
    } catch (const cipher_error& e) {
        cout << "✗ 4.2 Максимальный сдвиг - ОШИБКА: " << e.what() << endl;
    }
    
    // 4.3 Длинный текст с коротким ключом
    try {
        total++;
        modAlphaCipher cipher("АБ");
        string longText = "ТЕКСТТЕКСТТЕКСТТЕКСТТЕКСТТЕКСТ";
        string encrypted = cipher.encrypt(longText);
        string decrypted = cipher.decrypt(encrypted);
        
        if (decrypted == longText) {
            cout << "✓ 4.3 Длинный текст с коротким ключом - ОК" << endl;
            passed++;
        } else {
            cout << "✗ 4.3 Длинный текст с коротким ключом - ОШИБКА" << endl;
        }
    } catch (const cipher_error& e) {
        cout << "✗ 4.3 Длинный текст с коротким ключом - ОШИБКА: " << e.what() << endl;
    }
    
    // Итоги
    cout << "\n========================================" << endl;
    cout << "ИТОГИ ТЕСТИРОВАНИЯ:" << endl;
    cout << "Всего тестов: " << total << endl;
    cout << "Пройдено: " << passed << endl;
    cout << "Не пройдено: " << (total - passed) << endl;
    
    double successRate = (total > 0) ? (passed * 100.0 / total) : 0.0;
    cout << "Успешность: " << successRate << "%" << endl;
    cout << "========================================" << endl;
}

int main() {
    PrintTestResults();
    
    cout << "\nНажмите Enter для выхода..." << endl;
    cin.get();
    
    return 0;
}
