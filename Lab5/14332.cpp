#include <iostream>
#include <vector>

using namespace std;

long long merge(vector<int>& arr, int left, int mid, int right) {
    vector<int> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;
    long long inversions = 0;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
            inversions += (mid - i + 1);
        }
    }

    while (i <= mid) {
        temp[k++] = arr[i++];
    }

    while (j <= right) {
        temp[k++] = arr[j++];
    }

    for (long long p = 0; p < temp.size(); p++) {
        arr[left + p] = temp[p];
    }

    return inversions;
}

long long mergeSort(vector<int>& arr, int left, int right) {
    long long inversions = 0;

    if (left < right) {
        int mid = left + (right - left) / 2;
        inversions += mergeSort(arr, left, mid);
        inversions += mergeSort(arr, mid + 1, right);
        inversions += merge(arr, left, mid, right);
    }

    return inversions;
}

int main() {
    long long n;
    cin >> n;

    vector<int> stones(n);
    
    for (int i = 0; i < n; i++) {
        cin >> stones[i];
    }

    long long inversions = mergeSort(stones, 0, n - 1);
    cout << inversions << endl;

    return 0;
}