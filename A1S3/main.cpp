#include <bits/stdc++.h>

double generateRandomDouble(double a, double b) {
  static std::random_device rd;
  static std::mt19937 gen(rd());

  std::uniform_real_distribution<double> dis(a, b);

  return dis(gen);
}

class Point {
 public:
  Point(double x, double y) {
    this->x = x;
    this->y = y;
  }

  double GetDistance(Point &p) const {
    double dist = std::sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y));
    return dist;
  }

 private:
  double x;
  double y;
};

class Circle : Point {
 public:
  Circle(double x, double y, double r) : Point(x, y), x(x), y(y), r(r) {
  }

  bool IsAround(Point &p) const {
    double dist = GetDistance(p);
    return dist <= r;
  }
 private:
  double x;
  double y;
  double r;
};

double CalculateSquare(const std::vector<Circle> &circles, int N, double a1, double a2, double b1, double b2) {
  int n = circles.size();
  int count = 0;
  for (int i = 0; i < N; ++i) {
    double x = generateRandomDouble(a1, a2);
    double y = generateRandomDouble(b1, b2);
    auto p = Point(x, y);
    bool isAround = true;
    for (int j = 0; j < n && isAround; ++j) {
      isAround = circles[j].IsAround(p);
    }
    if (isAround) {
      count++;
    }
  }

  return count / (double) N * (a2 - a1) * (b2 - b1);
}

int main() {
  std::vector<Circle> circles{
      Circle(1, 1, 1),
      Circle(1.5, 2, std::sqrt(5) / 2),
      Circle(2, 1.5, std::sqrt(5) / 2)
  };
  double a1 = 0, a2 = 3.5, b1 = 0, b2 = 3.5;
  std::cout << "square: big square\n";
  for (int i = 100 ; i < 100000; i += 500) {
    double square = CalculateSquare(circles, i, a1, a2, b1, b2);
    std::cout << square << ' ';
  }
  a1 = 0.8, a2 = 2.2, b1 = 0.8, b2 = 2.2;
  std::cout << "\nsquare: small square\n";
  for (int i = 100 ; i < 100000; i += 500) {
    double square = CalculateSquare(circles, i, a1, a2, b1, b2);
    std::cout << square << ' ';
  }

  return 0;
}

