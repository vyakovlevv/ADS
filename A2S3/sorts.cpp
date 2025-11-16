#include <bits/stdc++.h>


void merge(std::vector<int> &arr, int n, int start, int threshold) {
  if (start + 1 >= start + n) return;
  if (n < threshold) {
    for (int i = start + 1; i < start + n; i++) {
      int key = arr[i];
      int j = i - 1;

      while (j >= start && arr[j] > key) {
        arr[j + 1] = arr[j];
        j--;
      }
      arr[j + 1] = key;
    }
    return;
  }
  int mid = n / 2;
  merge(arr, mid, start, threshold);
  merge(arr, n - mid, start + mid, threshold);
  int index_a = start, index_b = start + mid, index = 0;
  std::vector<int> res(n);
  while (index_a < start + mid && index_b < start+n) {
    if (arr[index_a] > arr[index_b]) {
      res[index++] = arr[index_b++];
    } else {
      res[index++] = arr[index_a++];
    }
  }
  while (index_a < start + mid) {
    res[index++] = arr[index_a++];
  }
  while (index_b < start + n) {
    res[index++] = arr[index_b++];
  }
  for (int i = 0; i < n; ++i) {
    arr[start + i] = res[i];
  }
}