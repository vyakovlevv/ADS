#include <bits/stdc++.h>
void merge(std::vector<int> &arr, int n, int start, int threshold);
class SortTester {
 private:
  template<typename Func>
  long long measureTime(Func sortFunction, std::vector<int> arr) {
    auto start = std::chrono::high_resolution_clock::now();
    sortFunction(arr);
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    return std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
  }

 public:
  long long TestMergeSort(const std::vector<int>& originalArr, int iterations = 3) {
    std::vector<long long> times;

    for (int i = 0; i < iterations; ++i) {
      auto time = measureTime([](std::vector<int> arr) {
        merge(arr, arr.size(), 0, 0);
      }, originalArr);
      times.push_back(time);
    }

    std::sort(times.begin(), times.end());
    return times[iterations / 2];
  }

  long long TestHybridMergeSort(const std::vector<int>& originalArray, int threshold, int iterations = 3) {
    std::vector<long long> times;

    for (int i = 0; i < iterations; ++i) {
      auto time = measureTime([threshold](std::vector<int> arr) {
        merge(arr, arr.size(), 0, threshold);
      }, originalArray);
      times.push_back(time);
    }

    std::sort(times.begin(), times.end());
    return times[iterations / 2];
  }
};