/*
 * factorial.c
 * ===========
 * Problem: Compute n! (factorial of n).
 * Category: Foundation > A. Basic Number Problems
 *
 * Compile: gcc -Wall -Wextra -std=c11 -O2 -o factorial factorial.c
 * Debug:   gcc -fsanitize=address,undefined -g -std=c11 -o factorial factorial.c
 *
 * Key overflow facts:
 *   12! = 479001600    (fits in int32)
 *   13! = 6227020800   (overflows int32)
 *   20! = 2432902008176640000  (fits in uint64)
 *   21! overflows uint64
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

/* ============================================================
 * APPROACH 1: ITERATIVE (PREFERRED for embedded/firmware)
 * Time: O(n), Space: O(1)
 * Safe up to n=20 using unsigned long long.
 * ============================================================ */
unsigned long long factorial_iterative(int n) {
    if (n < 0) return 0;   /* undefined for negative */
    if (n == 0 || n == 1) return 1ULL;

    unsigned long long result = 1;
    for (int i = 2; i <= n; i++) {
        result *= (unsigned long long)i;
    }
    return result;
}

/* ============================================================
 * APPROACH 2: RECURSIVE
 * Time: O(n), Space: O(n) — call stack depth = n
 * Risk: stack overflow for large n (n > 10000 on typical systems)
 * ============================================================ */
unsigned long long factorial_recursive(int n) {
    if (n < 0) return 0;
    if (n == 0 || n == 1) return 1ULL;
    return (unsigned long long)n * factorial_recursive(n - 1);
}

/* ============================================================
 * APPROACH 3: TAIL-RECURSIVE (Compiler may optimize to iteration)
 * Time: O(n), Space: O(1) if tail-call optimized
 * ============================================================ */
static unsigned long long factorial_tail_helper(int n, unsigned long long acc) {
    if (n <= 1) return acc;
    return factorial_tail_helper(n - 1, acc * (unsigned long long)n);
}

unsigned long long factorial_tail_recursive(int n) {
    if (n < 0) return 0;
    return factorial_tail_helper(n, 1ULL);
}

/* ============================================================
 * APPROACH 4: LOOKUP TABLE (O(1) — for small fixed n)
 * Factorials grow fast — only 21 valid uint64 values.
 * Ideal for embedded systems: precomputed in ROM.
 * ============================================================ */
static const unsigned long long fact_table[21] = {
    1ULL,           /* 0! */
    1ULL,           /* 1! */
    2ULL,           /* 2! */
    6ULL,           /* 3! */
    24ULL,          /* 4! */
    120ULL,         /* 5! */
    720ULL,         /* 6! */
    5040ULL,        /* 7! */
    40320ULL,       /* 8! */
    362880ULL,      /* 9! */
    3628800ULL,     /* 10! */
    39916800ULL,    /* 11! */
    479001600ULL,   /* 12! */
    6227020800ULL,  /* 13! */
    87178291200ULL, /* 14! */
    1307674368000ULL,  /* 15! */
    20922789888000ULL, /* 16! */
    355687428096000ULL,/* 17! */
    6402373705728000ULL,  /* 18! */
    121645100408832000ULL,/* 19! */
    2432902008176640000ULL/* 20! */
};

unsigned long long factorial_lut(int n) {
    if (n < 0 || n > 20) return 0;   /* out of range */
    return fact_table[n];
}

/* ============================================================
 * APPROACH 5: MEMOIZED RECURSIVE (Cache repeated calls)
 * Time: O(n) first call, O(1) subsequent
 * Space: O(n) cache
 * ============================================================ */
static unsigned long long memo[21] = {0};
static bool memo_initialized = false;

static void init_memo(void) {
    if (memo_initialized) return;
    memo[0] = 1;
    memo[1] = 1;
    for (int i = 2; i <= 20; i++) {
        memo[i] = (unsigned long long)i * memo[i-1];
    }
    memo_initialized = true;
}

unsigned long long factorial_memoized(int n) {
    if (n < 0 || n > 20) return 0;
    init_memo();
    return memo[n];
}

/* ============================================================
 * APPROACH 6: BIG INTEGER STRING (No overflow, any n)
 * Multiply digit-by-digit to support n > 20.
 * Time: O(n^2 * log n), Space: O(n * log n) for result string
 * ============================================================ */
#define BIGINT_MAX_DIGITS 200

static void bigint_multiply(int *digits, int *num_digits, int factor) {
    int carry = 0;
    for (int i = 0; i < *num_digits; i++) {
        int prod = digits[i] * factor + carry;
        digits[i] = prod % 10;
        carry = prod / 10;
    }
    while (carry > 0) {
        digits[(*num_digits)++] = carry % 10;
        carry /= 10;
    }
}

