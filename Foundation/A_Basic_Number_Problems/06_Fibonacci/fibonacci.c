/*
 * fibonacci.c
 * ===========
 * Problem: Compute the nth Fibonacci number.
 * F(0)=0, F(1)=1, F(n)=F(n-1)+F(n-2) for n>=2
 *
 * Category: Foundation > A. Basic Number Problems
 *
 * Compile: gcc -Wall -Wextra -std=c11 -O2 -o fibonacci fibonacci.c
 * Debug:   gcc -fsanitize=address,undefined -g -std=c11 -o fibonacci fibonacci.c
 *
 * Overflow: F(93) is the last Fibonacci that fits in uint64_t.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

typedef unsigned long long ull;

/* ============================================================
 * APPROACH 1: NAIVE RECURSIVE (Exponential — DO NOT USE)
 * Time: O(2^n), Space: O(n) stack
 * Only for illustration of exponential branching.
 * ============================================================ */
ull fibonacci_naive(int n) {
    if (n < 0)  return 0;
    if (n == 0) return 0;
    if (n == 1) return 1;
    return fibonacci_naive(n - 1) + fibonacci_naive(n - 2);
}

/* ============================================================
 * APPROACH 2: MEMOIZED RECURSIVE (Top-Down DP)
 * Time: O(n), Space: O(n)
 * ============================================================ */
#define MEMO_SIZE 94

static ull fib_memo[MEMO_SIZE];
static bool fib_memo_init = false;

static void fib_init_memo(void) {
    if (fib_memo_init) return;
    memset(fib_memo, 0xFF, sizeof(fib_memo));  /* 0xFF...FF = unset */
    fib_memo[0] = 0;
    fib_memo[1] = 1;
    fib_memo_init = true;
}

ull fibonacci_memoized(int n) {
    if (n < 0 || n >= MEMO_SIZE) return 0;
    fib_init_memo();
    if (fib_memo[n] != (ull)(-1)) return fib_memo[n];
    fib_memo[n] = fibonacci_memoized(n - 1) + fibonacci_memoized(n - 2);
    return fib_memo[n];
}

/* ============================================================
 * APPROACH 3: ITERATIVE (Bottom-Up DP)  [PREFERRED for firmware]
 * Time: O(n), Space: O(1)
 * ============================================================ */
ull fibonacci_iterative(int n) {
    if (n < 0)  return 0;
    if (n == 0) return 0;
    if (n == 1) return 1;

    ull prev = 0, curr = 1;
    for (int i = 2; i <= n; i++) {
        ull next = prev + curr;
        prev = curr;
        curr = next;
    }
    return curr;
}

/* ============================================================
 * APPROACH 4: SPACE-OPTIMIZED — Two-variable iterative
 * (Same as above but with explicit variable naming)
 * Time: O(n), Space: O(1)
 * ============================================================ */
ull fibonacci_two_var(int n) {
    if (n < 0)  return 0;
    ull a = 0, b = 1;    /* F(0), F(1) */
    for (int i = 0; i < n; i++) {
        ull t = a + b;
        a = b;
        b = t;
    }
    return a;  /* after n iterations, a = F(n) */
}

/* ============================================================
 * APPROACH 5: LOOKUP TABLE (O(1))
 * F(93) is the last Fibonacci that fits in uint64_t.
 * ============================================================ */
static const ull fib_table[94] = {
    0ULL, 1ULL, 1ULL, 2ULL, 3ULL, 5ULL, 8ULL, 13ULL, 21ULL, 34ULL,
    55ULL, 89ULL, 144ULL, 233ULL, 377ULL, 610ULL, 987ULL, 1597ULL,
    2584ULL, 4181ULL, 6765ULL, 10946ULL, 17711ULL, 28657ULL, 46368ULL,
    75025ULL, 121393ULL, 196418ULL, 317811ULL, 514229ULL, 832040ULL,
    1346269ULL, 2178309ULL, 3524578ULL, 5702887ULL, 9227465ULL,
    14930352ULL, 24157817ULL, 39088169ULL, 63245986ULL, 102334155ULL,
    165580141ULL, 267914296ULL, 433494437ULL, 701408733ULL,
    1134903170ULL, 1836311903ULL, 2971215073ULL, 4807526976ULL,
    7778742049ULL, 12586269025ULL, 20365011074ULL, 32951280099ULL,
    53316291173ULL, 86267571272ULL, 139583862445ULL, 225851433717ULL,
    365435296162ULL, 591286729879ULL, 956722026041ULL, 1548008755920ULL,
    2504730781961ULL, 4052739537881ULL, 6557470319842ULL,
    10610209857723ULL, 17167680177565ULL, 27777890035288ULL,
    44945570212853ULL, 72723460248141ULL, 117669030460994ULL,
    190392490709135ULL, 308061521170129ULL, 498454011879264ULL,
    806515533049393ULL, 1304969544928657ULL, 2111485077978050ULL,
    3416454622906707ULL, 5527939700884757ULL, 8944394323791464ULL,
    14472334024676221ULL, 23416728348467685ULL, 37889062373143906ULL,
    61305790721611591ULL, 99194853094755497ULL, 160500643816367088ULL,
    259695496911122585ULL, 420196140727489673ULL, 679891637638612258ULL,
    1100087778366101931ULL, 1779979416004714189ULL, 2880067194370816120ULL,
    4660046610375530309ULL, 7540113804746346429ULL
};

