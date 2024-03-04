#include <iostream>
#include <fstream>
#include <random>
using namespace std;

int main() {
    // 랜던변수 초기화
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<int> distribution(1, 10000);

    // 데이터를 파일에 저장
    ofstream outputFile("random_numbers.txt");
    for (int i = 0; i < 10000; ++i) {
        int random_number = distribution(generator);
        outputFile << random_number << std::endl;
    }
    outputFile.close();

    cout << "'random_numbers.txt'에 저장되었습니다." << endl;

    return 0;
}