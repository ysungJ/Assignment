#include <iostream>
#include <fstream>
#include <random>
using namespace std;

int main() {
    // �������� �ʱ�ȭ
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<int> distribution(1, 10000);

    // �����͸� ���Ͽ� ����
    ofstream outputFile("random_numbers.txt");
    for (int i = 0; i < 10000; ++i) {
        int random_number = distribution(generator);
        outputFile << random_number << std::endl;
    }
    outputFile.close();

    cout << "'random_numbers.txt'�� ����Ǿ����ϴ�." << endl;

    return 0;
}