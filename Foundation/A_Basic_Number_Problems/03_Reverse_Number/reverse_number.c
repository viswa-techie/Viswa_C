/*
 * reverse_number.c
 * ================
 * Problem: Reverse the digits of an integer.
 * Category: Foundation > A. Basic Number Problems
 *
 * Compile: gcc -Wall -Wextra -std=c11 -O2 -o reverse reverse_number.c
 * Debug:   gcc -fsanitize=address,undefined -g -std=c11 -o reverse reverse_number.c
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>
#include <assert.h>

/* ============================================================
 * APPROACH 1: BASIC LOOP (No overflow check)
 * Time: O(log n), Space: O(1)
 * WARNING: May overflow for large n — educational only
 * ============================================================ */
long long reverse_basic(int n) {
    long long result = 0;
    int sign = (n < 0) ? -1 : 1;
    long long num = (n < 0) ? -(long long)n : (long long)n;

    while (num > 0) {
        result = result * 10 + num % 10;
        num /= 10;
    }
    return sign * result;
}

/* ============================================================
 * APPROACH 2: OVERFLOW-SAFE WITH int32 BOUNDS CHECK [PREFERRED]
 * Time: O(log n), Space: O(1)
 *
 * Checks before each digit append whether result would overflow INT_MAX/INT_MIN.
 * Returns INT_MIN as sentinel for overflow (or use a separate flag).
 * ============================================================ */
int reverse_overflow_safe(int n) {
    int result = 0;

    while (n != 0) {
        int digit = n % 10;  /* C99: result has sign of dividend */
        n /= 10;

        /*
         * Overflow check BEFORE modifying result:
         * If result > INT_MAX/10, then result*10 > INT_MAX
         * If result == INT_MAX/10 and digit > 7 → overflow (INT_MAX ends in 7)
         * Mirror for negative.
         */
        if (result > INT_MAX / 10 || (result == INT_MAX / 10 && digit > 7)) {
            return INT_MIN;  /* overflow sentinel */
        }
        if (result < INT_MIN / 10 || (result == INT_MIN / 10 && digit < -8)) {
            return INT_MIN;  /* underflow sentinel */
        }

        result = result * 10 + digit;
    }
    return result;
}

/* ============================================================
 * APPROACH 3: STRING-BASED REVERSAL
 * Time: O(log n), Space: O(log n)
 * ============================================================ */
long long reverse_string(int n) {
    char buf[14];  /* sign + 10 digits + null + extra */
    int sign = 1;
    unsigned int abs_n;

    if (n < 0) {
        sign = -1;
        /* Handle INT_MIN safely: cast to unsigned before negation */
        abs_n = (n == INT_MIN) ? (unsigned int)INT_MAX + 1u : (unsigned int)(-n);
    } else {
        abs_n = (unsigned int)n;
    }

    snprintf(buf, sizeof(buf), "%u", abs_n);

    int len = (int)strlen(buf);
    int left = 0, right = len - 1;
    while (left < right) {
        char tmp = buf[left];
        buf[left] = buf[right];
        buf[right] = tmp;
        left++;
        right--;
    }

    long long result = 0;
    for (int i = 0; i < len; i++) {
        result = result * 10 + (buf[i] - '0');
    }
    return (long long)sign * result;
}

/* ============================================================
 * APPROACH 4: RECURSIVE
 * Time: O(log n), Space: O(log n) — call stack
 * ============================================================ */
static long long reverse_rec_helper(long long n, long long acc) {
    if (n == 0) return acc;
    return reverse_rec_helper(n / 10, acc * 10 + n % 10);
}

long long reverse_recursive(int n) {
    int sign = (n < 0) ? -1 : 1;
    long long abs_n = (n < 0) ? -(long long)n : (long long)n;
    return (long long)sign * reverse_rec_helper(abs_n, 0);
}

/* ============================================================
 * APPROACH 5: DIGIT COUNT THEN RECONSTRUCT
 * Time: O(log n), Space: O(1)
 * Extract each digit by position and place in mirrored position.
 * ============================================================ */