ull fibonacci_lut(int n) {
    if (n < 0 || n >= 94) return 0;
    return fib_table[n];
}

/* ============================================================
 * APPROACH 6: MATRIX EXPONENTIATION (O(log n))
 * [F(n+1) F(n)  ] = [1 1]^n
 * [F(n)   F(n-1)]   [1 0]
 * Time: O(log n), Space: O(1)
 * ============================================================ */
typedef struct { ull a[2][2]; } Matrix2x2;

static Matrix2x2 mat_mul(Matrix2x2 A, Matrix2x2 B) {
    Matrix2x2 C;
    C.a[0][0] = A.a[0][0]*B.a[0][0] + A.a[0][1]*B.a[1][0];
    C.a[0][1] = A.a[0][0]*B.a[0][1] + A.a[0][1]*B.a[1][1];
    C.a[1][0] = A.a[1][0]*B.a[0][0] + A.a[1][1]*B.a[1][0];
    C.a[1][1] = A.a[1][0]*B.a[0][1] + A.a[1][1]*B.a[1][1];
    return C;
}

static Matrix2x2 mat_pow(Matrix2x2 M, int p) {
    Matrix2x2 result = {{{1,0},{0,1}}};  /* identity matrix */
    while (p > 0) {
        if (p & 1) result = mat_mul(result, M);
        M = mat_mul(M, M);
        p >>= 1;
    }
    return result;
}

ull fibonacci_matrix(int n) {
    if (n < 0)  return 0;
    if (n == 0) return 0;
    Matrix2x2 base = {{{1,1},{1,0}}};
    Matrix2x2 result = mat_pow(base, n);
    return result.a[0][1];  /* F(n) */
}

/* ============================================================
 * APPROACH 7: FAST DOUBLING (O(log n), elegant)
 * Based on identities:
 *   F(2k)   = F(k) * (2*F(k+1) - F(k))
 *   F(2k+1) = F(k)^2 + F(k+1)^2
 * Time: O(log n), Space: O(log n) recursive or O(1) iterative
 * ============================================================ */
static void fast_doubling(int n, ull *fk, ull *fk1) {
    if (n == 0) { *fk = 0; *fk1 = 1; return; }
    ull a, b;
    fast_doubling(n >> 1, &a, &b);
    ull c = a * (2*b - a);   /* F(2k)   */
    ull d = a*a + b*b;        /* F(2k+1) */
    if (n & 1) { *fk = d; *fk1 = c + d; }
    else       { *fk = c; *fk1 = d; }
}

ull fibonacci_fast_doubling(int n) {
    if (n < 0) return 0;
    ull fk, fk1;
    fast_doubling(n, &fk, &fk1);
    return fk;
}

/* ============================================================
 * APPROACH 8: BINET'S FORMULA (Closed-form, O(1) — FP only)
 * F(n) = round( phi^n / sqrt(5) )
 * phi = (1 + sqrt(5)) / 2 ≈ 1.6180339887
 * WARNING: Loses precision for large n (n > ~75 on double)
 * ============================================================ */
#include <math.h>

ull fibonacci_binet(int n) {
    if (n < 0)  return 0;
    if (n == 0) return 0;
    if (n == 1) return 1;
    const double phi   = 1.6180339887498948482;
    const double sqrt5 = 2.2360679774997896964;
    return (ull)round(pow(phi, n) / sqrt5);
}

/* ============================================================
 * DRY RUN
 * ============================================================ */
