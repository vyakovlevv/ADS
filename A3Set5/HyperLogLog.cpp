#include <cstdint>
#include "HashFuncGen.cpp"

class HyperLogLog {
 public:
  HyperLogLog(unsigned p_, const HashFuncGen& hash_gen_)
      : p(p_), hash_gen(hash_gen_)
  {
    if (p >= 32) {
      throw runtime_error("p must be < 32");
    }
    m = 1u << p;
    M.assign(m, 0);
    if (m == 16) alpha = 0.673;
    else if (m == 32) alpha = 0.697;
    else if (m == 64) alpha = 0.709;
    else alpha = 0.7213 / (1.0 + 1.079 / double(m));
  }

  void clear() { fill(M.begin(), M.end(), 0); }

  void add(const string& s) {
    uint32_t x = hash_gen.hash(s);
    uint32_t idx = x & ((1U << p) - 1U);
    uint32_t w = x >> p;
    uint8_t r = rho(w);
    if (r > M[idx]) M[idx] = r;
  }

  double estimate() const {
    double Z = 0.0;
    for (auto v : M) Z += pow(2.0, -double(v));
    double E = alpha * double(m) * double(m) / Z;

    if (E <= 2.5 * m) {
      uint32_t V = 0;
      for (auto v : M) if (v == 0) ++V;
      if (V > 0) {
        E = double(m) * log(double(m) / double(V));
      }
    }

    const double TWO32 = 4294967296.0;
    if (E > (1.0/30.0) * TWO32) {
      E = -TWO32 * log(1.0 - E / TWO32);
    }
    return E;
  }

  unsigned get_m() const { return m; }

 private:
  unsigned p;
  unsigned m;
  vector<uint8_t> M;
  double alpha;
  const HashFuncGen& hash_gen;
  uint8_t rho(uint32_t w) const {
    unsigned max_bits = 32 - p;
    if (w == 0) return static_cast<uint8_t>(max_bits + 1);
    unsigned bl = 32 - __builtin_clz(w);
    unsigned lz = max_bits - bl;
    return static_cast<uint8_t>(lz + 1);
  }
};