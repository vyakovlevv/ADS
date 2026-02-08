#include <bits/stdc++.h>
using namespace std;

class HashFuncGen {
 public:
  HashFuncGen(uint32_t seed = 0) :  seed(seed) {}

  uint32_t hash(const string& s) const {
    return fnv1a_32(s, seed);
  }

  // хэширование FNV-1a
  static uint32_t fnv1a_32(const string& data, uint32_t seed = 2166136261u) {
    const uint32_t fnv_prime = 16777619u;
    uint32_t hash = seed;
    for (unsigned char c : data) {
      hash ^= static_cast<uint32_t>(c);
      hash *= fnv_prime;
    }
    return hash;
  }

 private:
  uint32_t seed;
};
