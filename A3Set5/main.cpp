#include <bits/stdc++.h>
#include "RandomStreamGen.cpp"
#include "HyperLogLog.cpp"
using namespace std;

vector<size_t> cumulative_bounds(size_t N, double step_percent) {
  vector<size_t> bounds;
  if (step_percent <= 0.0) {
    bounds.push_back(N);
    return bounds;
  }
  double s = 0.0;
  while (s < 100.0 - 1e-12) {
    double e = min(100.0, s + step_percent);
    size_t end = static_cast<size_t>(ceil((e / 100.0) * double(N)));
    if (end > N) end = N;
    if (bounds.empty() || end != bounds.back()) bounds.push_back(end);
    s += step_percent;
  }
  return bounds;
}

double compute_mean(const vector<double> &v) {
  if (v.empty()) return 0.0;
  double s = 0;
  for (double x : v) s += x;
  return s / double(v.size());
}
double compute_std(const vector<double> &v) {
  if (v.empty()) return 0.0;
  double mean = compute_mean(v);
  double var = 0;
  for (double x : v) var += (x - mean) * (x - mean);
  var /= double(v.size());
  return sqrt(var);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  vector<size_t> stream_sizes = {100000, 300000, 600000};
  double step_percent = 10.0;
  vector<unsigned> B_values = {6, 8, 10, 12};
  size_t min_len = 5, max_len = 15;

  RandomStreamGen rgen(0xDEAD);

  ofstream fout_runs("per_run_results.csv");
  fout_runs << "stream_size,B,step_index,processed_pct,Ft,Nt\n";

  unordered_map<unsigned, vector<vector<double>>> Nt_byB;
  unordered_map<unsigned, vector<vector<uint64_t>>> Ft_byB;

  for (auto B : B_values) {
    Nt_byB[B] = {};
    Ft_byB[B] = {};
  }

  for (size_t si = 0; si < stream_sizes.size(); ++si) {
    size_t N = stream_sizes[si];
    cout << "stream size: " << N << " ...\n";
    vector<string> stream = rgen.generate(N, min_len, max_len);
    auto bounds = cumulative_bounds(N, step_percent);

    for (auto B : B_values) {
      HashFuncGen hfg(0);
      HyperLogLog hll(B, hfg);
      unordered_set<string> seen;
      seen.reserve(1024);

      vector<double> Nt_list;
      vector<uint64_t> Ft_list;

      size_t bound_idx = 0;
      size_t next_bound = bounds[bound_idx];
      for (size_t i = 0; i < stream.size(); ++i) {
        const string &s = stream[i];
        hll.add(s);
        seen.insert(s);
        if (static_cast<size_t>(i + 1) == next_bound) {
          uint64_t Ft = seen.size();
          double Nt = hll.estimate();
          Nt_list.push_back(Nt);
          Ft_list.push_back(Ft);

          double pct = double(next_bound) * 100.0 / double(N);
          fout_runs << N << "," << B << "," << (bound_idx + 1) << "," << pct << "," << Ft << "," << Nt << "\n";

          ++bound_idx;
          if (bound_idx >= bounds.size()) break;
          next_bound = bounds[bound_idx];
        }
      }
      Nt_byB[B].push_back(move(Nt_list));
      Ft_byB[B].push_back(move(Ft_list));
    }
    cout << "stream size " << N << " done.\n";
  }
  fout_runs.flush();
  fout_runs.close();
  cout << "zapisan per_run_results.csv\n";

  struct Stats { double mean_rel; double std_rel; double rmse; double theoretical_rel; unsigned m; };
  unordered_map<unsigned, Stats> bstats;

  for (auto B : B_values) {
    vector<double> rels;
    vector<double> estimates;
    for (size_t si = 0; si < stream_sizes.size(); ++si) {
      const auto &Nt_list = Nt_byB[B][si];
      const auto &Ft_list = Ft_byB[B][si];
      size_t K = Nt_list.size();
      for (size_t k = 0; k < K; ++k) {
        double ft = double(Ft_list[k]);
        double nt = Nt_list[k];
        if (ft > 0.0) {
          double rel = (nt - ft) / ft;
          rels.push_back(rel);
          estimates.push_back(nt);
        }
      }
    }
    double mean_rel = compute_mean(rels);
    double std_rel = compute_std(rels);
    double mse = 0;
    for (double r : rels) mse += r * r;
    mse = (rels.empty() ? 0.0 : mse / double(rels.size()));
    double rmse = sqrt(mse);
    unsigned m = 1u << B;
    double theoretical = 1.04 / sqrt(double(m));
    bstats[B] = {mean_rel, std_rel, rmse, theoretical, m};
  }

  unsigned chosenB = B_values.front();
  double best_std = 1e300;
  for (auto B : B_values) {
    if (bstats[B].std_rel < best_std || (fabs(bstats[B].std_rel - best_std) < 1e-15 && (1u << B) < (1u << chosenB))) {
      best_std = bstats[B].std_rel;
      chosenB = B;
    }
  }
  cout << "\nvibran B = " << chosenB << " (m = " << (1u << chosenB) << " registers).\n";
  cout << "otnositelnaya oshibka dlya vibrannogo B: " << bstats[chosenB].theoretical_rel << "\n";

  ofstream fout_agg("agg_stats_chosenB.csv");
  fout_agg << "step_index,processed_pct,mean_Nt,std_Nt,mean_Ft,std_Ft\n";

  size_t steps = Nt_byB[chosenB][0].size();
  for (size_t k = 0; k < steps; ++k) {
    vector<double> Ntvals;
    vector<double> Ftvals;
    for (size_t si = 0; si < stream_sizes.size(); ++si) {
      Ntvals.push_back(Nt_byB[chosenB][si][k]);
      Ftvals.push_back(double(Ft_byB[chosenB][si][k]));
    }
    double meanNt = compute_mean(Ntvals);
    double stdNt = compute_std(Ntvals);
    double meanFt = compute_mean(Ftvals);
    double stdFt = compute_std(Ftvals);
    double pct = double(k + 1) * step_percent;
    fout_agg << (k + 1) << "," << pct << "," << meanNt << "," << stdNt << "," << meanFt << "," << stdFt << "\n";
  }
  fout_agg.flush();
  fout_agg.close();
  cout << "zapisan agg_stats_chosenB.csv\n";

  return 0;
}