long long reverse_positional(int n) {
    int sign = (n < 0) ? -1 : 1;
    long long num = (n < 0) ? -(long long)n : (long long)n;

    /* Count digits */
    int k = 0;
    long long temp = num;
    while (temp > 0) { k++; temp /= 10; }
    if (k == 0) return 0;

    long long result = 0;
    long long power = 1;
    for (int i = 0; i < k - 1; i++) power *= 10;  /* 10^(k-1) */

    for (int i = 0; i < k; i++) {
        int digit = (int)(num % 10);
        result += (long long)digit * power;
        num /= 10;
        power /= 10;
    }
    return (long long)sign * result;
}

/* ============================================================
 * DRY RUN
 * ============================================================ */
static void dry_run(int n) {
    printf("\n--- Dry Run: reverse_overflow_safe(%d) ---\n", n);
    int result = 0;
    int original = n;
    int step = 1;
    while (n != 0) {
        int digit = n % 10;
        n /= 10;
        result = result * 10 + digit;
        printf("  Step %d: digit=%d, n_remaining=%d, result=%d\n",
               step++, digit, n, result);
    }
    printf("  reverse(%d) = %d\n", original, result);
}

/* ============================================================
 * TEST SUITE
 * ============================================================ */
static void run_tests(void) {
    printf("\n=== Running Tests ===\n");

    /* Basic cases */
    assert(reverse_basic(123)    == 321);
    assert(reverse_basic(120)    == 21);    /* trailing zero becomes leading */
    assert(reverse_basic(0)      == 0);
    assert(reverse_basic(1)      == 1);
    assert(reverse_basic(-123)   == -321);
    assert(reverse_basic(-120)   == -21);

    /* Overflow-safe */
    assert(reverse_overflow_safe(123)    == 321);
    assert(reverse_overflow_safe(-123)   == -321);
    assert(reverse_overflow_safe(120)    == 21);
    assert(reverse_overflow_safe(0)      == 0);
    assert(reverse_overflow_safe(1534236469) == INT_MIN); /* would overflow */

    /* Cross-validate string vs basic for small values */
    for (int n = -999; n <= 999; n++) {
        long long r1 = reverse_basic(n);
        long long r2 = reverse_string(n);
        long long r3 = reverse_recursive(n);
        if (r1 != r2 || r2 != r3) {
            printf("MISMATCH at n=%d: basic=%lld str=%lld rec=%lld\n",
                   n, r1, r2, r3);
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
    printf(" Reverse Number — All Approaches\n");
    printf("=========================================\n");

    run_tests();

    dry_run(12345);
    dry_run(-456);
    dry_run(1200);

    int test_vals[] = {0, 1, -1, 123, -456, 1200, 2147483641};
    int count = (int)(sizeof(test_vals) / sizeof(test_vals[0]));
    for (int i = 0; i < count; i++) {
        printf("\nn=%d: basic=%lld  safe=%d  str=%lld  rec=%lld\n",
               test_vals[i],
               reverse_basic(test_vals[i]),
               reverse_overflow_safe(test_vals[i]),
               reverse_string(test_vals[i]),
               reverse_recursive(test_vals[i]));
    }

    printf("\n=========================================\n");
    printf(" Complexity Summary\n");
    printf("=========================================\n");
    printf("%-25s %-12s %-10s\n", "Approach", "Time", "Space");
    printf("%-25s %-12s %-10s\n", "Basic Loop",      "O(log n)", "O(1)");
    printf("%-25s %-12s %-10s\n", "Overflow-Safe",   "O(log n)", "O(1)");
    printf("%-25s %-12s %-10s\n", "String-Based",    "O(log n)", "O(log n)");
    printf("%-25s %-12s %-10s\n", "Recursive",       "O(log n)", "O(log n)");
    printf("%-25s %-12s %-10s\n", "Positional",      "O(log n)", "O(1)");

    return 0;
}
