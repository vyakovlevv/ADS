#include <bits/stdc++.h>
#include "ArrayGenerator.cpp"
#include "SortTester.cpp"

const int MIN_SIZE = 500;
const int MAX_SIZE = 100000;
const int STEP = 100;
const int LARGE_ARRAY_SIZE = 100000;

int main() {
  ArrayGenerator generator;
  SortTester tester;

  std::vector<int> thresholds = {5, 30};
  std::ofstream randomFile("random_results.csv");
  std::ofstream reverseFile("reverse_results.csv");
  std::ofstream almostFile("almost_results.csv");

  randomFile << "Size,MergeSort,Hybrid5,Hybrid30\n";
  reverseFile << "Size,MergeSort,Hybrid5,Hybrid30\n";
  almostFile << "Size,MergeSort,Hybrid5,Hybrid30\n";


  for (int size = MIN_SIZE; size <= MAX_SIZE; size += STEP) {
    std::cout << "Тестирование размера: " << size << std::endl;

    auto randomArray = generator.GenerateRandomArray(size);
    long long mergeTimeRandom = tester.TestMergeSort(randomArray);
    std::vector<long long> hybridTimesRandom;
    for (int threshold : thresholds) {
      hybridTimesRandom.push_back(tester.TestHybridMergeSort(randomArray, threshold));
    }

    auto reverseArray = generator.GenerateReverseArray(size);
    long long mergeTimeReverse = tester.TestMergeSort(reverseArray);
    std::vector<long long> hybridTimesReverse;
    for (int threshold : thresholds) {
      hybridTimesReverse.push_back(tester.TestHybridMergeSort(reverseArray, threshold));
    }

    auto almostArray = generator.GenerateAlmostArray(size);
    long long mergeTimeAlmost = tester.TestMergeSort(almostArray);
    std::vector<long long> hybridTimesAlmost;
    for (int threshold : thresholds) {
      hybridTimesAlmost.push_back(tester.TestHybridMergeSort(almostArray, threshold));
    }

    randomFile << size << "," << mergeTimeRandom;
    for (auto time : hybridTimesRandom) randomFile << "," << time;
    randomFile << "\n";

    reverseFile << size << "," << mergeTimeReverse;
    for (auto time : hybridTimesReverse) reverseFile << "," << time;
    reverseFile << "\n";

    almostFile << size << "," << mergeTimeAlmost;
    for (auto time : hybridTimesAlmost) almostFile << "," << time;
    almostFile << "\n";

    if (size % 5000 == 0) {
      randomFile.flush();
      reverseFile.flush();
      almostFile.flush();
    }
  }

  randomFile.close();
  reverseFile.close();
  almostFile.close();

  std::cout << "end" << std::endl;
}