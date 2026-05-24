/*
 * palindrome.c
 * ============
 * Problem: Check if a number is a palindrome.
 * Category: Foundation > A. Basic Number Problems
 *
 * Compile: gcc -Wall -Wextra -std=c11 -O2 -o palindrome palindrome.c
 * Debug:   gcc -fsanitize=address,undefined -g -std=c11 -o palindrome palindrome.c
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

/* ============================================================
 * APPROACH 1: STRING CONVERSION
 * Time: O(log n), Space: O(log n)
 * ============================================================ */
bool is_palindrome_string(int n) {
    if (n < 0) return false;

    char buf[12];  /* max 10 digits + sign + null */
    snprintf(buf, sizeof(buf), "%d", n);

    int len = (int)strlen(buf);
    int left = 0, right = len - 1;
    while (left < right) {
        if (buf[left] != buf[right]) return false;
        left++;
        right--;
    }
    return true;
}

/* ============================================================
 * APPROACH 2: FULL DIGIT REVERSAL
 * Time: O(log n), Space: O(1)
 * Uses long long to prevent overflow.
 * ============================================================ */
bool is_palindrome_full_reverse(int n) {
    if (n < 0) return false;

    long long original = (long long)n;
    long long reversed = 0;
    long long temp = original;

    while (temp > 0) {
        reversed = reversed * 10 + temp % 10;
        temp /= 10;
    }
    return original == reversed;
}

/* ============================================================
 * APPROACH 3: HALF REVERSAL [PREFERRED — INTERVIEW STANDARD]
 * Time: O(log n / 2), Space: O(1)
 *
 * Key insight: only reverse the second half of the digits.
 * Stop when remaining n <= reversed_half.
 * Even digits: n == reversed_half
 * Odd digits:  n == reversed_half / 10  (center digit irrelevant)
 * ============================================================ */
bool is_palindrome_half_reverse(int n) {
    /* Negative or non-zero number ending in 0 → NOT palindrome */
    if (n < 0 || (n != 0 && n % 10 == 0)) return false;

    int reversed_half = 0;
    while (n > reversed_half) {
        reversed_half = reversed_half * 10 + n % 10;
        n /= 10;
    }
    return n == reversed_half || n == reversed_half / 10;
}

/* ============================================================
 * APPROACH 4: DIGIT ARRAY COMPARISON
 * Time: O(log n), Space: O(log n)
 * ============================================================ */
bool is_palindrome_digit_array(int n) {
    if (n < 0) return false;
    if (n == 0) return true;

    int digits[10];   /* int has at most 10 digits */
    int count = 0;

    /* Extract digits in reverse order (LSD first) */
    int temp = n;
    while (temp > 0) {
        digits[count++] = temp % 10;
        temp /= 10;
    }

    /* digits[0]=LSD, digits[count-1]=MSD */
    /* Palindrome: digits[i] == digits[count-1-i] */
    for (int i = 0; i < count / 2; i++) {
        if (digits[i] != digits[count - 1 - i]) return false;
    }
    return true;
}

/* ============================================================
 * APPROACH 5: TWO POINTER ON DIGITS (No array)
 * Time: O(log n), Space: O(1)
 *
 * Extract MSD via divisor, LSD via modulo.
 * Shrink from both ends each iteration.
 * ============================================================ */
bool is_palindrome_two_pointer(int n) {
    if (n < 0) return false;
    if (n == 0) return true;

    /* Compute divisor = 10^(k-1) where k = number of digits */
    int divisor = 1;
    while (n / divisor >= 10) divisor *= 10;

    while (divisor > 1) {
        int left_digit  = n / divisor;       /* MSD */
        int right_digit = n % 10;            /* LSD */
        if (left_digit != right_digit) return false;

        /* Strip leftmost and rightmost digit */
        n = (n % divisor) / 10;
        divisor /= 100;                      /* removed 2 digits → divisor shrinks by 10^2 */
    }
    return true;
}

/* ============================================================
 * APPROACH 6: BINARY PALINDROME (Bit-level variant)
 * Time: O(log n), Space: O(1)
 * Check if the binary representation is a palindrome.
 * ============================================================ */
bool is_binary_palindrome(uint32_t n) {
    if (n == 0) return true;

    /* Count significant bits */
    int bit_len = 0;
    uint32_t temp = n;
    while (temp > 0) { bit_len++; temp >>= 1; }

    /* Reverse only significant bits */
    uint32_t original = n;
    uint32_t reversed = 0;
    for (int i = 0; i < bit_len; i++) {
        reversed = (reversed << 1) | (n & 1u);
        n >>= 1;
    }
    return original == reversed;
}

/* ============================================================
 * DRY RUN DEMONSTRATION
 * ============================================================ */
static void dry_run_half_reverse(int n) {
    printf("\n--- Dry Run: is_palindrome_half_reverse(%d) ---\n", n);

    if (n < 0) { printf("  n < 0 → NOT palindrome\n"); return; }
    if (n != 0 && n % 10 == 0) { printf("  trailing zero → NOT palindrome\n"); return; }

    int original = n;
    int rev = 0;
    int step = 1;
    while (n > rev) {
        int digit = n % 10;
        rev = rev * 10 + digit;
        n /= 10;
        printf("  Step %d: digit=%d, remaining_n=%d, reversed_half=%d\n",
               step++, digit, n, rev);
    }
    bool result = (n == rev || n == rev / 10);
    printf("  n=%d, rev=%d → %s\n", n, rev,
           result ? "PALINDROME" : "NOT PALINDROME");
    printf("  Original: %d\n", original);
}

