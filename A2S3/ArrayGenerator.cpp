#include <bits/stdc++.h>

const int MIN_RAND_VALUE = 0;
const int MAX_RAND_VALUE = 6000;

class ArrayGenerator {
 private:
  std::random_device rd;
  std::mt19937 gen;
  std::vector<int> preCalcRandArr;
  std::vector<int> preCalcReverseArr;
  std::vector<int> preCalcAlmostArr;


  std::vector<int> generateRandomArray(int size) {
    std::vector<int> arr(size);
    std::uniform_int_distribution<int> dis(MIN_RAND_VALUE, MAX_RAND_VALUE);

    for (int i = 0; i < size; ++i) {
      arr[i] = dis(gen);
    }
    return arr;
  }

  std::vector<int> generateReverseSortedArray(int size) {
    std::vector<int> arr = generateRandomArray(size);
    std::sort(arr.begin(), arr.end(), std::greater<int>());
    return arr;
  }

  std::vector<int> generateAlmostSortedArray(int size, int numSwaps) {
    std::vector<int> arr(size);

    for (int i = 0; i < size; ++i) {
      arr[i] = MIN_RAND_VALUE + (MAX_RAND_VALUE - MIN_RAND_VALUE) * i / size;
    }

    std::uniform_int_distribution<int> indexDis(0, size - 1);

    for (int i = 0; i < numSwaps; ++i) {
      int idx1 = indexDis(gen);
      int idx2 = indexDis(gen);
      std::swap(arr[idx1], arr[idx2]);
    }

    return arr;
  }

  std::vector<int> getSubarray(const std::vector<int>& arr, int size) {
    if (size > arr.size()) {
      return arr;
    }
    return std::vector<int>(arr.begin(), arr.begin() + size);
  }


 public:
  ArrayGenerator() : gen(rd()) {
    preCalcRandArr = generateRandomArray(100000);
    preCalcAlmostArr = generateAlmostSortedArray(100000, 30);
    preCalcReverseArr = generateReverseSortedArray(100000);
  }

  std::vector<int> GenerateRandomArray(int size) {
    return getSubarray(preCalcRandArr, size);
  }

  std::vector<int> GenerateAlmostArray(int size) {
    return getSubarray(preCalcAlmostArr, size);
  }

  std::vector<int> GenerateReverseArray(int size) {
    return getSubarray(preCalcReverseArr, size);
  }
};
