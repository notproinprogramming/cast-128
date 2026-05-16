#include "s-boxes.hpp"
#include <iostream>
#include "cast-128.hpp"

uint32_t circ_shift_left(uint32_t a, uint32_t b) {
    
  uint32_t res = (a << b) | (a >> (32 - b));
    return res;
}
std::vector<uint8_t> divide_I(uint32_t I) {
    std::vector<uint8_t> res;
    for (int i = 0; i < 4; i++) {
        uint8_t b = (I >> (24 - (8 * i))) & 0xFF;
        res.push_back(b);
    }
    return res;
}
uint32_t to_int(uint8_t x0, uint8_t x1, uint8_t x2, uint8_t x3){
  uint32_t res = (x0<<24) + (x1<<16) + (x2<<8) + x3;
return res;}

uint32_t vec_to_int(std::vector<uint8_t> x){
  uint32_t res = (x[0]<<24) + (x[1]<<16) + (x[2]<<8) + x[3];
return res;}

uint32_t f1(uint32_t Km, uint32_t D, uint32_t Kr) {
    uint32_t I = circ_shift_left((Km + D), Kr);
    std::vector<uint8_t> divided_I = divide_I(I);
    uint32_t f = ((S1[divided_I[0]] ^ S2[divided_I[1]]) - S3[divided_I[2]]) +
                 S4[divided_I[3]];
    return f;
}

uint32_t f2(uint32_t Km, uint32_t D, uint32_t Kr) {
    uint32_t I = circ_shift_left((Km ^ D), Kr);
    std::vector<uint8_t> divided_I = divide_I(I);
    uint32_t f = ((S1[divided_I[0]] - S2[divided_I[1]]) + S3[divided_I[2]]) ^
                 S4[divided_I[3]];
    return f;
}

uint32_t f3(uint32_t Km, uint32_t D, uint32_t Kr) {
    uint32_t I = circ_shift_left((Km - D), Kr);
    std::vector<uint8_t> divided_I = divide_I(I);
    uint32_t f = ((S1[divided_I[0]] + S2[divided_I[1]]) ^ S3[divided_I[2]]) -
                 S4[divided_I[3]];
    return f;
}

uint64_t EncryptBlock(uint64_t P,std::vector<uint32_t> Km, std::vector<uint32_t> Kr ) {
    uint32_t L_prev = (P >> 32) & 0xFFFFFFFF;
    uint32_t R_prev = P & 0xFFFFFFFF;
    uint32_t L_next, R_next;
    for (int i = 1; i <= 16; i++) {
        L_next = R_prev;
        if (i % 3 == 1) {
            R_next = L_prev ^ f1(Km[i], R_prev,Kr[i]);
        }
        else if (i % 3 == 2) {
            R_next = L_prev ^ f2(Km[i], R_prev,Kr[i]);
        }
        else if (i % 3 == 0) {
            R_next = L_prev ^ f3(Km[i], R_prev,Kr[i]);
        } 
        L_prev = L_next;
        R_prev = R_next;
    }
    uint64_t C = (((uint64_t) R_next)<<32) | L_next;
    return C;
}

uint64_t DecryptBlock(uint64_t C, std::vector<uint32_t> Km, std::vector<uint32_t> Kr) {
    uint32_t L_prev = (C >> 32) & 0xFFFFFFFF;
    uint32_t R_prev = C & 0xFFFFFFFF;
    uint32_t L_next, R_next;

    for (int i = 16; i >= 1; i--) {   
        L_next = R_prev;
        if (i % 3 == 1) {
            R_next = L_prev ^ f1(Km[i], R_prev, Kr[i]);
        } else if (i % 3 == 2) {
            R_next = L_prev ^ f2(Km[i], R_prev, Kr[i]);
        } else {
            R_next = L_prev ^ f3(Km[i], R_prev, Kr[i]);
        }
        L_prev = L_next;
        R_prev = R_next;
    }

    uint64_t P = (((uint64_t)R_next) << 32) | L_next;
    return P;
}

