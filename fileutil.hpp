#include <vector>
#include <string>
#include <stdexcept>
#include <cstdint>

std::vector<std::vector<uint8_t>> parseKey128(const std::string& hex);
std::string hexToBytes(const std::string& hex);
std::string bytesToHex(const std::string& bytes);
std::string readFile(const std::string& filename);
void writeFile(const std::string& filename, const std::string& data);