void factorial_bigint(int n, char *result_str, int str_size) {
    if (n < 0 || str_size < 2) {
        snprintf(result_str, (size_t)str_size, "0");
        return;
    }

    int digits[BIGINT_MAX_DIGITS] = {0};
    int num_digits = 1;
    digits[0] = 1;  /* start with 1 */

    for (int i = 2; i <= n; i++) {
        bigint_multiply(digits, &num_digits, i);
        if (num_digits >= BIGINT_MAX_DIGITS - 1) {
            snprintf(result_str, (size_t)str_size, "OVERFLOW");
            return;
        }
    }

    /* Convert to string (digits stored LSD first, reverse for output) */
    int pos = 0;
    for (int i = num_digits - 1; i >= 0 && pos < str_size - 1; i--) {
        result_str[pos++] = (char)('0' + digits[i]);
    }
    result_str[pos] = '\0';
}

/* ============================================================
 * APPROACH 7: OVERFLOW-DETECTING ITERATIVE
 * Returns false and sets result=0 if overflow would occur.
 * ============================================================ */
bool factorial_safe(int n, unsigned long long *result) {
    if (n < 0 || !result) return false;
    if (n > 20) return false;   /* known overflow boundary for uint64 */
    *result = fact_table[n];
    return true;
}

/* ============================================================
 * DRY RUN
 * ============================================================ */
static void dry_run_iterative(int n) {
    printf("\n--- Dry Run: factorial_iterative(%d) ---\n", n);
    if (n < 0) { printf("  n < 0 → undefined\n"); return; }
    unsigned long long result = 1;
    printf("  result = 1\n");
    for (int i = 2; i <= n; i++) {
        result *= (unsigned long long)i;
        printf("  result *= %d → %llu\n", i, result);
    }
    printf("  %d! = %llu\n", n, result);
}

/* ============================================================
 * TEST SUITE
 * ============================================================ */
static void run_tests(void) {
    printf("\n=== Running Tests ===\n");

    /* Base cases */
    assert(factorial_iterative(0)  == 1ULL);
    assert(factorial_iterative(1)  == 1ULL);
    assert(factorial_iterative(2)  == 2ULL);
    assert(factorial_iterative(3)  == 6ULL);
    assert(factorial_iterative(4)  == 24ULL);
    assert(factorial_iterative(5)  == 120ULL);
    assert(factorial_iterative(10) == 3628800ULL);
    assert(factorial_iterative(12) == 479001600ULL);
    assert(factorial_iterative(20) == 2432902008176640000ULL);

    /* Edge: negative → 0 */
    assert(factorial_iterative(-1) == 0ULL);
    assert(factorial_iterative(-5) == 0ULL);

    /* Cross-validate all approaches for n = 0..20 */
    for (int n = 0; n <= 20; n++) {
        unsigned long long r1 = factorial_iterative(n);
        unsigned long long r2 = factorial_recursive(n);
        unsigned long long r3 = factorial_tail_recursive(n);
        unsigned long long r4 = factorial_lut(n);
        unsigned long long r5 = factorial_memoized(n);
        if (r1 != r2 || r2 != r3 || r3 != r4 || r4 != r5) {
            printf("MISMATCH at n=%d: iter=%llu rec=%llu tail=%llu lut=%llu memo=%llu\n",
                   n, r1, r2, r3, r4, r5);
            assert(false);
        }
    }

    /* Big integer test */
    char buf[200];
    factorial_bigint(25, buf, sizeof(buf));
    printf("  25! = %s\n", buf);
    assert(buf[0] != '0');  /* should not be 0 */

    printf("All tests PASSED.\n");
}

/* ============================================================
 * MAIN
 * ============================================================ */
int main(void) {
    printf("=========================================\n");
    printf(" Factorial — All Approaches\n");
    printf("=========================================\n");

    run_tests();

    dry_run_iterative(6);

    printf("\n=== Factorial Table (0! to 20!) ===\n");
    for (int i = 0; i <= 20; i++) {
        printf("  %2d! = %20llu\n", i, factorial_lut(i));
    }

    printf("\n=== Big Integer Factorials ===\n");
    char buf[200];
    int big_tests[] = {21, 25, 30, 50, 100};
    int bt_count = (int)(sizeof(big_tests)/sizeof(big_tests[0]));
    for (int i = 0; i < bt_count; i++) {
        factorial_bigint(big_tests[i], buf, sizeof(buf));
        printf("  %d! = %s\n", big_tests[i], buf);
    }

    printf("\n=========================================\n");
    printf(" Complexity Summary\n");
    printf("=========================================\n");
    printf("%-22s %-12s %-12s %-20s\n", "Approach", "Time", "Space", "Notes");
    printf("%-22s %-12s %-12s %-20s\n", "Iterative",      "O(n)", "O(1)",   "Best for firmware");
    printf("%-22s %-12s %-12s %-20s\n", "Recursive",      "O(n)", "O(n)",   "Stack depth = n");
    printf("%-22s %-12s %-12s %-20s\n", "Tail Recursive", "O(n)", "O(1)*",  "*if TCO");
    printf("%-22s %-12s %-12s %-20s\n", "LUT",            "O(1)", "O(1)",   "Max n=20, ROM");
    printf("%-22s %-12s %-12s %-20s\n", "Memoized",       "O(n)", "O(n)",   "Cache for repeated");
    printf("%-22s %-12s %-12s %-20s\n", "Big Integer",    "O(n^2 log n)", "O(n log n)", "Arbitrary n");

    return 0;
}
