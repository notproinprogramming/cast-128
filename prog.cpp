#include "cast-128.hpp"
#include "fileutil.hpp"
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

namespace fs = std::filesystem;

static const std::string DIR_INPUT     = "my files";
static const std::string DIR_ENCRYPTED = "encrypted files";
static const std::string DIR_DECRYPTED = "decrypted files";

static void ensureDirs() {
    for (const auto& d : {DIR_INPUT, DIR_ENCRYPTED, DIR_DECRYPTED})
        fs::create_directories(d);
}

static void printBanner() {
    std::cout << "\n";
    std::cout << "  ========================================\n";
    std::cout << "           CAST-128  Encryptor            \n";
    std::cout << "  ========================================\n";
}

static void printMenu() {
    std::cout << "\n=== MENU ===\n";
    std::cout << "0) Exit\n";
    std::cout << "1) Encrypt file\n";
    std::cout << "2) Decrypt file\n";
    std::cout << "\n";
    std::cout << "  Directories:\n";
    std::cout << "    Input files  -> \"" << DIR_INPUT     << "/\"\n";
    std::cout << "    Encrypted    -> \"" << DIR_ENCRYPTED << "/\"\n";
    std::cout << "    Decrypted    -> \"" << DIR_DECRYPTED << "/\"\n";
    std::cout << "    Key file     -> anywhere (also searched in \""
              << DIR_INPUT << "/\")\n";
    std::cout << "\nYour choice: ";
}


static std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

//  Key 
static std::vector<std::vector<uint8_t>> loadKey(const std::string& keyFile) {
    std::string path = keyFile;
    if (!fs::exists(path)) {
        path = DIR_INPUT + "/" + keyFile;
        if (!fs::exists(path))
            throw std::runtime_error("Key file not found: " + keyFile);
    }
    std::string raw = readFile(path);
    std::string hex;
    for (char c : raw)
        if (!std::isspace(static_cast<unsigned char>(c)))
            hex += c;
    return parseKey128(hex);
}

//  Encrypt 
static void doEncrypt() {
    std::string inFile  = readLine("Input file (from \""  + DIR_INPUT     + "/\"): ");
    std::string outFile = readLine("Output file (into \"" + DIR_ENCRYPTED + "/\"): ");
    std::string keyFile = readLine("Key file: ");

    std::string inPath = DIR_INPUT + "/" + inFile;
    if (!fs::exists(inPath)) {
        std::cerr << "[ERROR] File not found: " << inPath << "\n";
        return;
    }

    try {
        std::cout << "\n[*] Reading:  " << inPath << "\n";
        std::string plaintext = readFile(inPath);

        std::cout << "[*] Loading key: " << keyFile << "\n";
        auto key = loadKey(keyFile);

        std::cout << "[*] Encrypting (" << plaintext.size() << " bytes)...\n";
        std::string ciphertext;
        EncryptData(key, plaintext, ciphertext);

        std::string outPath = DIR_ENCRYPTED + "/" + outFile;
        writeFile(outPath, ciphertext);

        std::cout << "[OK] Done -> " << outPath << "\n";
        std::cout << "     Input:  " << plaintext.size()  << " bytes\n";
        std::cout << "     Output: " << ciphertext.size() << " bytes\n";
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << "\n";
    }
}

//  Decrypt 
static void doDecrypt() {
    std::string inFile  = readLine("Encrypted file (from \"" + DIR_ENCRYPTED + "/\"): ");
    std::string outFile = readLine("Output file (into \""    + DIR_DECRYPTED + "/\"): ");
    std::string keyFile = readLine("Key file: ");

    std::string inPath = DIR_ENCRYPTED + "/" + inFile;
    if (!fs::exists(inPath)) {
        std::cerr << "[ERROR] File not found: " << inPath << "\n";
        return;
    }

    try {
        std::cout << "\n[*] Reading:  " << inPath << "\n";
        std::string ciphertext = readFile(inPath);

        std::cout << "[*] Loading key: " << keyFile << "\n";
        auto key = loadKey(keyFile);

        std::cout << "[*] Decrypting (" << ciphertext.size() << " bytes)...\n";
        std::string plaintext;
        DecryptData(key, ciphertext, plaintext);

        std::string outPath = DIR_DECRYPTED + "/" + outFile;
        writeFile(outPath, plaintext);

        std::cout << "[OK] Done -> " << outPath << "\n";
        std::cout << "     Input:  " << ciphertext.size() << " bytes\n";
        std::cout << "     Output: " << plaintext.size()  << " bytes\n";
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << "\n";
    }
}

//  main 
int main() {
    printBanner();
    ensureDirs();

    while (true) {
        printMenu();

        int choice;
        std::cin >> choice;
        std::cin.ignore(); 

        if (choice == 0)
            break;

        if (choice == 1) {
            doEncrypt();
        } else if (choice == 2) {
            doDecrypt();
        } else {
            std::cerr << "[ERROR] Unknown option.\n";
        }
    }

    std::cout << "\nBye!\n";
    return 0;
}
