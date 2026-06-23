// Author: B133245007 李芸希
// Date  : 2025/11/25

/*
Purpose:
    實作各種排序演算法，並比較其效能
    包含: Insertion Sort (A 類), Merge Sort (B 類),
          Quick Sort (C 類), C qsort (D 類), C++ sort (E 類)
*/

#include <bits/stdc++.h>
using namespace std;

// A 類：Insertion Sort（插入排序）
void insertionSort(vector<int>& a) {
    int n = a.size();
    for (int i = 1; i < n; ++i) {      // 從第 2 個元素開始當作 key
        int key = a[i];
        int j = i - 1;
        // 往左找合適的位置，並一路搬移比 key 大的元素
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;               // 把 key 放到正確的位置
    }
}

// B 類：Merge Sort（合併排序）
// 將區間 [left, mid] 與 [mid+1, right] 的兩段已排序子陣列合併
void mergeVec(vector<int>& a, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);         // 暫存左、右兩段
    for (int i = 0; i < n1; i++) L[i] = a[left + i];
    for (int j = 0; j < n2; j++) R[j] = a[mid + 1 + j];

    int i = 0, j = 0, k = left;
    // 兩個已排序子序列作 merge
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) a[k++] = L[i++];
        else              a[k++] = R[j++];
    }
    // 把剩下沒用完的元素補上
    while (i < n1) a[k++] = L[i++];
    while (j < n2) a[k++] = R[j++];
}

// Merge Sort 主遞迴
void mergeSort(vector<int>& a, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(a, left, mid);          // 排左半
        mergeSort(a, mid + 1, right);     // 排右半
        mergeVec(a, left, mid, right);    // 合併
    }
}

// C 類：Quick Sort（快速排序）
// 使用左端 a[left] 當 pivot，分割區間
int partitionQuick(vector<int>& a, int left, int right) {
    int pivot = a[left];
    int i = left, j = right;

    while (i < j) {
        while (i <= right && a[i] <= pivot) i++; // 從左找比 pivot 大的
        while (a[j] > pivot) j--;                // 從右找比 pivot 小等的
        if (i < j) swap(a[i], a[j]);             // 交換錯邊的元素
    }
    swap(a[left], a[j]);                         // 把 pivot 放回正確位置
    return j;                                    // 回傳 pivot 最後的位置
}

// Quick Sort 主遞迴
void quickSort(vector<int>& a, int left, int right) {
    if (left < right) {
        int p = partitionQuick(a, left, right);
        quickSort(a, left, p - 1);
        quickSort(a, p + 1, right);
    }
}

// D 類：C qsort() 比較函式（升冪排序）
int compareInt(const void* p1, const void* p2) {
    int x = *(const int*)p1;
    int y = *(const int*)p2;
    // x > y 傳回 1，x < y 傳回 -1，等於傳回 0
    return (x > y) - (x < y);
}

// 封裝 C 標準函式庫的 qsort，使用同樣的 vector<int> 介面
void c_qsort(vector<int>& a) {
    if (a.empty()) return;
    qsort(a.data(), a.size(), sizeof(int), compareInt);
}

// E 類：C++ STL sort()
void cpp_sort(vector<int>& a) {
    sort(a.begin(), a.end());
}

// Part B：大量資料測試 — 8 種 size × 5 種排序，並取 10 次平均
void testcase() {
    using namespace chrono;

    // 題目指定的資料量
    vector<int> sizes = {100, 500, 1000, 5000, 10000, 50000, 100000, 500000};
    mt19937 rng((unsigned)steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> dist(0, 1000000);

    const int TRIALS = 10;   // 每個資料量測 10 組

    for (int n : sizes) {

        // 五種排序各自累積時間，最後除以 TRIALS 取平均
        double sumA = 0, sumB = 0, sumC = 0, sumD = 0, sumE = 0;

        for (int t = 0; t < TRIALS; t++) {

            // 產生一組長度為 n 的亂數資料
            vector<int> base(n);
            for (int i = 0; i < n; i++) base[i] = dist(rng);

            // 五種排序法都用同一組 base 的複本
            vector<int> aA = base;
            vector<int> aB = base;
            vector<int> aC = base;
            vector<int> aD = base;
            vector<int> aE = base;

            // A 類：Insertion Sort
            auto t1 = steady_clock::now();
            insertionSort(aA);
            auto t2 = steady_clock::now();
            sumA += duration<double>(t2 - t1).count();

            // B 類：Merge Sort
            t1 = steady_clock::now();
            mergeSort(aB, 0, aB.size() - 1);
            t2 = steady_clock::now();
            sumB += duration<double>(t2 - t1).count();

            // C 類：Quick Sort
            t1 = steady_clock::now();
            quickSort(aC, 0, aC.size() - 1);
            t2 = steady_clock::now();
            sumC += duration<double>(t2 - t1).count();

            // D 類：C qsort()
            t1 = steady_clock::now();
            c_qsort(aD);
            t2 = steady_clock::now();
            sumD += duration<double>(t2 - t1).count();

            // E 類：C++ sort
            t1 = steady_clock::now();
            cpp_sort(aE);
            t2 = steady_clock::now();
            sumE += duration<double>(t2 - t1).count();
        }

        // 印出「平均」執行時間
        cout << "Insertion Sort For size " << n
             << " Execute : " << sumA / TRIALS << " sec\n";

        cout << "Merge Sort For size " << n
             << " Execute : " << sumB / TRIALS << " sec\n";

        cout << "Quick Sort For size " << n
             << " Execute : " << sumC / TRIALS << " sec\n";

        cout << "C qsort() For size " << n
             << " Execute : " << sumD / TRIALS << " sec\n";

        cout << "C++ sort For size " << n
             << " Execute : " << sumE / TRIALS << " sec\n\n";
    }
}


// Part A：檔案處理（讀 input.txt → 輸出到 outputA～E.txt）
void filerelate() {

    ifstream in("input.txt");          // 由當前目錄讀 input.txt
    if (!in) {
        cerr << "Cannot open input.txt\n";
        return;
    }

    int N;
    in >> N;                           // 第一行：資料筆數 N
    vector<int> data(N);
    for (int i = 0; i < N; i++) in >> data[i];  // 讀入 N 個整數
    in.close();

    // 五份拷貝，分別給 A~E 類排序使用
    vector<int> aA = data, aB = data, aC = data, aD = data, aE = data;

    // 開啟五個輸出檔案
    ofstream outA("outputA.txt");
    ofstream outB("outputB.txt");
    ofstream outC("outputC.txt");
    ofstream outD("outputD.txt");
    ofstream outE("outputE.txt");

    // A：Insertion Sort
    insertionSort(aA);
    outA << "Insertion Sort\n";
    for (int x : aA) outA << x << "\n";

    // B：Merge Sort
    mergeSort(aB, 0, aB.size() - 1);
    outB << "Merge Sort\n";
    for (int x : aB) outB << x << "\n";

    // C：Quick Sort
    quickSort(aC, 0, aC.size() - 1);
    outC << "Quick Sort\n";
    for (int x : aC) outC << x << "\n";

    // D：C qsort()
    c_qsort(aD);
    outD << "C qsort()\n";
    for (int x : aD) outD << x << "\n";

    // E：C++ sort()
    cpp_sort(aE);
    outE << "C++ sort()\n";
    for (int x : aE) outE << x << "\n";
}

int main() {
    srand(time(NULL));  

    // testcase();      // 大量資料測試（寫報告時用）
    filerelate();        // 讀 input.txt + 輸出 5 個排序結果檔案

    return 0;
}
