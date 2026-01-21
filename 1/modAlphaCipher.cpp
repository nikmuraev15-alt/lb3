#include "modAlphaCipher.h"
#include <stdexcept>

std::string modAlphaCipher::removeSpaces(const std::string& s) const
{
    std::string result;
    for (char c : s) {
        if (c != ' ') {
            result += c;
        }
    }
    return result;
}

std::vector<int> modAlphaCipher::textToIndices(const std::string& text) const
{
    std::vector<int> indices;
    size_t i = 0;
    while (i < text.size()) {
        if (i + 1 >= text.size()) break;
        std::string letter = text.substr(i, 2);

        bool found = false;
        for (size_t pos = 0; pos < numAlpha.size(); pos += 2) {
            if (numAlpha.substr(pos, 2) == letter) {
                indices.push_back(static_cast<int>(pos / 2));
                found = true;
                break;
            }
        }

        if (!found) {
            throw cipher_error("Invalid character in input (not a Russian uppercase letter)");
        }

        i += 2;
    }

    if (indices.empty()) {
        throw cipher_error("Empty text after processing");
    }

    return indices;
}

std::string modAlphaCipher::indicesToText(const std::vector<int>& indices) const
{
    std::string result;
    for (int idx : indices) {
        if (idx >= 0 && idx < static_cast<int>(numAlpha.size() / 2)) {
            result += numAlpha.substr(idx * 2, 2);
        }
    }
    return result;
}

modAlphaCipher::modAlphaCipher(const std::string& skey)
{
    std::string cleanKey = removeSpaces(skey);
    key = textToIndices(cleanKey);
}

std::string modAlphaCipher::encrypt(const std::string& open_text)
{
    std::string cleanText = removeSpaces(open_text);
    std::vector<int> work = textToIndices(cleanText);
    int alphabetSize = static_cast<int>(numAlpha.size() / 2);

    for (size_t i = 0; i < work.size(); ++i) {
        work[i] = (work[i] + key[i % key.size()]) % alphabetSize;
    }
    return indicesToText(work);
}

std::string modAlphaCipher::decrypt(const std::string& cipher_text)
{
    std::string cleanText = removeSpaces(cipher_text);
    std::vector<int> work = textToIndices(cleanText);
    int alphabetSize = static_cast<int>(numAlpha.size() / 2);

    for (size_t i = 0; i < work.size(); ++i) {
        work[i] = (work[i] - key[i % key.size()] + alphabetSize) % alphabetSize;
    }
    return indicesToText(work);
}
