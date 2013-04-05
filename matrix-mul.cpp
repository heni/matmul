#include "multiplication.h"
#include "transposition.h"
#include "common.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>
using namespace std;

template<class URNG>
TMatrix RandomMatrix(size_t n, size_t m, URNG&& g) {
    normal_distribution<double> distribution(0.0, 1.0);
    TMatrix result(n, vector<double>(m));
    for (size_t i = 0; i < n; ++i)
        generate(result[i].begin(), result[i].end(), [&g, &distribution]()->double { return distribution(g); });
    return result;
}

ostream& operator<<(ostream& out, const TMatrix& m) {
    for (const auto& row: m) {
        for (auto v: row)
            out << setw(8) << fixed << setprecision(4) << v;
        out << endl;
    }
}

int main() {
    const size_t N = 1000;
    TMatrix a = RandomMatrix(N, N, default_random_engine(1));
    TMatrix b = RandomMatrix(N, N, default_random_engine(2));
    TMatrix res;
    //MultiplyMatricesParallel(a, b, res);
    //cout << res << endl;
    MultiplyMatricesParallelWithTransposition(a, b, res);
    //cout << res << endl;
    return 0;
}
