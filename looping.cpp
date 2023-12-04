



///////////////////////////////////////////////////////////////////////////////////////////////////
// Loop unrolling - https://en.wikipedia.org/wiki/Loop_unrolling
//
// The compiler can perform this automatically and will likely do so when it can. If the compiler
// chooses not to do this, then you should measure a baseline and any changes you make to ensure
// that loop unrolling actually provides a benefit.
//
// Pros:
//  Less tests and jumps
// Cons:
//  Increased binary - may also reduce performance with increased register usage
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <cstddef> //size_t

constexpr std::size_t N = 100;

void loop_unrolling_1() {
    int a[N];
    for (std::size_t i = 0; i < N; i++) {
        a[i] = i;
    }
}

void loop_unrolling_2() {
    int a[N];
    for (std::size_t i = 0; i < N; i+=4) {
        a[i]   = i;
        a[i+1] = i+1;
        a[i+2] = i+2;
        a[i+3] = i+3;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// Loop interchange - https://en.wikipedia.org/wiki/Loop_interchange
//
// In principal, the CPU is better at access of contiguous memory in C/C++ (row-major order). In this example all we
// have to do is swap the order of for loops to ensure that each matrix is accessed in row-major order.
//
// In practice, things like strided access can come into play and the processing bottleneck can be either core bound or
// memory bound. When the CPU fetches a cache line contiguous access is typically more efficient, but this can be
// outweighed by the amount of processing that needs to be done. In many cases for work that is core bound, the CPU can
// fetch the next appropriate cache line before an iteration of the loop has completed, in these cases loop_interchange
// may not help.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <cstddef> //size_t

constexpr std::size_t N = 100;

void loop_interchange_1() {
    int a[N][N], b[N][N], c[N][N];
    for (std::size_t i = 0; i < N; i++) {
        for (std::size_t j = 0; j < N; j++) {
            for (std::size_t k = 0; k < N; k++) {
                a[i][j] = b[i][k] + c[k][j];
                // a indexes i then j : row-major
                // b indexes i then k : row-major
                // c indexes k then j : column-major
            }
        }
    }
}

void loop_interchange_2() {
    int a[N][N], b[N][N], c[N][N];
    for (std::size_t i = 0; i < N; i++) {
        for (std::size_t k = 0; k < N; k++) {
            for (std::size_t j = 0; j < N; j++) {
                a[i][j] = b[i][k] + c[k][j];
                // a indexes i then j : row-major
                // b indexes i then k : row-major
                // c indexes k then j : row-major
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// Loop fusion (also see expression templates)
//
// The combining of loops...
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// Loop fission - break up a large loop into multiple smaller ones
//
// The combining of loops...
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// Data dependency
//
// In the first loop here there is a data dependency in the use of b[i] and the next iteration of
// b[i+1]. This is removed in the second loop by doing the first calculation before the loop which
// allows the compiler to vectorise the loop using simd
///////////////////////////////////////////////////////////////////////////////////////////////////

int data_dependancy_1(int (&a)[1000], int (&b)[1000], int (&c)[1000]) {
    for (int i = 0; i <= 998; ++i) {
        a[i]   += b[i];
        b[i+1] += c[i];
    }
    return b[999];
}

int data_dependancy_2(int (&a)[1000], int (&b)[1000], int (&c)[1000]) {
    a[0] += b[0];

    for (int i = 1; i < 998; ++i) {
        b[i+1] += c[i];
        a[i+1] += b[i+1];
    }
    return b[999];
}

///////////////////////////////////////////////////////////////////////////////////////////////////