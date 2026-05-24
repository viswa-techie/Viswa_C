/*
 * armstrong.c
 * ===========
 * Problem: Check if a number is an Armstrong (Narcissistic) number.
 * An n-digit number is Armstrong if sum of each digit raised to power n equals the number.
 * Example: 153 = 1^3 + 5^3 + 3^3 = 1 + 125 + 27 = 153
 *
 * Category: Foundation > A. Basic Number Problems
 *
 * Compile: gcc -Wall -Wextra -std=c11 -O2 -o armstrong armstrong.c -lm
 * Debug:   gcc -fsanitize=address,undefined -g -std=c11 -o armstrong armstrong.c -lm
 */

#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>
#include <assert.h>

/* ============================================================
 * HELPER: Count digits in a positive integer
 * ============================================================ */
static int count_digits(int n) {
    if (n == 0) return 1;
    int count = 0;
    while (n > 0) { count++; n /= 10; }
    return count;
}

/* ============================================================
 * HELPER: Integer power (base^exp) using long long
 * Avoids floating-point error for integer exponents.
 * ============================================================ */
static long long ipow(long long base, int exp) {
    long long result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* ============================================================
 * APPROACH 1: USING pow() from math.h
 * Time: O(log n * log k), Space: O(1)
 * WARNING: pow() is floating-point — may have rounding errors.
 * Use only for demonstration; prefer integer power.
 * ============================================================ */
bool is_armstrong_pow(int n) {
    if (n < 0) return false;
    if (n < 10) return true;  /* all single-digit numbers are Armstrong */

    int k = count_digits(n);
    int original = n;
    long long sum = 0;

    while (n > 0) {
        int digit = n % 10;
        sum += (long long)round(pow((double)digit, k));  /* round to fix FP error */
        n /= 10;
    }
    return sum == (long long)original;
}

/* ============================================================
 * APPROACH 2: INTEGER POWER — No floating point [PREFERRED]
 * Time: O(log n * log k), Space: O(1)
 * ============================================================ */
bool is_armstrong_ipow(int n) {
    if (n < 0) return false;
    if (n < 10) return true;

    int k = count_digits(n);
    int original = n;
    long long sum = 0;

    while (n > 0) {
        int digit = n % 10;
        sum += ipow((long long)digit, k);
        if (sum > (long long)original + 1) return false;  /* early exit */
        n /= 10;
    }
    return sum == (long long)original;
}

/* ============================================================
 * APPROACH 3: DIGIT ARRAY PRECOMPUTED
 * Store digits, count k, then sum digit^k.
 * Useful when you need to reuse digit list.
 * Time: O(log n * log k), Space: O(log n)
 * ============================================================ */
bool is_armstrong_digit_array(int n) {
    if (n < 0) return false;
    if (n < 10) return true;

    int digits[10];
    int k = 0;
    int temp = n;

    while (temp > 0) {
        digits[k++] = temp % 10;
        temp /= 10;
    }

    long long sum = 0;
    for (int i = 0; i < k; i++) {
        sum += ipow((long long)digits[i], k);
    }
    return sum == (long long)n;
}

/* ============================================================
 * APPROACH 4: RECURSIVE
 * Time: O(log n * log k), Space: O(log n)
 * ============================================================ */
static long long armstrong_rec(int n, int k) {
    if (n == 0) return 0;
    return ipow((long long)(n % 10), k) + armstrong_rec(n / 10, k);
}

bool is_armstrong_recursive(int n) {
    if (n < 0) return false;
    if (n < 10) return true;
    int k = count_digits(n);
    return armstrong_rec(n, k) == (long long)n;
}

/* ============================================================
 * APPROACH 5: PRECOMPUTED LIST (for range queries)
 * Narcissistic numbers are finite in any base.
 * In base-10 they are: 1-9, 153, 370, 371, 407, 1634, 8208, 9474, ...
 * Time: O(1) lookup, Space: O(1) table
 * ============================================================ */
static const int armstrong_table[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    153, 370, 371, 407,             /* 3-digit */
    1634, 8208, 9474,               /* 4-digit */
    54748, 92727, 93084,            /* 5-digit */
    548834,                         /* 6-digit */
    1741725, 4210818, 9800817, 9926315,  /* 7-digit */
    24678050, 24678051, 88593477    /* 8-digit */
};
static const int table_size = (int)(sizeof(armstrong_table) / sizeof(armstrong_table[0]));

bool is_armstrong_table(int n) {
    if (n < 0) return false;
    for (int i = 0; i < table_size; i++) {
        if (armstrong_table[i] == n) return true;
        if (armstrong_table[i] > n)  return false;  /* table is sorted */
    }
    return false;
}

/* ============================================================
 * DRY RUN DEMO
 * ============================================================ */
static void dry_run(int n) {
    printf("\n--- Dry Run: is_armstrong(%d) ---\n", n);
    if (n < 0) { printf("  Negative → false\n"); return; }

    int k = count_digits(n);
    printf("  Digits: k = %d\n", k);

    int temp = n;
    long long sum = 0;
    while (temp > 0) {
        int d = temp % 10;
        long long contrib = ipow((long long)d, k);
        printf("  digit=%d, %d^%d = %lld\n", d, d, k, contrib);
        sum += contrib;
        temp /= 10;
    }
    printf("  sum = %lld, n = %d → %s\n", sum, n,
           sum == (long long)n ? "ARMSTRONG" : "NOT ARMSTRONG");
}

/* ============================================================
 * PRINT ALL ARMSTRONG NUMBERS IN RANGE
 * ============================================================ */
static void print_armstrong_range(int limit) {
    printf("\n=== Armstrong Numbers up to %d ===\n", limit);
    int count = 0;
    for (int i = 0; i <= limit; i++) {
        if (is_armstrong_ipow(i)) {
            printf("%d ", i);
            count++;
        }
    }
    printf("\nTotal: %d\n", count);
}

/* ============================================================
 * TEST SUITE
 * ============================================================ */
static void run_tests(void) {
    printf("\n=== Running Tests ===\n");

    /* Known Armstrong numbers */
    int known[] = {0,1,2,3,4,5,6,7,8,9, 153,370,371,407, 1634,8208,9474};
    int known_count = (int)(sizeof(known)/sizeof(known[0]));
    for (int i = 0; i < known_count; i++) {
        assert(is_armstrong_ipow(known[i])  == true);
        assert(is_armstrong_pow(known[i])   == true);
        assert(is_armstrong_table(known[i]) == true);
    }

    /* Non-Armstrong numbers */
    int non_arm[] = {10, 11, 100, 152, 154, 200, 1000, 9999};
    int na_count = (int)(sizeof(non_arm)/sizeof(non_arm[0]));
    for (int i = 0; i < na_count; i++) {
        assert(is_armstrong_ipow(non_arm[i])  == false);
        assert(is_armstrong_table(non_arm[i]) == false);
    }

    /* Edge cases */
    assert(is_armstrong_ipow(-1)  == false);
    assert(is_armstrong_ipow(-153)== false);
    assert(is_armstrong_ipow(0)   == true);

    /* Cross-validate approaches [0, 10000] */
    for (int n = 0; n <= 10000; n++) {
        bool r1 = is_armstrong_ipow(n);
        bool r2 = is_armstrong_pow(n);
        bool r3 = is_armstrong_digit_array(n);
        bool r4 = is_armstrong_recursive(n);
        if (r1 != r2 || r2 != r3 || r3 != r4) {
            printf("MISMATCH at n=%d\n", n);
            assert(false);
        }
    }

    printf("All tests PASSED.\n");
}

/* ============================================================
 * MAIN
 * ============================================================ */
int main(void) {
    printf("=========================================\n");
    printf(" Armstrong Number — All Approaches\n");
    printf("=========================================\n");

    run_tests();

    dry_run(153);
    dry_run(370);
    dry_run(371);
    dry_run(407);
    dry_run(1634);
    dry_run(100);

    print_armstrong_range(10000);

    printf("\n=========================================\n");
    printf(" Complexity Summary\n");
    printf("=========================================\n");
    printf("%-25s %-15s %-10s %-20s\n", "Approach", "Time", "Space", "Notes");
    printf("%-25s %-15s %-10s %-20s\n", "pow() based",     "O(d log d)", "O(1)", "FP rounding risk");
    printf("%-25s %-15s %-10s %-20s\n", "ipow integer",    "O(d log d)", "O(1)", "Exact, preferred");
    printf("%-25s %-15s %-10s %-20s\n", "Digit array",     "O(d log d)", "O(d)", "Two-pass");
    printf("%-25s %-15s %-10s %-20s\n", "Recursive",       "O(d log d)", "O(d)", "Stack frames");
    printf("%-25s %-15s %-10s %-20s\n", "Table lookup",    "O(1)",       "O(1)", "Finite set");
    printf("\nd = number of digits = O(log n)\n");

    return 0;
}
