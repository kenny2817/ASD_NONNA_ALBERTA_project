#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <iostream>

#include "nonna.h"

using namespace std;

struct centrino {
    float mean;
    float median;
    vector<int> to;

    centrino() : mean(0), median(0) {}
};

vector<centrino> c;
ofstream out ("output.txt");

// mean
inline bool compareByMean (const int &a, const int &b) {
    return c[a].mean < c[b].mean;
}

// median
inline bool compareByMedian (const int &a, const int &b) {
    return c[a].median < c[b].median;
}

// Median calculation function
inline float calculateMedian(vector<int>& values) {
    size_t size = values.size();
    if (size == 0) return 0;
    sort(values.begin(), values.end());

    if (size % 2 == 0) {
        return (values[size / 2 - 1] + values[size / 2]) / 2.0;
    } else {
        return values[size / 2];
    }
}

// O (log(G))
int arbreMagique (vector<int>& ftree, int index, int G) {
    // sum
    int sum = 0;
    for (int i = index +2; i <= G; i += i & -i)
        sum += ftree[i];

    // update
    for (int i = index +1; i > 0; i -= i & -i)
        ftree[i]++;

    return sum;
}

// O(F*log(G))
int count(const vector<int>& order, const int G) {
    int crossings = 0;
    vector<int> ftree(G + 1, 0);

    // theta (F)
    for (const auto& i : order)
        for (const int j : c[i].to)
            // O (log(G))
            crossings += arbreMagique(ftree, j, G);

    return crossings;
}

void printout (int count, vector<int>& order) {
    // print count
    out << count << endl;
    // print order
    for (int const i : order)
        out << i << " ";
    // print separator
    out << endl << "***" << endl;
}

int main() {
    // Define the duration for which the loop should run
    auto start_time = chrono::steady_clock::now();
    auto duration1 = chrono::milliseconds (2990);

    ifstream in ("input.txt");

    int C, G, F;
    in >> C >> G >> F;

    c.resize(C);
    vector<int> order (C);
    int tempA, tempB;

    for (int i = 0; i < F; ++i) {
        in >> tempA >> tempB;
        // normalize
        tempB -= C;
        c[tempA].to.push_back(tempB);
        c[tempA].mean += (float)tempB;
    }

    for (int i = 0; i < C; ++i) {
        order[i] = i;

        sort(c[i].to.begin(), c[i].to.end());

        if (!c[i].to.empty()) {
            c[i].mean /= c[i].to.size();
            c[i].median = calculateMedian(c[i].to);
        }
    }

    bool best = false;
    //start
    int minSoFar = count(order, G);

    // mean heuristic
    sort(order.begin(), order.end(), compareByMean);
    int actual = count(order, G);
    if (minSoFar > actual) {
        best = true;
        minSoFar = actual;
    }

    // median euristic
    sort(order.begin(), order.end(), compareByMedian);
    actual = count(order, G);
    if (minSoFar > actual) {
        minSoFar = actual;
    } else if (best) {
        sort(order.begin(), order.end(), compareByMean);
        minSoFar = count(order, G);
    } else {
        sort(order.begin(), order.end());
        minSoFar = count(order, G);
    }

    printout(minSoFar, order);

    // random generator
    random_device rd;
    mt19937 generate(rd());
    uniform_int_distribution<> distribution(0, C -1);

    int a,b;

    // random
    while (chrono::steady_clock::now() - start_time < duration1) {
        a = distribution(generate);

        do b = distribution(generate);
        while (a == b || abs(a - b) > C /17);

        swap(order[a], order[b]);
        actual = count(order, G);

        if (actual < minSoFar)  minSoFar = actual;
        else                    swap(order[a], order[b]);
    }

    printout(minSoFar, order);

    return 0;
}