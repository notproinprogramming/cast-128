#include "fileutil.hpp"
#include <fstream>


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


std::string hexToBytes(const std::string& hex) {
  std::string bytes;
    for (size_t i = 0; i < hex.size(); i += 2) {
        uint8_t byte = static_cast<uint8_t>(stoi(hex.substr(i, 2), nullptr, 16));
        bytes.push_back(static_cast<char>(byte));
    }
    return bytes;
}

std::string bytesToHex(const std::string& bytes) {
    const char* table = "0123456789ABCDEF";
    std::string result;
    for (unsigned char c : bytes) {
        result.push_back(table[c >> 4]);  
        result.push_back(table[c & 0xF]);
    }
    return result;
}


std::string readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary); 
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    return std::string(std::istreambuf_iterator<char>(file),
                       std::istreambuf_iterator<char>());
}

void writeFile(const std::string& filename, const std::string& data) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    file.write(data.data(), data.size());
}