/* ============================================================
 * COMPARISON TABLE
 * ============================================================ */
static void compare_all(int n) {
    printf("\n=== n = %d ===\n", n);
    printf("  String:       %s\n", is_palindrome_string(n)       ? "YES" : "NO");
    printf("  Full Reverse: %s\n", is_palindrome_full_reverse(n)  ? "YES" : "NO");
    printf("  Half Reverse: %s\n", is_palindrome_half_reverse(n)  ? "YES" : "NO");
    printf("  Digit Array:  %s\n", is_palindrome_digit_array(n)   ? "YES" : "NO");
    printf("  Two Pointer:  %s\n", is_palindrome_two_pointer(n)   ? "YES" : "NO");
}

/* ============================================================
 * TEST SUITE
 * ============================================================ */
static void run_tests(void) {
    printf("\n=== Running Tests ===\n");

    /* All single digits are palindromes */
    for (int i = 0; i <= 9; i++) {
        assert(is_palindrome_half_reverse(i) == true);
        assert(is_palindrome_string(i)       == true);
    }

    /* Two-digit palindromes */
    assert(is_palindrome_half_reverse(11)  == true);
    assert(is_palindrome_half_reverse(22)  == true);
    assert(is_palindrome_half_reverse(99)  == true);
    assert(is_palindrome_half_reverse(12)  == false);
    assert(is_palindrome_half_reverse(10)  == false);

    /* Multi-digit */
    assert(is_palindrome_half_reverse(121)     == true);
    assert(is_palindrome_half_reverse(1221)    == true);
    assert(is_palindrome_half_reverse(12321)   == true);
    assert(is_palindrome_half_reverse(123)     == false);
    assert(is_palindrome_half_reverse(100)     == false);
    assert(is_palindrome_half_reverse(1000021) == false);

    /* Edge cases */
    assert(is_palindrome_half_reverse(-121)      == false);
    assert(is_palindrome_half_reverse(-1)        == false);
    assert(is_palindrome_half_reverse(0)         == true);
    assert(is_palindrome_half_reverse(2147483647) == false);  /* INT_MAX */

    /* Cross-validate all approaches [0, 10000] */
    for (int n = -100; n <= 10000; n++) {
        bool a1 = is_palindrome_string(n);
        bool a2 = is_palindrome_full_reverse(n);
        bool a3 = is_palindrome_half_reverse(n);
        bool a4 = is_palindrome_digit_array(n);
        bool a5 = is_palindrome_two_pointer(n);
        if (a1 != a2 || a2 != a3 || a3 != a4 || a4 != a5) {
            printf("MISMATCH at n=%d: s=%d fr=%d hr=%d da=%d tp=%d\n",
                   n, a1, a2, a3, a4, a5);
            assert(false);
        }
    }

    /* Binary palindrome tests */
    assert(is_binary_palindrome(0)   == true);   /* 0 */
    assert(is_binary_palindrome(1)   == true);   /* 1 */
    assert(is_binary_palindrome(3)   == true);   /* 11 */
    assert(is_binary_palindrome(5)   == true);   /* 101 */
    assert(is_binary_palindrome(9)   == true);   /* 1001 */
    assert(is_binary_palindrome(6)   == false);  /* 110 */
    assert(is_binary_palindrome(4)   == false);  /* 100 */

    printf("All tests PASSED.\n");
}

/* ============================================================
 * MAIN
 * ============================================================ */
int main(void) {
    printf("=========================================\n");
    printf(" Palindrome Number — All Approaches\n");
    printf("=========================================\n");

    run_tests();

    dry_run_half_reverse(1221);
    dry_run_half_reverse(12321);
    dry_run_half_reverse(123);
    dry_run_half_reverse(10);

    int test_vals[] = {0, 1, 11, 121, 1221, 12321, 123, -121, 10, 1000001, 2147483647};
    int count = (int)(sizeof(test_vals) / sizeof(test_vals[0]));
    for (int i = 0; i < count; i++) {
        compare_all(test_vals[i]);
    }

    printf("\n=========================================\n");
    printf(" Complexity Summary\n");
    printf("=========================================\n");
    printf("%-25s %-15s %-10s\n", "Approach",         "Time",       "Space");
    printf("%-25s %-15s %-10s\n", "String",           "O(log n)",   "O(log n)");
    printf("%-25s %-15s %-10s\n", "Full Reversal",    "O(log n)",   "O(1)");
    printf("%-25s %-15s %-10s\n", "Half Reversal",    "O(log n/2)", "O(1)");
    printf("%-25s %-15s %-10s\n", "Digit Array",      "O(log n)",   "O(log n)");
    printf("%-25s %-15s %-10s\n", "Two Pointer",      "O(log n)",   "O(1)");
    printf("%-25s %-15s %-10s\n", "Binary Palindrome","O(log n)",   "O(1)");

    return 0;
}
