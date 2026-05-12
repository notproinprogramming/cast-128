#include <cstdint>
#include <vector>
#include <cstdint>
#include <string>
uint32_t circ_shift_left(uint32_t a, uint32_t b);
std::vector<uint8_t> divide_I(uint32_t I);
uint32_t to_int(uint8_t x0, uint8_t x1, uint8_t x2, uint8_t x3);
uint32_t f1(uint32_t Km, uint32_t D, uint32_t Kr);
uint32_t f2(uint32_t Km, uint32_t D, uint32_t Kr);
uint32_t f3(uint32_t Km, uint32_t D, uint32_t Kr);

uint64_t EncryptBlock(uint64_t P);
char *DecryptBlock(char *M);
// GenerateRoundKeys;

void EncryptData(std::vector<std::vector<uint8_t>> key,
                 std::string textP,
                 std::string& C);