static void dry_run_iterative(int n) {
    printf("\n--- Dry Run: fibonacci_iterative(%d) ---\n", n);
    if (n < 0) { printf("  n < 0 → 0\n"); return; }
    ull prev = 0, curr = 1;
    if (n == 0) { printf("  F(0) = 0\n"); return; }
    printf("  F(0)=0, F(1)=1\n");
    for (int i = 2; i <= n; i++) {
        ull next = prev + curr;
        printf("  F(%d) = F(%d) + F(%d) = %llu + %llu = %llu\n",
               i, i-1, i-2, curr, prev, next);
        prev = curr;
        curr = next;
    }
    printf("  F(%d) = %llu\n", n, curr);
}

/* ============================================================
 * TEST SUITE
 * ============================================================ */
static void run_tests(void) {
    printf("\n=== Running Tests ===\n");

    /* Known values */
    ull expected[] = {0,1,1,2,3,5,8,13,21,34,55,89,144,233,377,610,987};
    int count = (int)(sizeof(expected)/sizeof(expected[0]));
    for (int i = 0; i < count; i++) {
        assert(fibonacci_iterative(i)     == expected[i]);
        assert(fibonacci_two_var(i)       == expected[i]);
        assert(fibonacci_lut(i)           == expected[i]);
        assert(fibonacci_matrix(i)        == expected[i]);
        assert(fibonacci_fast_doubling(i) == expected[i]);
    }

    /* Large values */
    assert(fibonacci_iterative(50) == 12586269025ULL);
    assert(fibonacci_iterative(93) == 12200160415121876738ULL);

    /* Edge */
    assert(fibonacci_iterative(0)  == 0ULL);
    assert(fibonacci_iterative(-1) == 0ULL);

    /* Binet's formula (valid up to ~75) */
    for (int i = 0; i < 70; i++) {
        if (fibonacci_binet(i) != fibonacci_lut(i)) {
            printf("  Binet mismatch at n=%d: binet=%llu lut=%llu\n",
                   i, fibonacci_binet(i), fibonacci_lut(i));
        }
    }

    /* Naive vs iterative for small n (naive is too slow for large) */
    for (int i = 0; i <= 20; i++) {
        assert(fibonacci_naive(i) == fibonacci_iterative(i));
    }

    printf("All tests PASSED.\n");
}

/* ============================================================
 * MAIN
 * ============================================================ */
int main(void) {
    printf("=========================================\n");
    printf(" Fibonacci — All Approaches\n");
    printf("=========================================\n");

    run_tests();

    dry_run_iterative(8);

    printf("\n=== Fibonacci Numbers F(0) to F(20) ===\n");
    for (int i = 0; i <= 20; i++) {
        printf("  F(%2d) = %llu\n", i, fibonacci_lut(i));
    }

    printf("\n=== Comparing Approaches for selected n ===\n");
    printf("%-5s %-20s %-20s %-20s %-20s\n",
           "n", "Iterative", "Matrix", "FastDoubling", "Binet");
    int test_n[] = {0, 1, 5, 10, 20, 30, 50, 70, 93};
    int tn_count = (int)(sizeof(test_n)/sizeof(test_n[0]));
    for (int i = 0; i < tn_count; i++) {
        int n = test_n[i];
        printf("%-5d %-20llu %-20llu %-20llu %-20llu\n",
               n,
               fibonacci_iterative(n),
               fibonacci_matrix(n),
               fibonacci_fast_doubling(n),
               (n <= 75) ? fibonacci_binet(n) : 0ULL);
    }

    printf("\n=========================================\n");
    printf(" Complexity Summary\n");
    printf("=========================================\n");
    printf("%-22s %-12s %-12s %-20s\n", "Approach", "Time", "Space", "Notes");
    printf("%-22s %-12s %-12s %-20s\n", "Naive Recursive",  "O(2^n)", "O(n)",   "NEVER use");
    printf("%-22s %-12s %-12s %-20s\n", "Memoized",         "O(n)",   "O(n)",   "Top-down DP");
    printf("%-22s %-12s %-12s %-20s\n", "Iterative",        "O(n)",   "O(1)",   "Best for firmware");
    printf("%-22s %-12s %-12s %-20s\n", "Two-variable",     "O(n)",   "O(1)",   "Cleanest O(n)");
    printf("%-22s %-12s %-12s %-20s\n", "LUT",              "O(1)",   "O(1)",   "Max n=93, ROM");
    printf("%-22s %-12s %-12s %-20s\n", "Matrix Exp",       "O(log n)","O(1)",  "Exact, any n");
    printf("%-22s %-12s %-12s %-20s\n", "Fast Doubling",    "O(log n)","O(log n)","Elegant");
    printf("%-22s %-12s %-12s %-20s\n", "Binet's Formula",  "O(1)",   "O(1)",   "FP: valid n<75");

    return 0;
}
