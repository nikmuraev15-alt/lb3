#include <UnitTest++/UnitTest++.h>
#include <iostream>
#include <locale>
#include <codecvt>
#include "modAlphaCipher.h"

// ============================================================================
// ТЕСТЫ ДЛЯ КОНСТРУКТОРА
// ============================================================================

SUITE(ConstructorTest)
{
    // Позитивные тесты - корректные ключи
    TEST(ValidRussianKey) {
        modAlphaCipher cipher(L"ПРИВЕТ");
        CHECK(true); // Если объект создался - тест пройден
    }
    
    TEST(ValidRussianKeyMixedCase) {
        modAlphaCipher cipher(L"ПрИвЕт");
        CHECK(true);
    }
    
    TEST(ValidSingleCharKey) {
        modAlphaCipher cipher(L"А");
        CHECK(true);
    }
    
    TEST(ValidLongKey) {
        modAlphaCipher cipher(L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ");
        CHECK(true);
    }
    
    // Негативные тесты - невалидные ключи
    TEST(EmptyKey) {
        CHECK_THROW(modAlphaCipher cipher(L""), cipher_error);
    }
    
    TEST(KeyWithSpaces) {
        CHECK_THROW(modAlphaCipher cipher(L"ПРИВЕТ МИР"), cipher_error);
    }
    
    TEST(KeyWithDigits) {
        CHECK_THROW(modAlphaCipher cipher(L"ПРИВЕТ123"), cipher_error);
    }
    
    TEST(KeyWithPunctuation) {
        CHECK_THROW(modAlphaCipher cipher(L"ПРИВЕТ,МИР"), cipher_error);
    }
    
    TEST(KeyWithEnglishLetters) {
        CHECK_THROW(modAlphaCipher cipher(L"HELLO"), cipher_error);
    }
    
    TEST(KeyWithNoRussianLetters) {
        CHECK_THROW(modAlphaCipher cipher(L"123!@#"), cipher_error);
    }
}

// ============================================================================
// ТЕСТЫ ДЛЯ МЕТОДА ENCRYPT
// ============================================================================

SUITE(EncryptTest)
{
    // Фикстура для тестов с ключом "ПРИВЕТ"
    struct FixturePRIVET {
        modAlphaCipher cipher;
        FixturePRIVET() : cipher(L"ПРИВЕТ") {}
    };
    
    // Фикстура для тестов с ключом "А" (максимальный сдвиг)
    struct FixtureA {
        modAlphaCipher cipher;
        FixtureA() : cipher(L"А") {}
    };
    
    // Позитивные тесты шифрования
    TEST_FIXTURE(FixturePRIVET, EncryptRussianText) {
        std::wstring encrypted = cipher.encrypt(L"ПРИВЕТМИР");
        CHECK(encrypted != L"ПРИВЕТМИР"); // Текст должен измениться
    }
    
    TEST_FIXTURE(FixturePRIVET, EncryptMixedCase) {
        std::wstring encrypted = cipher.encrypt(L"ПриветМир");
        CHECK(!encrypted.empty());
    }
    
    TEST_FIXTURE(FixturePRIVET, EncryptWithSpaces) {
        // Пробелы должны игнорироваться
        std::wstring encrypted = cipher.encrypt(L"ПРИВЕТ МИР");
        CHECK(!encrypted.empty());
    }
    
    TEST_FIXTURE(FixtureA, EncryptWithMaxShift) {
        std::wstring encrypted = cipher.encrypt(L"Я");
        CHECK(encrypted == L"А");
    }
    
    TEST_FIXTURE(FixtureA, EncryptAlphabet) {
        std::wstring encrypted = cipher.encrypt(L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ");
        CHECK(!encrypted.empty());
    }
    
    // Негативные тесты шифрования
    TEST_FIXTURE(FixturePRIVET, EncryptEmptyText) {
        CHECK_THROW(cipher.encrypt(L""), cipher_error);
    }
    
    TEST_FIXTURE(FixturePRIVET, EncryptOnlySpaces) {
        CHECK_THROW(cipher.encrypt(L"   "), cipher_error);
    }
    
    TEST_FIXTURE(FixturePRIVET, EncryptWithDigits) {
        CHECK_THROW(cipher.encrypt(L"ПРИВЕТ123"), cipher_error);
    }
    
    TEST_FIXTURE(FixturePRIVET, EncryptWithPunctuation) {
        CHECK_THROW(cipher.encrypt(L"ПРИВЕТ,МИР!"), cipher_error);
    }
    
    TEST_FIXTURE(FixturePRIVET, EncryptEnglishText) {
        CHECK_THROW(cipher.encrypt(L"HELLO"), cipher_error);
    }
}

// ============================================================================
// ТЕСТЫ ДЛЯ МЕТОДА DECRYPT
// ============================================================================

SUITE(DecryptTest)
{
    // Фикстура для тестов с ключом "ПРИВЕТ"
    struct FixturePRIVET {
        modAlphaCipher cipher;
        FixturePRIVET() : cipher(L"ПРИВЕТ") {}
    };
    
    // Фикстура для тестов с ключом "А" (максимальный сдвиг)
    struct FixtureA {
        modAlphaCipher cipher;
        FixtureA() : cipher(L"А") {}
    };
    
    // Позитивные тесты дешифрования
    TEST_FIXTURE(FixturePRIVET, DecryptRussianText) {
        std::wstring decrypted = cipher.decrypt(L"ЙУЖЩФТЁУ");
        CHECK(!decrypted.empty());
    }
    
    TEST_FIXTURE(FixtureA, DecryptWithMaxShift) {
        std::wstring decrypted = cipher.decrypt(L"А");
        CHECK(decrypted == L"Я");
    }
    
    TEST_FIXTURE(FixtureA, DecryptAlphabet) {
        std::wstring decrypted = cipher.decrypt(L"БВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯА");
        CHECK(!decrypted.empty());
    }
    
    // Тесты шифрование-дешифрование (полный цикл)
    TEST_FIXTURE(FixturePRIVET, EncryptDecryptCycle) {
        std::wstring original = L"ПРИВЕТМИР";
        std::wstring encrypted = cipher.encrypt(original);
        std::wstring decrypted = cipher.decrypt(encrypted);
        CHECK(original == decrypted);
    }
    
    TEST_FIXTURE(FixturePRIVET, EncryptDecryptLongText) {
        std::wstring original = L"СЪЕШЬЖЕЕЩЁЭТИХМЯГКИХФРАНЦУЗСКИХБУЛОКДАВЫПЕЙЧАЮ";
        std::wstring encrypted = cipher.encrypt(original);
        std::wstring decrypted = cipher.decrypt(encrypted);
        CHECK(original == decrypted);
    }
    
    // Негативные тесты дешифрования
    TEST_FIXTURE(FixturePRIVET, DecryptEmptyText) {
        CHECK_THROW(cipher.decrypt(L""), cipher_error);
    }
    
    TEST_FIXTURE(FixturePRIVET, DecryptWithSpaces) {
        CHECK_THROW(cipher.decrypt(L"ЙУЖЩ ФТЁУ"), cipher_error);
    }
    
    TEST_FIXTURE(FixturePRIVET, DecryptWithDigits) {
        CHECK_THROW(cipher.decrypt(L"ЙУЖЩ123"), cipher_error);
    }
    
    TEST_FIXTURE(FixturePRIVET, DecryptWithPunctuation) {
        CHECK_THROW(cipher.decrypt(L"ЙУЖЩ,ФТЁУ!"), cipher_error);
    }
    
    TEST_FIXTURE(FixturePRIVET, DecryptEnglishText) {
        CHECK_THROW(cipher.decrypt(L"HELLO"), cipher_error);
    }
}

// ============================================================================
// ТЕСТЫ ДЛЯ ПРЕОБРАЗОВАНИЙ
// ============================================================================

SUITE(ConversionTest)
{
    TEST(ConvertStringToVector) {
        modAlphaCipher cipher(L"А");
        std::wstring test_str = L"АБВ";
        std::wstring encrypted = cipher.encrypt(test_str);
        CHECK(!encrypted.empty());
    }
}

// ============================================================================
// ГЛАВНАЯ ФУНКЦИЯ
// ============================================================================

int main() {
    std::cout << "=== ТЕСТИРОВАНИЕ MODALPHACIPHER ===" << std::endl;
    
    // Устанавливаем локаль для корректного вывода русских букв
    std::locale::global(std::locale("ru_RU.UTF-8"));
    
    int result = UnitTest::RunAllTests();
    
    std::cout << "\n=== РЕЗУЛЬТАТЫ ТЕСТИРОВАНИЯ ===" << std::endl;
    if (result == 0) {
        std::cout << "✅ ВСЕ ТЕСТЫ ПРОЙДЕНЫ" << std::endl;
    } else {
        std::cout << "❌ ЕСТЬ ОШИБКИ В ТЕСТАХ" << std::endl;
    }
    
    return result;
}
