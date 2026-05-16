// робота з файлами
#include <vector>
#include <string>
#include <stdexcept>
#include <cstdint>
std::vector<std::vector<uint8_t>> parseKey128(const std::string& hex);
std::string hexToBytes(const std::string& hex);
std::string bytesToHex(const std::string& bytes);
