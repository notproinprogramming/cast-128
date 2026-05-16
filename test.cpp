#include "cast-128.hpp"
#include "fileutil.hpp"
#include <iostream>
#include <string>
#include <cassert>

void test_encrypt_decrypt() {
    std::string keyHex = "0123456712345678234567893456789a";
    auto key = parseKey128(keyHex);

    std::string P = hexToBytes("0123456789abcdef");
    std::string expectedC = "238B4FE5847E44B2";

    // тест шифрування
    std::string C;
    EncryptData(key, P, C);
    std::string actualC = bytesToHex(C);

    if (actualC == expectedC) {
        std::cout << "[PASS] Encryption: " << actualC << std::endl;
    } else {
        std::cout << "[FAIL] Encryption" << std::endl;
        std::cout << "  Expected: " << expectedC << std::endl;
        std::cout << "  Got:      " << actualC << std::endl;
    }

    // тест дешифрування
    std::string decrypted;
    DecryptData(key, C, decrypted);
    std::string actualP = bytesToHex(decrypted);
    std::string expectedP = "0123456789ABCDEF";

    if (actualP == expectedP) {
        std::cout << "[PASS] Decryption: " << actualP << std::endl;
    } else {
        std::cout << "[FAIL] Decryption" << std::endl;
        std::cout << "  Expected: " << expectedP << std::endl;
        std::cout << "  Got:      " << actualP << std::endl;
    }

    // тест на некоректну довжину
    try {
        std::string badCiphertext = "abc"; // не кратно 8
        DecryptData(key, badCiphertext, decrypted);
        std::cout << "[FAIL] Invalid length check (no exception thrown)" << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cout << "[PASS] Invalid length check: " << e.what() << std::endl;
    }
}

int main() {
    test_encrypt_decrypt();
    return 0;
}
