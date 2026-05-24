/*
 * digit_sum.c
 * ===========
 * Problem: Compute the sum of digits of a number.
 * Extended: Digital root (repeated digit sum until single digit).
 *
 * Category: Foundation > A. Basic Number Problems
 *
 * Compile: gcc -Wall -Wextra -std=c11 -O2 -o digit_sum digit_sum.c
 * Debug:   gcc -fsanitize=address,undefined -g -std=c11 -o digit_sum digit_sum.c
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

/* ============================================================
 * APPROACH 1: ITERATIVE — Basic Digit Sum
 * Time: O(log n), Space: O(1)
 * ============================================================ */
int digit_sum_iterative(int n) {
    if (n < 0) n = -n;   /* handle negative: sum of absolute digits */
    int sum = 0;
    while (n > 0) {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}

/* ============================================================
 * APPROACH 2: RECURSIVE
 * Time: O(log n), Space: O(log n) — one frame per digit
 * ============================================================ */
int digit_sum_recursive(int n) {
    if (n < 0) n = -n;
    if (n < 10) return n;
    return n % 10 + digit_sum_recursive(n / 10);
}

/* ============================================================
 * APPROACH 3: USING STRING
 * Time: O(log n), Space: O(log n)
 * ============================================================ */
#include <string.h>
int digit_sum_string(int n) {
    if (n < 0) n = -n;
    char buf[12];
    snprintf(buf, sizeof(buf), "%d", n);
    int sum = 0;
    for (int i = 0; buf[i] != '\0'; i++) {
        sum += buf[i] - '0';
    }
    return sum;
}

/* ============================================================
 * APPROACH 4: DIGITAL ROOT — O(1) Mathematical Formula
 * The digital root of n is the single digit obtained by
 * repeatedly summing digits until one digit remains.
 *
 * Formula:
 *   digital_root(0) = 0
 *   digital_root(n) = 1 + (n-1) % 9  for n > 0
 *
 * This is a classic O(1) trick using modulo 9 arithmetic.
 * Time: O(1), Space: O(1)
 * ============================================================ */
int digital_root_formula(int n) {
    if (n < 0) n = -n;
    if (n == 0) return 0;
    int r = n % 9;
    return (r == 0) ? 9 : r;
}

/* Digital root by repeated iteration (for comparison) */
int digital_root_iterative(int n) {
    if (n < 0) n = -n;
    while (n >= 10) {
        n = digit_sum_iterative(n);
    }
    return n;
}

/* ============================================================
 * APPROACH 5: DIGIT SUM USING MODULO PATTERN
 * No division — uses subtraction. Slower but division-free.
 * Time: O(log n * 10), Space: O(1)
 * ============================================================ */
int digit_sum_no_division(int n) {
    if (n < 0) n = -n;
    int sum = 0;
    while (n > 0) {
        /* Extract last digit without division */
        int digit = n;
        while (digit >= 10) digit -= 10;  /* digit = n % 10 without % */
        sum += digit;
        /* Remove last digit: n = n / 10 using subtraction */
        n = (n - digit);   /* now n is divisible by 10 */
        /* Divide by 10 using repeated subtraction — impractical, use /10 */
        n /= 10;
    }
    return sum;
}

/* ============================================================
 * APPROACH 6: SUM OF DIGITS FROM 1 TO N (Count-based)
 * Useful follow-up: "Sum all digits of all numbers 1..N"
 * Time: O(log n), Space: O(1)
 * ============================================================ */
long long sum_of_digits_1_to_n(long long n) {
    if (n <= 0) return 0;

    /*
     * Pattern: sum of digits of numbers 1..10^k-1 = 45 * k * 10^(k-1)
     * For each position, digits cycle 0-9.
     * Use digit-by-digit decomposition.
     */
    long long total = 0;
    long long power = 1;     /* current positional power */
    long long rem = n;

    while (rem > 0) {
        long long digit = rem % 10;
        rem /= 10;

        /* Complete cycles: rem full groups of 10*power numbers */
        total += rem * digit * power;
        /* Sum 0..digit-1 for current position */
        total += (digit * (digit - 1) / 2) * power;
        /* Add contribution from remaining numbers in current cycle */
        total += digit * (n % power + 1);

        power *= 10;
    }
    return total;
}

/* ============================================================
 * APPROACH 7: DIGIT PRODUCT
 * Product of all digits (bonus variant).
 * Time: O(log n), Space: O(1)
 * ============================================================ */
int digit_product(int n) {
    if (n < 0) n = -n;
    if (n == 0) return 0;
    int prod = 1;
    while (n > 0) {
        int d = n % 10;
        if (d == 0) return 0;  /* any zero digit → product is 0 */
        prod *= d;
        n /= 10;
    }
    return prod;
}

/* ============================================================
 * DRY RUN
 * ============================================================ */
static void dry_run(int n) {
    printf("\n--- Dry Run: digit_sum(%d) ---\n", n);
    int num = (n < 0) ? -n : n;
    int sum = 0;
    printf("  Digits: ");
    while (num > 0) {
        int d = num % 10;
        printf("%d ", d);
        sum += d;
        num /= 10;
    }
    printf("\n  Sum = %d\n", sum);
    printf("  Digital root = %d (formula: %d)\n",
           digital_root_iterative(n), digital_root_formula(n));
}

/* ============================================================
 * TEST SUITE
 * ============================================================ */
static void run_tests(void) {
    printf("\n=== Running Tests ===\n");

    /* Basic digit sums */
    assert(digit_sum_iterative(0)   == 0);
    assert(digit_sum_iterative(1)   == 1);
    assert(digit_sum_iterative(9)   == 9);
    assert(digit_sum_iterative(10)  == 1);
    assert(digit_sum_iterative(99)  == 18);
    assert(digit_sum_iterative(123) == 6);
    assert(digit_sum_iterative(999) == 27);
    assert(digit_sum_iterative(-45) == 9);  /* handles negative */

    /* Digital root formula vs iterative */
    int test_vals[] = {0, 1, 9, 10, 18, 19, 99, 100, 999, 12345};
    int tc = (int)(sizeof(test_vals)/sizeof(test_vals[0]));
    for (int i = 0; i < tc; i++) {
        int formula = digital_root_formula(test_vals[i]);
        int iter    = digital_root_iterative(test_vals[i]);
        if (formula != iter) {
            printf("  MISMATCH digital root n=%d: formula=%d iter=%d\n",
                   test_vals[i], formula, iter);
            assert(false);
        }
    }

    /* Cross-validate all approaches */
    for (int n = 0; n <= 10000; n++) {
        int r1 = digit_sum_iterative(n);
        int r2 = digit_sum_recursive(n);
        int r3 = digit_sum_string(n);
        if (r1 != r2 || r2 != r3) {
            printf("MISMATCH at n=%d: iter=%d rec=%d str=%d\n", n, r1, r2, r3);
            assert(false);
        }
    }

    /* Digit product */
    assert(digit_product(234) == 24);
    assert(digit_product(0)   == 0);
    assert(digit_product(100) == 0);   /* contains 0 */
    assert(digit_product(999) == 729);

    printf("All tests PASSED.\n");
}

/* ============================================================
 * MAIN
 * ============================================================ */
int main(void) {
    printf("=========================================\n");
    printf(" Digit Sum & Digital Root\n");
    printf("=========================================\n");

    run_tests();

    dry_run(12345);
    dry_run(9999);
    dry_run(100);

    printf("\n=== Digit Sum vs Digital Root ===\n");
    printf("%-10s %-12s %-12s %-12s\n", "n", "digit_sum", "digital_root", "formula");
    int show[] = {0, 9, 10, 19, 99, 100, 999, 12345, 2147483647};
    int sc = (int)(sizeof(show)/sizeof(show[0]));
    for (int i = 0; i < sc; i++) {
        printf("%-10d %-12d %-12d %-12d\n",
               show[i],
               digit_sum_iterative(show[i]),
               digital_root_iterative(show[i]),
               digital_root_formula(show[i]));
    }

    printf("\n=========================================\n");
    printf(" Complexity Summary\n");
    printf("=========================================\n");
    printf("%-25s %-12s %-10s\n", "Approach", "Time", "Space");
    printf("%-25s %-12s %-10s\n", "Iterative",      "O(log n)", "O(1)");
    printf("%-25s %-12s %-10s\n", "Recursive",      "O(log n)", "O(log n)");
    printf("%-25s %-12s %-10s\n", "String-Based",   "O(log n)", "O(log n)");
    printf("%-25s %-12s %-10s\n", "Digital Root",   "O(1)",     "O(1)");
    printf("%-25s %-12s %-10s\n", "Digit Product",  "O(log n)", "O(1)");
    printf("%-25s %-12s %-10s\n", "Sum 1..N",       "O(log n)", "O(1)");

    return 0;
}
