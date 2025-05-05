#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>
using namespace std;

// Safe parallel bubble sort using Odd-Even Transposition Sort
void parallel_bubble_sort(vector<int>& arr) {
    int n = arr.size();
    bool swapped = true;

    while (swapped) {
        swapped = false;

        bool local_swapped = false;
        #pragma omp parallel for shared(arr) reduction(|:local_swapped)
        for (int i = 0; i < n - 1; i += 2) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                local_swapped = true;
            }
        }
        swapped = swapped || local_swapped;

        local_swapped = false;
        #pragma omp parallel for shared(arr) reduction(|:local_swapped)
        for (int i = 1; i < n - 1; i += 2) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                local_swapped = true;
            }
        }
        swapped = swapped || local_swapped;
    }
}

// Parallel merge sort
void parallel_merge_sort(vector<int>& arr) {
    if (arr.size() <= 1) return;

    vector<int> left(arr.begin(), arr.begin() + arr.size() / 2);
    vector<int> right(arr.begin() + arr.size() / 2, arr.end());

    #pragma omp parallel sections
    {
        #pragma omp section
        parallel_merge_sort(left);

        #pragma omp section
        parallel_merge_sort(right);
    }

    merge(left.begin(), left.end(), right.begin(), right.end(), arr.begin());
}

// Display helper
void show(int op, vector<int>& arr){
    vector<int> copy = arr;
    string str = "", name = "";

    switch(op){
        case 0:
            name = "Original"; str = " without"; break;
        case 1:
            name = "Sequential"; str = " bubble";
            sort(copy.begin(), copy.end()); break;
        case 2:
            name = "Parallel"; str = "   bubble";
            parallel_bubble_sort(copy); break;
        case 3:
            name = "Sequential"; str = " merge";
            stable_sort(copy.begin(), copy.end()); break;
        case 4:
            name = "Parallel"; str = "   merge";
            parallel_merge_sort(copy); break;
    }

    cout << name << str << " sort : ";
    for (size_t i = 0; i < copy.size(); ++i)
        cout << copy[i] << " ";
    cout << endl;
}

int main() {
    int a[] = {4, 2, 6, 8, 1, 3, 9, 5, 7};
    vector<int> arr(a, a + sizeof(a)/sizeof(int));
    for (int i = 0; i < 5; i++) show(i, arr);
    return 0;
}


