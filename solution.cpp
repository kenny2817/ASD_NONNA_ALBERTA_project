//
// Created by quent on 26/05/2024.
//
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>

#include "nonna.h"

using namespace std;

struct centrino {
    float mean;
    float median;
    vector<int> to;

    explicit centrino(float mn = 0) : mean(mn), to({}) {}
};

vector<centrino> c;
ofstream out ("output.txt");

// mean
bool compareByMean (const int &a, const int &b) {
    return c[a].mean < c[b].mean;
}

// Median calculation function
float calculateMedian(vector<int>& values) {
    size_t size = values.size();
    if (size == 0) return 0; // If there are no elements, return 0
    sort(values.begin(), values.end()); // Sort the values

    if (size % 2 == 0) {
        return (values[size / 2 - 1] + values[size / 2]) / 2.0;
    } else {
        return values[size / 2];
    }
}

// median
bool compareByMedian (const int &a, const int &b) {
    return c[a].median < c[b].median;
}

// O (log(G))
int arbreMagique (int* ftree, int index, int G) {
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
    int* ftree = new int[G +1]();

    // theta (F)
    for (const auto& i : order)
        for (const int j : c[i].to)
            // O (log(G))
            crossings += arbreMagique(ftree, j, G);

    // Deallocate the memory
    delete[] ftree;

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

        c[i].mean /= (float) c[i].to.size();

        c[i].median = calculateMedian(c[i].to);
    }

    //start
    int minSoFar = count(order, G);
    printout(minSoFar, order);

    // mean heuristic
    sort(order.begin(), order.end(), compareByMean);

    int actual = count(order, G);
    if (minSoFar > actual) {
        minSoFar = actual;
        printout(minSoFar, order);
    }

    // median euristic
    sort(order.begin(), order.end(), compareByMedian);

    actual = count(order, G);
    if (minSoFar > actual) {
        minSoFar = actual;
        printout(minSoFar, order);
    } else
        sort(order.begin(), order.end(), compareByMean);

    // random generator
    random_device rd;
    mt19937 generate(rd());
    uniform_int_distribution<> distribution(0, C -1);

    int a,b;
    // random
    while (1) {
        a = distribution(generate);

        do b = distribution(generate);
        while (a == b);

        swap(order[a], order[b]);
        actual = count(order, G);

        if (actual < minSoFar) {
            minSoFar = actual;
            printout(minSoFar, order);
            continue;
        }

        if (actual == minSoFar) continue;

        swap(order[a], order[b]);
    }
}