std::vector<std::vector<uint8_t>> key(4); // вектор векторів з байт по 4 штуки кожен
std::vector<uint32_t> Km;
std::vector<uint32_t> Kr;
std::vector<std::vector<uint8_t>> z(4);

void  GenerateRoundKeys(std::vector<std::vector<uint8_t>> x, std::vector<std::vector<uint8_t>> z, std::vector<uint32_t>& Km, std::vector<uint32_t>& Kr){
  uint32_t tmp;

tmp = vec_to_int(x[0]) ^ S5[x[3][1]] ^ S6[x[3][3]] ^ S7[x[3][0]] ^ S8[x[3][2]] ^ S7[x[2][0]];
z[0]  = divide_I(tmp);
tmp = vec_to_int(x[2]) ^ S5[z[0][0]] ^ S6[z[0][2]] ^ S7[z[0][1]] ^ S8[z[0][3]] ^ S8[x[2][2]];
z[1] = divide_I(tmp);
tmp = vec_to_int(x[3]) ^ S5[z[1][3]] ^ S6[z[1][2]] ^ S7[z[1][1]] ^ S8[z[1][0]] ^ S5[x[2][1]];
z[2] =  divide_I(tmp);
tmp = vec_to_int(x[1]) ^ S5[z[2][2]] ^ S6[z[2][1]] ^ S7[z[2][3]] ^ S8[z[2][0]] ^ S6[x[2][3]];
z[3] = divide_I(tmp);
Km[1] = S5[z[2][0]] ^ S6[z[2][1]] ^ S7[z[1][3]] ^ S8[z[1][2]] ^ S5[z[0][2]];
Km[2] = S5[z[2][2]] ^ S6[z[2][3]] ^ S7[z[1][1]] ^ S8[z[1][0]] ^ S6[z[1][2]];
Km[3] = S5[z[3][0]] ^ S6[z[3][1]] ^ S7[z[0][3]] ^ S8[z[0][2]] ^ S7[z[2][1]];
Km[4] = S5[z[3][2]] ^ S6[z[3][3]] ^ S7[z[0][1]] ^ S8[z[0][0]] ^ S8[z[3][0]];
tmp = vec_to_int(z[2]) ^ S5[z[1][1]] ^ S6[z[1][3]] ^ S7[z[1][0]] ^ S8[z[1][2]] ^ S7[z[0][0]];
x[0] = divide_I(tmp);
tmp = vec_to_int(z[0]) ^ S5[x[0][0]] ^ S6[x[0][2]] ^ S7[x[0][1]] ^ S8[x[0][3]] ^ S8[z[0][2]];
x[1]  = divide_I(tmp);
tmp = vec_to_int(z[1]) ^ S5[x[1][3]] ^ S6[x[1][2]] ^ S7[x[1][1]] ^ S8[x[1][0]] ^ S5[z[0][1]];
x[2] = divide_I(tmp);
tmp = vec_to_int(z[3]) ^ S5[x[2][2]] ^ S6[x[2][1]] ^ S7[x[2][3]] ^ S8[x[2][0]] ^ S6[z[0][3]];
x[3]  = divide_I(tmp);
Km[5] = S5[x[0][3]] ^ S6[x[0][2]] ^ S7[x[3][0]] ^ S8[x[3][1]] ^ S5[x[2][0]];
Km[6] = S5[x[0][1]] ^ S6[x[0][0]] ^ S7[x[3][2]] ^ S8[x[3][3]] ^ S6[x[3][1]];
Km[7] = S5[x[1][3]] ^ S6[x[1][2]] ^ S7[x[2][0]] ^ S8[x[2][1]] ^ S7[x[0][3]];
Km[8] = S5[x[1][1]] ^ S6[x[1][0]] ^ S7[x[2][2]] ^ S8[x[2][3]] ^ S8[x[1][3]];
tmp = vec_to_int(x[0]) ^ S5[x[3][1]] ^ S6[x[3][3]] ^ S7[x[3][0]] ^ S8[x[3][2]] ^ S7[x[2][0]];
z[0] = divide_I(tmp);
tmp = vec_to_int(x[2]) ^ S5[z[0][0]] ^ S6[z[0][2]] ^ S7[z[0][1]] ^ S8[z[0][3]] ^ S8[x[2][2]];
z[1] = divide_I(tmp);
tmp = vec_to_int(x[3]) ^ S5[z[1][3]] ^ S6[z[1][2]] ^ S7[z[1][1]] ^ S8[z[1][0]] ^ S5[x[2][1]];
z[2] = divide_I(tmp);
tmp = vec_to_int(x[1]) ^ S5[z[2][2]] ^ S6[z[2][1]] ^ S7[z[2][3]] ^ S8[z[2][0]] ^ S6[x[2][3]];
z[3] = divide_I(tmp);
Km[9] = S5[z[0][3]] ^ S6[z[0][2]] ^ S7[z[3][0]] ^ S8[z[3][1]] ^ S5[z[2][1]];
Km[10] = S5[z[0][1]] ^ S6[z[0][0]] ^ S7[z[3][2]] ^ S8[z[3][3]] ^ S6[z[3][0]];
Km[11] = S5[z[1][3]] ^ S6[z[1][2]] ^ S7[z[2][0]] ^ S8[z[2][1]] ^ S7[z[0][2]];
Km[12] = S5[z[1][1]] ^ S6[z[1][0]] ^ S7[z[2][2]] ^ S8[z[2][3]] ^ S8[z[1][2]];
tmp = vec_to_int(z[2]) ^ S5[z[1][1]] ^ S6[z[1][3]] ^ S7[z[1][0]] ^ S8[z[1][2]] ^ S7[z[0][0]];
x[0] = divide_I(tmp);
tmp = vec_to_int(z[0]) ^ S5[x[0][0]] ^ S6[x[0][2]] ^ S7[x[0][1]] ^ S8[x[0][3]] ^ S8[z[0][2]];
x[1] = divide_I(tmp);
tmp = vec_to_int(z[1]) ^ S5[x[1][3]] ^ S6[x[1][2]] ^ S7[x[1][1]] ^ S8[x[1][0]] ^ S5[z[0][1]];
x[2] = divide_I(tmp);
tmp = vec_to_int(z[3]) ^ S5[x[2][2]] ^ S6[x[2][1]] ^ S7[x[2][3]] ^ S8[x[2][0]] ^ S6[z[0][3]];
x[3] = divide_I(tmp);
Km[13] = S5[x[2][0]] ^ S6[x[2][1]] ^ S7[x[1][3]] ^ S8[x[1][2]] ^ S5[x[0][3]];
Km[14] = S5[x[2][2]] ^ S6[x[2][3]] ^ S7[x[1][1]] ^ S8[x[1][0]] ^ S6[x[1][3]];
Km[15] = S5[x[3][0]] ^ S6[x[3][1]] ^ S7[x[0][3]] ^ S8[x[0][2]] ^ S7[x[2][0]];
Km[16] = S5[x[3][2]] ^ S6[x[3][3]] ^ S7[x[0][1]] ^ S8[x[0][0]] ^ S8[x[3][1]];


tmp = vec_to_int(x[0]) ^ S5[x[3][1]] ^ S6[x[3][3]] ^ S7[x[3][0]] ^ S8[x[3][2]] ^ S7[x[2][0]];
z[0]  = divide_I(tmp);
tmp = vec_to_int(x[2]) ^ S5[z[0][0]] ^ S6[z[0][2]] ^ S7[z[0][1]] ^ S8[z[0][3]] ^ S8[x[2][2]];
z[1] = divide_I(tmp);
tmp = vec_to_int(x[3]) ^ S5[z[1][3]] ^ S6[z[1][2]] ^ S7[z[1][1]] ^ S8[z[1][0]] ^ S5[x[2][1]];
z[2] =  divide_I(tmp);
tmp = vec_to_int(x[1]) ^ S5[z[2][2]] ^ S6[z[2][1]] ^ S7[z[2][3]] ^ S8[z[2][0]] ^ S6[x[2][3]];
z[3] = divide_I(tmp);
Kr[1] = (S5[z[2][0]] ^ S6[z[2][1]] ^ S7[z[1][3]] ^ S8[z[1][2]] ^ S5[z[0][2]]) & 0x1F;
Kr[2] = (S5[z[2][2]] ^ S6[z[2][3]] ^ S7[z[1][1]] ^ S8[z[1][0]] ^ S6[z[1][2]]) & 0x1F;
Kr[3] = (S5[z[3][0]] ^ S6[z[3][1]] ^ S7[z[0][3]] ^ S8[z[0][2]] ^ S7[z[2][1]]) & 0x1F;
Kr[4] = (S5[z[3][2]] ^ S6[z[3][3]] ^ S7[z[0][1]] ^ S8[z[0][0]] ^ S8[z[3][0]]) & 0x1F;
tmp = vec_to_int(z[2]) ^ S5[z[1][1]] ^ S6[z[1][3]] ^ S7[z[1][0]] ^ S8[z[1][2]] ^ S7[z[0][0]];
x[0] = divide_I(tmp);
tmp = vec_to_int(z[0]) ^ S5[x[0][0]] ^ S6[x[0][2]] ^ S7[x[0][1]] ^ S8[x[0][3]] ^ S8[z[0][2]];
x[1]  = divide_I(tmp);
tmp = vec_to_int(z[1]) ^ S5[x[1][3]] ^ S6[x[1][2]] ^ S7[x[1][1]] ^ S8[x[1][0]] ^ S5[z[0][1]];
x[2] = divide_I(tmp);
tmp = vec_to_int(z[3]) ^ S5[x[2][2]] ^ S6[x[2][1]] ^ S7[x[2][3]] ^ S8[x[2][0]] ^ S6[z[0][3]];
x[3]  = divide_I(tmp);
Kr[5] = (S5[x[0][3]] ^ S6[x[0][2]] ^ S7[x[3][0]] ^ S8[x[3][1]] ^ S5[x[2][0]]) & 0x1F;
Kr[6] = (S5[x[0][1]] ^ S6[x[0][0]] ^ S7[x[3][2]] ^ S8[x[3][3]] ^ S6[x[3][1]]) & 0x1F;
Kr[7] = (S5[x[1][3]] ^ S6[x[1][2]] ^ S7[x[2][0]] ^ S8[x[2][1]] ^ S7[x[0][3]]) & 0x1F;
Kr[8] = (S5[x[1][1]] ^ S6[x[1][0]] ^ S7[x[2][2]] ^ S8[x[2][3]] ^ S8[x[1][3]]) & 0x1F;
tmp = vec_to_int(x[0]) ^ S5[x[3][1]] ^ S6[x[3][3]] ^ S7[x[3][0]] ^ S8[x[3][2]] ^ S7[x[2][0]];
z[0] = divide_I(tmp);
tmp = vec_to_int(x[2]) ^ S5[z[0][0]] ^ S6[z[0][2]] ^ S7[z[0][1]] ^ S8[z[0][3]] ^ S8[x[2][2]];
z[1] = divide_I(tmp);
tmp = vec_to_int(x[3]) ^ S5[z[1][3]] ^ S6[z[1][2]] ^ S7[z[1][1]] ^ S8[z[1][0]] ^ S5[x[2][1]];
z[2] = divide_I(tmp);
tmp = vec_to_int(x[1]) ^ S5[z[2][2]] ^ S6[z[2][1]] ^ S7[z[2][3]] ^ S8[z[2][0]] ^ S6[x[2][3]];
z[3] = divide_I(tmp);
Kr[9] = (S5[z[0][3]] ^ S6[z[0][2]] ^ S7[z[3][0]] ^ S8[z[3][1]] ^ S5[z[2][1]]) & 0x1F;
Kr[10] = (S5[z[0][1]] ^ S6[z[0][0]] ^ S7[z[3][2]] ^ S8[z[3][3]] ^ S6[z[3][0]]) & 0x1F;
Kr[11] = (S5[z[1][3]] ^ S6[z[1][2]] ^ S7[z[2][0]] ^ S8[z[2][1]] ^ S7[z[0][2]]) & 0x1F;
Kr[12] = (S5[z[1][1]] ^ S6[z[1][0]] ^ S7[z[2][2]] ^ S8[z[2][3]] ^ S8[z[1][2]]) & 0x1F;
tmp = vec_to_int(z[2]) ^ S5[z[1][1]] ^ S6[z[1][3]] ^ S7[z[1][0]] ^ S8[z[1][2]] ^ S7[z[0][0]];
x[0] = divide_I(tmp);
tmp = vec_to_int(z[0]) ^ S5[x[0][0]] ^ S6[x[0][2]] ^ S7[x[0][1]] ^ S8[x[0][3]] ^ S8[z[0][2]];
x[1] = divide_I(tmp);
tmp = vec_to_int(z[1]) ^ S5[x[1][3]] ^ S6[x[1][2]] ^ S7[x[1][1]] ^ S8[x[1][0]] ^ S5[z[0][1]];
x[2] = divide_I(tmp);
tmp = vec_to_int(z[3]) ^ S5[x[2][2]] ^ S6[x[2][1]] ^ S7[x[2][3]] ^ S8[x[2][0]] ^ S6[z[0][3]];
x[3] = divide_I(tmp);
Kr[13] = (S5[x[2][0]] ^ S6[x[2][1]] ^ S7[x[1][3]] ^ S8[x[1][2]] ^ S5[x[0][3]]) & 0x1F;
Kr[14] = (S5[x[2][2]] ^ S6[x[2][3]] ^ S7[x[1][1]] ^ S8[x[1][0]] ^ S6[x[1][3]]) & 0x1F;
Kr[15] = (S5[x[3][0]] ^ S6[x[3][1]] ^ S7[x[0][3]] ^ S8[x[0][2]] ^ S7[x[2][0]]) & 0x1F;
Kr[16] = (S5[x[3][2]] ^ S6[x[3][3]] ^ S7[x[0][1]] ^ S8[x[0][0]] ^ S8[x[3][1]]) & 0x1F;

}

