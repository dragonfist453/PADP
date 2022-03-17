#include <math.h>
#include <omp.h>
#include <string.h>
#include <iostream>
#include <vector>
using namespace std;

double t;
inline long Strike(vector<bool>& composite, long i, long stride, long limit) {
    for (; i <= limit; i += stride) {
        composite[i] = true;
    }
    return i;
}

long CNE(long n) {
    long m = sqrt(n);
    long count = 0;
    vector<bool> composite(n + 1, false);
    t = omp_get_wtime();
    for (long i = 2; i <= m; i++) {
        if (!composite[i]) {
            count++;
            Strike(composite, 2 * i, i, n);
        }
    }

    for (long i = m + 1; i <= n; i++) {
        if (!composite[i])
            count++;
    }

    t = omp_get_wtime() - t;

    return count;
}

long CFE(long n) {
    long count = 0, m = (long)sqrt(n);
    vector<bool> composite(n + 1, false);
    vector<long> factor, striker;
    t = omp_get_wtime();
    for (int i = 2; i <= m; i++) {
        if (!composite[i]) {
            count++;
            striker.push_back(Strike(composite, 2 * i, i, m));
            factor.push_back(i);
        }
    }

    for (long w = m + 1; w <= n; w += m) {
        long limit = min(w + m - 1, n);
        for (int i = 0; i < factor.size(); i++) {
            striker[i] = Strike(composite, striker[i], factor[i], limit);
        }

        for (long i = w; i <= limit; i++) {
            if (!composite[i])
                count++;
        }
    }
    t = omp_get_wtime() - t;
    return count;
}

long CPE(long n) {
    long count = 0, m = (long)sqrt(n);

    vector<long> factor;
    t = omp_get_wtime();
    {
        vector<bool> composite(m + 1, false);
        for (int i = 2; i <= m; i++) {
            if (!composite[i]) {
                count++;
                Strike(composite, 2 * i, i, m);
                factor.push_back(i);
            }
        }
    }

// now the painful parallel part
#pragma omp parallel
    {
        vector<bool> composite(m + 1, false);
        vector<long> striker;
        long base = -1;

#pragma omp for reduction(+ : count)
        for (long w = m + 1; w < n; w += m) {
            // reset composite
            composite.assign(m + 1, false);
            long limit = min(w + m - 1, n) - w;
            if (base != w) {
                // compute striker
                for (auto f : factor) {
                    striker.push_back((f + w - 1) / f * f - w);
                }
            }

            // now that we have strikers proceed
            for (long i = 0; i < factor.size(); i++) {
                striker[i] =
                    Strike(composite, striker[i], factor[i], limit) - m;
            }
            for (long i = 0; i <= limit; i++) {
                if (!composite[i])
                    count++;
            }

            base += m;
        }
    }
    t = omp_get_wtime() - t;
    return count;
}

int main() {
    cout << 5761455 << endl;
    cout << CNE(100000000) << endl;
    cout << "t=" << t << endl;
    cout << CFE(100000000) << endl;
    cout << "t=" << t << endl;

    cout << CPE(100000000) << endl;
    cout << "t=" << t << endl;

    return 0;
}