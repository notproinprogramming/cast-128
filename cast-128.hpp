#include <cstdint>
#include <vector>
#include <cstdint>
#include <string>
uint32_t circ_shift_left(uint32_t a, uint32_t b);
std::vector<uint8_t> divide_I(uint32_t I);
uint32_t to_int(uint8_t x0, uint8_t x1, uint8_t x2, uint8_t x3);
uint32_t vec_to_int(std::vector<uint8_t> x);

uint32_t f1(uint32_t Km, uint32_t D, uint32_t Kr);
uint32_t f2(uint32_t Km, uint32_t D, uint32_t Kr);
uint32_t f3(uint32_t Km, uint32_t D, uint32_t Kr);

uint64_t EncryptBlock(uint64_t P);
uint64_t DecryptBlock(uint64_t C, std::vector<uint32_t> Km, std::vector<uint32_t> Kr);

void  GenerateRoundKeys(std::vector<std::vector<uint8_t>> x, std::vector<std::vector<uint8_t>> z, std::vector<uint32_t>& Km, std::vector<uint32_t>& Kr);

uint64_t getBlock64(const std::string& text, size_t offset);
void appendBlock64(uint64_t block, std::string& out); 

void EncryptData(std::vector<std::vector<uint8_t>> key,
                 std::string textP,
                 std::string& C);


void DecryptData(std::vector<std::vector<uint8_t>> key,
                 std::string C,
                 std::string& P);
