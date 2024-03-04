#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>

using namespace std;

// 정렬 알고리즘 클래스 정의
class SortingAlgorithm {
public:
    virtual void sort(vector<int>& data) = 0;
    virtual string complexity() = 0; // 추정 복잡도 반환
    virtual long long getComparisons() = 0; //비교 횟수 반환
};

//삽입정렬 클래스
class InsertionSort : public SortingAlgorithm {
public:
    long long comparisons = 0;
    void sort(vector<int>& data) override {
        for (int i = 1; i < data.size(); i++) {
            int key = data[i];
            int j = i - 1;
            while (j >= 0 && data[j] > key) {
                data[j + 1] = data[j];
                j--;
                comparisons++; // 비교 횟수 증가
            }
            data[j + 1] = key;
        }
    }

    string complexity() override {
        return "O(n^2)";
    }
    long long getComparisons() override {
        return comparisons; 
    }
};
//퀵정렬 클래스
class QuickSort : public SortingAlgorithm {
public:
    long long comparisons;
    void sort(vector<int>& data) override {
        comparisons = 0; //비교횟수 초기화
        quicksort(data, 0, data.size() - 1);
    }

private:
    void quicksort(vector<int>& data, int low, int high) {
        if (low < high) {
            int pivot = partition(data, low, high);
            quicksort(data, low, pivot - 1);
            quicksort(data, pivot + 1, high);
        }
    }

    int partition(vector<int>& data, int low, int high) {
        int pivot = data[high];
        int i = (low - 1);

        for (int j = low; j < high; j++) {
            comparisons++; //비교 수 증가
            if (data[j] <= pivot) {
                i++;
                swap(data[i], data[j]);
            }
        }
        swap(data[i + 1], data[high]);
        return i + 1;
    }

    string complexity() override {
        return "O(n*log(n))";
    }
    long long getComparisons() override {
        return comparisons;
    }
};
//힙정렬 클래스 
class HeapSort : public SortingAlgorithm {
public:
    long long comparisons = 0;
    void sort(vector<int>& data) override {
        buildHeap(data);
        int size = data.size();
        heapify(data,size, 0);
    }

    string complexity() override {
        return "O(n*log(n))";

    }
    long long getComparisons() override {
        return comparisons;
    }

private:
    void buildHeap(vector<int>& data) {
        for (int i = data.size() / 2 - 1; i >= 0; i--) {
            heapify(data, data.size(), i);
        }
    }

    void heapify(vector<int>& data, int size, int root) {
        int largest = root;
        int left = 2 * root + 1;
        int right = 2 * root + 2;

        if (left < size && data[left] > data[largest]) {
            largest = left;
        }

        if (right < size && data[right] > data[largest]) {
            largest = right;
        }

        if (largest != root) {
            swap(data[root], data[largest]);
            comparisons++; // 비교 횟수 증가
            heapify(data, size, largest);
        }
    }

};
//머지정렬 클래스
class MergeSort : public SortingAlgorithm {
public:
    long long comparisons;
    void sort(vector<int>& data) override {
        comparisons = 0; // 비교 횟수 초기화
        mergesort(data, 0, data.size() - 1);
    }

private:
    void mergesort(vector<int>& data, int low, int high) {
        if (low < high) {
            int mid = low + (high - low) / 2;
            mergesort(data, low, mid);
            mergesort(data, mid + 1, high);
            merge(data, low, mid, high);
        }
    }

    void merge(vector<int>& data, int low, int mid, int high) {
        int n1 = mid - low + 1;
        int n2 = high - mid;

        vector<int> left(n1);
        vector<int> right(n2);

        for (int i = 0; i < n1; i++)
            left[i] = data[low + i];
        for (int j = 0; j < n2; j++)
            right[j] = data[mid + 1 + j];

        int i = 0, j = 0, k = low;

        while (i < n1 && j < n2) {
            comparisons++; // 비교 수 증가
            if (left[i] <= right[j]) {
                data[k] = left[i];
                i++;
            }
            else {
                data[k] = right[j];
                j++;
            }
            k++;
        }

        while (i < n1) {
            data[k] = left[i];
            i++;
            k++;
        }

        while (j < n2) {
            data[k] = right[j];
            j++;
            k++;
        }
    }

    string complexity() override {
        return "O(n*log(n))";
    }
    long long getComparisons() override {
        return comparisons;
    }
};
//랜덤 변수 선언

void Random_N() {
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
    cout << "========================================" << endl;
};

//메인함수 
int main() {

    Random_N();
    
    vector<int> data;
    ifstream inputFile("random_numbers.txt");
    int num;

    while (inputFile >> num) {
        data.push_back(num);
    }
    inputFile.close();

   
    vector<SortingAlgorithm*> algorithms = { new InsertionSort, new QuickSort, new HeapSort, new MergeSort };

    for (SortingAlgorithm* algorithm : algorithms) {
        long long totalComparisons = 0.0;
        chrono::duration<double, milli> totalTime(0);

            vector<int> dataCopy = data;
            auto start = chrono::high_resolution_clock::now();
            algorithm->sort(dataCopy);
            auto end = chrono::high_resolution_clock::now();
            totalTime += chrono::duration_cast<chrono::duration<double, milli>>(end - start);
            totalComparisons += algorithm->getComparisons();// 비교 횟수를 더해준다
            string algorithmName = typeid(*algorithm).name();
            algorithmName = algorithmName.substr(6); // Remove "class "
        
            cout << "알고리즘 : " << algorithmName << endl;
            cout << "평균 시간: " << totalTime.count() << " Ms" << endl;
            cout << "평균 비교 횟수: " << totalComparisons  << endl;
            cout << "추정 복잡도 : " << algorithm->complexity() << endl;
            cout << "-------------------------" << endl;
        
    }


    for (SortingAlgorithm* algorithm : algorithms) {
        delete algorithm;
    }

    return 0;
}