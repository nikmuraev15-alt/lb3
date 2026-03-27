#include <UnitTest++/UnitTest++.h>
#include <iostream>
#include "routeCipher.h"

// ============================================================================
// ТЕСТЫ ДЛЯ КОНСТРУКТОРА
// ============================================================================

SUITE(ConstructorTest)
{
    // Позитивные тесты - корректные ключи
    TEST(ValidKey) {
        RouteCipher cipher(5);
        CHECK(true);
    }
    
    TEST(ValidKeyMinimum) {
        RouteCipher cipher(2);
        CHECK(true);
    }
    
    TEST(ValidKeyMaximum) {
        RouteCipher cipher(50);
        CHECK(true);
    }
    
    TEST(ValidKeyMedium) {
        RouteCipher cipher(10);
        CHECK(true);
    }
    
    // Негативные тесты - невалидные ключи
    TEST(ZeroKey) {
        CHECK_THROW(RouteCipher cipher(0), cipher_error);
    }
    
    TEST(NegativeKey) {
        CHECK_THROW(RouteCipher cipher(-5), cipher_error);
    }
    
    TEST(KeyEqualTo1) {
        CHECK_THROW(RouteCipher cipher(1), cipher_error);
    }
    
    TEST(TooLargeKey) {
        CHECK_THROW(RouteCipher cipher(51), cipher_error);
    }
    
    TEST(VeryLargeKey) {
        CHECK_THROW(RouteCipher cipher(100), cipher_error);
    }
}

// ============================================================================
// ТЕСТЫ ДЛЯ МЕТОДА ENCRYPT
// ============================================================================

