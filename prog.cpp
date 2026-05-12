#include "cast-128.hpp"
#include "fileutil.hpp"
#include <iostream>
#include <string>
using namespace std;

int main() {
string keyHex = "0123456712345678234567893456789a";
auto key = parseKey128(keyHex);

string P = "0123456789ABCDEF";
string C;
EncryptData(key, P, C);
string expectedC = "238B4FE5847E44B2";



cout << hex << C << endl;
}
