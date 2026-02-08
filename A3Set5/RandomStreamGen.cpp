#include <bits/stdc++.h>
using namespace std;

class RandomStreamGen {
 public:
  RandomStreamGen(uint64_t seed = std::chrono::high_resolution_clock::now().time_since_epoch().count())
      : rng(seed)
  {
    for (char c = 'A'; c <= 'Z'; ++c) alphabet.push_back(c);
    for (char c = 'a'; c <= 'z'; ++c) alphabet.push_back(c);
    for (char c = '0'; c <= '9'; ++c) alphabet.push_back(c);
    alphabet.push_back('-');
    dist_char = uniform_int_distribution<size_t>(0, alphabet.size() - 1);
  }

  vector<string> generate(size_t n, size_t min_len = 1, size_t max_len = 30) {
    if (min_len == 0) min_len = 1;
    if (max_len < min_len) max_len = min_len;
    uniform_int_distribution<size_t> dist_len(min_len, max_len);
    vector<string> out;
    out.reserve(n);
    for (size_t i = 0; i < n; ++i) {
      size_t len = dist_len(rng);
      out.push_back(generate_one(len));
    }
    return out;
  }

  string generate_one(size_t len) {
    string s;
    s.resize(len);
    for (size_t i = 0; i < len; ++i) {
      s[i] = alphabet[dist_char(rng)];
    }
    return s;
  }

  static vector<string> get_prefix_by_percentage(const vector<string>& stream, double percent) {
    if (percent <= 0.0) return {};
    if (percent >= 100.0) return stream;
    size_t n = stream.size();
    size_t k = static_cast<size_t>(ceil((percent / 100.0) * double(n)));
    if (k > n) k = n;
    return vector<string>(stream.begin(), stream.begin() + k);
  }

  static vector<vector<string>> chunk_by_percent(const vector<string>& stream, double step_percent) {
    vector<vector<string>> parts;
    if (step_percent <= 0.0) return parts;
    if (step_percent >= 100.0) { parts.push_back(stream); return parts; }
    size_t n = stream.size();
    for (double s = 0.0; s < 100.0; s += step_percent) {
      double e = min(100.0, s + step_percent);
      size_t start = static_cast<size_t>(floor((s / 100.0) * n));
      size_t end = static_cast<size_t>(ceil((e / 100.0) * n));
      if (start >= end) continue;
      parts.emplace_back(stream.begin() + start, stream.begin() + min(end, n));
    }
    return parts;
  }

 private:
  std::mt19937_64 rng;
  vector<char> alphabet;
  uniform_int_distribution<size_t> dist_char;
};