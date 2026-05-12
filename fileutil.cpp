#include "fileutil.hpp"

std::vector<std::vector<uint8_t>> parseKey128(const std::string& hex) {
    if (hex.size() != 32) {
        throw std::invalid_argument("Key must be 32 hex characters (128 bits)");
    }

    std::vector<uint8_t> bytes;
    bytes.reserve(16);

    for (size_t i = 0; i < hex.size(); i += 2) {
        std::string byteStr = hex.substr(i, 2);

        // перевірка на валідний hex
        if (!isxdigit(byteStr[0]) || !isxdigit(byteStr[1])) {
            throw std::invalid_argument("Invalid hex character in key");
        }

        uint8_t byte = static_cast<uint8_t>(std::stoi(byteStr, nullptr, 16));
        bytes.push_back(byte);
    }

    // формуємо 4x4
    std::vector<std::vector<uint8_t>> key(4, std::vector<uint8_t>(4));

    for (int i = 0; i < 16; ++i) {
        key[i / 4][i % 4] = bytes[i];
    }

    return key;
}