SUITE(EncryptTest)
{
    // Фикстура для тестов с ключом 3
    struct Fixture3 {
        RouteCipher cipher;
        Fixture3() : cipher(3) {}
    };
    
    // Фикстура для тестов с ключом 4
    struct Fixture4 {
        RouteCipher cipher;
        Fixture4() : cipher(4) {}
    };
    
    // Фикстура для тестов с ключом 2
    struct Fixture2 {
        RouteCipher cipher;
        Fixture2() : cipher(2) {}
    };
    
    // Позитивные тесты шифрования
    TEST_FIXTURE(Fixture3, EncryptEnglishText) {
        std::string encrypted = cipher.encrypt("HELLOWORLD");
        CHECK(!encrypted.empty());
        CHECK(encrypted != "HELLOWORLD");
    }
    
    TEST_FIXTURE(Fixture3, EncryptMixedCase) {
        std::string encrypted = cipher.encrypt("HelloWorld");
        CHECK(!encrypted.empty());
    }
    
    TEST_FIXTURE(Fixture3, EncryptWithSpaces) {
        std::string encrypted = cipher.encrypt("HELLO WORLD");
        CHECK(!encrypted.empty());
    }
    
    TEST_FIXTURE(Fixture3, EncryptWithPunctuation) {
        std::string encrypted = cipher.encrypt("HELLO, WORLD!");
        CHECK(!encrypted.empty());
    }
    
    // ИСПРАВЛЕНО: используем ключ 3 вместо 4 для текста длиной 11 символов
    TEST(EncryptShortTextWithProperKey) {
        RouteCipher cipher3(3); // 11/3 = 3.6 -> ключ 3 допустим
        std::string encrypted = cipher3.encrypt("TESTINGWORD");
        CHECK(!encrypted.empty());
    }
    
    TEST_FIXTURE(Fixture2, EncryptLongText) {
        std::string encrypted = cipher.encrypt("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
        CHECK(!encrypted.empty());
    }
    
    TEST_FIXTURE(Fixture3, EncryptExactMultiple) {
        std::string encrypted = cipher.encrypt("ABCDEFGHI");
        CHECK(!encrypted.empty());
    }
    
    TEST_FIXTURE(Fixture3, EncryptNotExactMultiple) {
        std::string encrypted = cipher.encrypt("ABCDEFGHIJKLMN");
        CHECK(!encrypted.empty());
    }
    
    // Негативные тесты шифрования
    TEST_FIXTURE(Fixture3, EncryptEmptyText) {
        CHECK_THROW(cipher.encrypt(""), cipher_error);
    }
    
    TEST_FIXTURE(Fixture3, EncryptOnlySpaces) {
        CHECK_THROW(cipher.encrypt("     "), cipher_error);
    }
    
    TEST_FIXTURE(Fixture3, EncryptOnlyPunctuation) {
        CHECK_THROW(cipher.encrypt("!@#$%^&*()"), cipher_error);
    }
    
    TEST_FIXTURE(Fixture3, EncryptOnlyDigits) {
        CHECK_THROW(cipher.encrypt("1234567890"), cipher_error);
    }
    
    TEST_FIXTURE(Fixture3, EncryptTooShortText) {
        CHECK_THROW(cipher.encrypt("ABC"), cipher_error);
    }
    
    TEST_FIXTURE(Fixture3, EncryptRussianText) {
        CHECK_THROW(cipher.encrypt("ПРИВЕТ"), cipher_error);
    }
    
    TEST_FIXTURE(Fixture3, EncryptKeyTooLargeForText) {
        RouteCipher bigCipher(10);
        CHECK_THROW(bigCipher.encrypt("HELLO"), cipher_error);
    }
    
    TEST_FIXTURE(Fixture3, EncryptKeyEqualToTextLength) {
        RouteCipher bigCipher(5);
        CHECK_THROW(bigCipher.encrypt("HELLO"), cipher_error);
    }
}

// ============================================================================
// ТЕСТЫ ДЛЯ МЕТОДА DECRYPT
// ============================================================================

SUITE(DecryptTest)
{
    // Фикстура для тестов с ключом 3
    struct Fixture3 {
        RouteCipher cipher;
        Fixture3() : cipher(3) {}
    };
    
    // Фикстура для тестов с ключом 2
    struct Fixture2 {
        RouteCipher cipher;
        Fixture2() : cipher(2) {}
    };
    
    // Позитивные тесты дешифрования
    TEST_FIXTURE(Fixture3, DecryptEnglishText) {
        std::string decrypted = cipher.decrypt("LOLREWHOD");
        CHECK(!decrypted.empty());
    }
    
    // ИСПРАВЛЕНО: используем ключ 3 для текста длиной 11 символов
    TEST(DecryptShortTextWithProperKey) {
        RouteCipher cipher3(3);
        std::string original = "TESTINGWORD";
        std::string encrypted = cipher3.encrypt(original);
        std::string decrypted = cipher3.decrypt(encrypted);
        CHECK(original == decrypted);
    }
    
    TEST_FIXTURE(Fixture2, DecryptLongText) {
        std::string decrypted = cipher.decrypt("BDFHJLNPRTVXZACEGIKMOQSUWY");
        CHECK(!decrypted.empty());
    }
    
    // Тесты шифрование-дешифрование (полный цикл)
    TEST_FIXTURE(Fixture3, EncryptDecryptCycle) {
        std::string original = "HELLOWORLD";
        std::string encrypted = cipher.encrypt(original);
        std::string decrypted = cipher.decrypt(encrypted);
        CHECK(original == decrypted);
    }
    
    TEST_FIXTURE(Fixture3, EncryptDecryptWithSpaces) {
        std::string original = "HELLO WORLD";
        std::string encrypted = cipher.encrypt(original);
        std::string decrypted = cipher.decrypt(encrypted);
        CHECK("HELLOWORLD" == decrypted);
    }
    
    TEST_FIXTURE(Fixture3, EncryptDecryptMixedCase) {
        std::string original = "HelloWorld";
        std::string encrypted = cipher.encrypt(original);
        std::string decrypted = cipher.decrypt(encrypted);
        CHECK("HELLOWORLD" == decrypted);
    }
    
    TEST(EncryptDecryptExactMultiple) {
        RouteCipher cipher4(4);
        std::string original = "ABCDEFGHIJKL";
        std::string encrypted = cipher4.encrypt(original);
        std::string decrypted = cipher4.decrypt(encrypted);
        CHECK(original == decrypted);
    }
    
    TEST(EncryptDecryptNotExactMultiple) {
        RouteCipher cipher4(4);
        std::string original = "ABCDEFGHIJKLMNOP";
        std::string encrypted = cipher4.encrypt(original);
        std::string decrypted = cipher4.decrypt(encrypted);
        CHECK(original == decrypted);
    }
    
    TEST_FIXTURE(Fixture3, EncryptDecryptLongTextCycle) {
        std::string original = "THEQUICKBROWNFOXJUMPSOVERTHELAZYDOG";
        std::string encrypted = cipher.encrypt(original);
        std::string decrypted = cipher.decrypt(encrypted);
        CHECK(original == decrypted);
    }
    
    // Негативные тесты дешифрования
    TEST_FIXTURE(Fixture3, DecryptEmptyText) {
        CHECK_THROW(cipher.decrypt(""), cipher_error);
    }
    
    TEST_FIXTURE(Fixture3, DecryptLowercaseText) {
        CHECK_THROW(cipher.decrypt("hello"), cipher_error);
    }
    
    TEST_FIXTURE(Fixture3, DecryptWithSpaces) {
        CHECK_THROW(cipher.decrypt("HEL LO"), cipher_error);
    }
    
    TEST_FIXTURE(Fixture3, DecryptWithDigits) {
        CHECK_THROW(cipher.decrypt("HELLO123"), cipher_error);
    }
    
    TEST_FIXTURE(Fixture3, DecryptWithPunctuation) {
        CHECK_THROW(cipher.decrypt("HELLO,WORLD!"), cipher_error);
    }
    
    TEST_FIXTURE(Fixture3, DecryptTooShortText) {
        CHECK_THROW(cipher.decrypt("ABC"), cipher_error);
    }
    
    TEST_FIXTURE(Fixture3, DecryptRussianText) {
        CHECK_THROW(cipher.decrypt("ПРИВЕТ"), cipher_error);
    }
    
    TEST_FIXTURE(Fixture3, DecryptKeyTooLargeForText) {
        RouteCipher bigCipher(10);
        CHECK_THROW(bigCipher.decrypt("HELLO"), cipher_error);
    }
}

// ============================================================================
// ТЕСТЫ ДЛЯ ВАЛИДАЦИИ КЛЮЧА И ТЕКСТА
// ============================================================================

SUITE(ValidationTest)
{
    // ИСПРАВЛЕНО: используем ключ, который точно нарушает правило 1/3
    TEST(KeyTextRatioValidation) {
        RouteCipher cipher(8);
        // Для текста длиной 15, максимальный рекомендуемый ключ = 15/3 = 5
        // Ключ 8 > 5, поэтому должно быть исключение
        CHECK_THROW(cipher.encrypt("ABCDEFGHIJKLMNO"), cipher_error);
    }
    
    TEST(MinimumRowsValidation) {
        RouteCipher cipher(6);
        std::string encrypted = cipher.encrypt("ABCDEFG");
        CHECK(!encrypted.empty());
    }
    
    TEST(SingleRowCase) {
        RouteCipher cipher(8);
        CHECK_THROW(cipher.encrypt("ABCDEFGH"), cipher_error);
    }
}

// ============================================================================
// ТЕСТЫ ДЛЯ КОНКРЕТНЫХ АЛГОРИТМОВ
// ============================================================================

SUITE(AlgorithmTest)
{
    TEST(EncryptDecryptConsistency) {
        RouteCipher cipher(3);
        std::string testString = "ABCDEFGHI";
        std::string encrypted = cipher.encrypt(testString);
        std::string decrypted = cipher.decrypt(encrypted);
        CHECK(testString == decrypted);
    }
    
    TEST(DifferentKeysProduceDifferentResults) {
        RouteCipher cipher3(3);
        RouteCipher cipher4(4);
        std::string original = "HELLOWORLD";
        
        std::string encrypted3 = cipher3.encrypt(original);
        std::string encrypted4 = cipher4.encrypt(original);
        
        CHECK(encrypted3 != encrypted4);
    }
}

// ============================================================================
// ГЛАВНАЯ ФУНКЦИЯ
// ============================================================================

int main() {
    std::cout << "=== ТЕСТИРОВАНИЕ ROUTECIPHER ===" << std::endl;
    
    int result = UnitTest::RunAllTests();
    
    std::cout << "\n=== РЕЗУЛЬТАТЫ ТЕСТИРОВАНИЯ ===" << std::endl;
    if (result == 0) {
        std::cout << "✅ ВСЕ ТЕСТЫ ПРОЙДЕНЫ" << std::endl;
    } else {
        std::cout << "❌ ЕСТЬ ОШИБКИ В ТЕСТАХ" << std::endl;
    }
    
    return result;
}
