#ifndef MODALPHACIPHER_H
#define MODALPHACIPHER_H

#include <string>
#include <vector>
#include <stdexcept>

class cipher_error : public std::invalid_argument {
public:
    explicit cipher_error(const std::string& what_arg) : std::invalid_argument(what_arg) {}
    explicit cipher_error(const char* what_arg) : std::invalid_argument(what_arg) {}
};

class modAlphaCipher
{
private:
    const std::string numAlpha = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    std::vector<int> key;

    std::string removeSpaces(const std::string& s) const;
    std::vector<int> textToIndices(const std::string& text) const;
    std::string indicesToText(const std::vector<int>& indices) const;

public:
    modAlphaCipher() = delete;
    modAlphaCipher(const std::string& skey);
    std::string encrypt(const std::string& open_text);
    std::string decrypt(const std::string& cipher_text);
};

#endif