uint64_t getBlock64(const std::string& text, size_t offset) {
    uint64_t block = 0;

    for (int i = 0; i < 8; i++) {
        block <<= 8;

        if (offset + i < text.size()) {
            block |= static_cast<uint8_t>(text[offset + i]);
        } else {
            block |= 0x00; // padding нулями
        }
    }

    return block;
}
void appendBlock64(uint64_t block, std::string& out) {
    for (int i = 7; i >= 0; i--) {
        out.push_back((block >> (8 * i)) & 0xFF);
    }
}


void EncryptData(std::vector<std::vector<uint8_t>> key,
                 std::string textP,
                 std::string& C)
{
    std::vector<uint32_t> Km(17), Kr(17);
    std::vector<std::vector<uint8_t>> z(4);

    GenerateRoundKeys(key, z, Km, Kr);

    C.clear();

    for (size_t i = 0; i < textP.size(); i += 8) {
        uint64_t block = getBlock64(textP, i);

        uint64_t encrypted = EncryptBlock(block, Km, Kr);

        appendBlock64(encrypted, C);
    }
}

void DecryptData(std::vector<std::vector<uint8_t>> key,
                 std::string C,
                 std::string& P)
{
  if (C.size()%8 !=0){
   throw std::invalid_argument("Ciphertext length must be a multiple of 8 bytes");
  }
    std::vector<uint32_t> Km(17), Kr(17);
    std::vector<std::vector<uint8_t>> z(4);

    GenerateRoundKeys(key, z, Km, Kr);

    P.clear();

    for (size_t i = 0; i < C.size(); i += 8) {
        uint64_t block = getBlock64(C, i);

        uint64_t decrypted = DecryptBlock(block, Km, Kr);

        appendBlock64(decrypted, P);
    }
}



