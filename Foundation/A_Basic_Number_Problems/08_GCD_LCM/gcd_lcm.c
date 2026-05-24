/*
 * gcd_lcm.c
 * =========
 * Problem: Compute GCD (Greatest Common Divisor) and LCM (Least Common Multiple).
 * Category: Foundation > A. Basic Number Problems
 *
 * Compile: gcc -Wall -Wextra -std=c11 -O2 -o gcd_lcm gcd_lcm.c
 * Debug:   gcc -fsanitize=address,undefined -g -std=c11 -o gcd_lcm gcd_lcm.c
 *
 * Key identity: LCM(a, b) = (a / GCD(a, b)) * b
 *               (divide first to avoid overflow)
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

typedef unsigned long long ull;

/* ============================================================
 * APPROACH 1: GCD — BRUTE FORCE (Check all up to min(a,b))
 * Time: O(min(a,b)), Space: O(1)
 * ============================================================ */
int gcd_brute(int a, int b) {
    a = abs(a); b = abs(b);
    if (a == 0) return b;
    if (b == 0) return a;
    int g = 1;
    for (int i = 1; i <= (a < b ? a : b); i++) {
        if (a % i == 0 && b % i == 0) g = i;
    }
    return g;
}

/* ============================================================
 * APPROACH 2: GCD — EUCLIDEAN ALGORITHM (Modulo Version)
 * Based on: gcd(a, b) = gcd(b, a % b)
 * Terminates when b == 0.
 * Time: O(log(min(a,b))), Space: O(1)  [PREFERRED]
 * ============================================================ */
int gcd_euclidean(int a, int b) {
    a = abs(a); b = abs(b);
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;   /* a holds GCD when b becomes 0 */
}

/* ============================================================
 * APPROACH 3: GCD — RECURSIVE EUCLIDEAN
 * Time: O(log(min(a,b))), Space: O(log(min(a,b))) — stack
 * ============================================================ */
int gcd_recursive(int a, int b) {
    a = abs(a); b = abs(b);
    if (b == 0) return a;
    return gcd_recursive(b, a % b);
}

/* ============================================================
 * APPROACH 4: GCD — BINARY GCD (Stein's Algorithm)
 * Uses only subtraction and bit shifts — no modulo/division.
 * Faster than Euclidean on hardware without hardware division.
 * Ideal for embedded systems without FPU/divider unit.
 * Time: O(log^2(min(a,b))), Space: O(1)
 * ============================================================ */
ull gcd_binary(ull a, ull b) {
    if (a == 0) return b;
    if (b == 0) return a;

    /* Count and remove common factors of 2 */
    int shift = __builtin_ctzll(a | b);   /* ctz: count trailing zeros */
    a >>= __builtin_ctzll(a);             /* remove all factors of 2 from a */

    do {
        b >>= __builtin_ctzll(b);         /* remove all factors of 2 from b */
        if (a > b) { ull t = a; a = b; b = t; }  /* ensure a <= b */
        b -= a;                           /* b = b - a (b is now even) */
    } while (b != 0);

    return a << shift;   /* restore common factor of 2 */
}

/* ============================================================
 * APPROACH 5: GCD — SUBTRACTION-BASED (Oldest form, O(max(a,b)))
 * Historically first, but slow for large inputs.
 * ============================================================ */
int gcd_subtraction(int a, int b) {
    a = abs(a); b = abs(b);
    while (a != b) {
        if (a > b) a -= b;
        else       b -= a;
    }
    return a;
}

/* ============================================================
 * APPROACH 6: GCD of MULTIPLE NUMBERS
 * gcd(a1, a2, ..., an) = gcd(gcd(a1, a2), a3, ...)
 * ============================================================ */
int gcd_array(const int *arr, int n) {
    if (!arr || n <= 0) return 0;
    int result = arr[0];
    for (int i = 1; i < n; i++) {
        result = gcd_euclidean(result, arr[i]);
        if (result == 1) return 1;  /* early exit: GCD can't be less than 1 */
    }
    return result;
}

/* ============================================================
 * LCM COMPUTATION
 * LCM(a, b) = (a / gcd(a,b)) * b  ← divide BEFORE multiply to prevent overflow
 * Time: O(log(min(a,b))), Space: O(1)
 * ============================================================ */
long long lcm(int a, int b) {
    if (a == 0 || b == 0) return 0;
    int g = gcd_euclidean(a, b);
    /* Divide first to avoid overflow: a/g * b instead of a*b/g */
    return ((long long)(a / g)) * (long long)b;
}

long long lcm_unsigned(ull a, ull b) {
    if (a == 0 || b == 0) return 0;
    ull g = gcd_binary(a, b);
    return (long long)((a / g) * b);
}

/* LCM of multiple numbers */
long long lcm_array(const int *arr, int n) {
    if (!arr || n <= 0) return 0;
    long long result = arr[0];
    for (int i = 1; i < n; i++) {
        result = lcm((int)result, arr[i]);
    }
    return result;
}

/* ============================================================
 * EXTENDED EUCLIDEAN ALGORITHM
 * Finds x, y such that: a*x + b*y = gcd(a, b)  (Bezout's identity)
 * Essential for modular inverse computation.
 * Time: O(log(min(a,b))), Space: O(1)
 * ============================================================ */
int gcd_extended(int a, int b, int *x, int *y) {
    if (b == 0) {
        *x = 1; *y = 0;
        return a;
    }
    int x1, y1;
    int g = gcd_extended(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return g;
}

/* ============================================================
 * MODULAR INVERSE (using Extended Euclidean)
 * Returns x such that: a * x ≡ 1 (mod m)
 * Exists only if gcd(a, m) == 1.
 * ============================================================ */
int modular_inverse(int a, int m) {
    int x, y;
    int g = gcd_extended(a, m, &x, &y);
    if (g != 1) return -1;  /* inverse doesn't exist */
    return ((x % m) + m) % m;  /* ensure positive result */
}

/* ============================================================
 * DRY RUN — Euclidean GCD
 * ============================================================ */
static void dry_run_gcd(int a, int b) {
    printf("\n--- Dry Run: gcd_euclidean(%d, %d) ---\n", a, b);
    int step = 1;
    while (b != 0) {
        printf("  Step %d: gcd(%d, %d) → gcd(%d, %d%%=%d=%d)\n",
               step++, a, b, b, a, b, a%b);
        int temp = b;
        b = a % b;
        a = temp;
    }
    printf("  GCD = %d\n", a);
}

/* ============================================================
 * TEST SUITE
 * ============================================================ */
static void run_tests(void) {
    printf("\n=== Running Tests ===\n");

    /* GCD basic */
    assert(gcd_euclidean(12, 8)   == 4);
    assert(gcd_euclidean(100, 75) == 25);
    assert(gcd_euclidean(7, 5)    == 1);  /* coprime */
    assert(gcd_euclidean(0, 5)    == 5);
    assert(gcd_euclidean(5, 0)    == 5);
    assert(gcd_euclidean(0, 0)    == 0);
    assert(gcd_euclidean(12, 12)  == 12);
    assert(gcd_euclidean(-12, 8)  == 4);  /* negative input */

    /* Cross-validate all GCD approaches */
    int test_pairs[][2] = {{12,8},{100,75},{7,5},{0,5},{48,18},{252,105}};
    int tp_count = (int)(sizeof(test_pairs)/sizeof(test_pairs[0]));
    for (int i = 0; i < tp_count; i++) {
        int a = test_pairs[i][0], b = test_pairs[i][1];
        int r1 = gcd_brute(a, b);
        int r2 = gcd_euclidean(a, b);
        int r3 = gcd_recursive(a, b);
        int r4 = (int)gcd_binary((ull)abs(a), (ull)abs(b));
        int r5 = (a > 0 && b > 0) ? gcd_subtraction(a, b) : r2;
        if (r1 != r2 || r2 != r3 || r3 != r4) {
            printf("MISMATCH gcd(%d,%d): brute=%d eucl=%d rec=%d bin=%d sub=%d\n",
                   a, b, r1, r2, r3, r4, r5);
            assert(false);
        }
    }

    /* LCM */
    assert(lcm(4, 6)   == 12);
    assert(lcm(3, 5)   == 15);
    assert(lcm(0, 5)   == 0);
    assert(lcm(12, 18) == 36);

    /* LCM × GCD identity: lcm(a,b) * gcd(a,b) = a * b */
    for (int a = 1; a <= 50; a++) {
        for (int b = 1; b <= 50; b++) {
            long long l = lcm(a, b);
            int g = gcd_euclidean(a, b);
            assert(l * g == (long long)a * b);
        }
    }

    /* Extended Euclidean */
    int x, y;
    int g = gcd_extended(35, 15, &x, &y);
    assert(g == 5);
    assert(35*x + 15*y == g);   /* Bezout's identity */

    /* Modular inverse: 3^(-1) mod 7 = 5 (since 3*5=15≡1 mod 7) */
    assert(modular_inverse(3, 7) == 5);
    assert(modular_inverse(2, 6) == -1);  /* gcd(2,6)=2 ≠ 1, no inverse */

    printf("All tests PASSED.\n");
}

/* ============================================================
 * MAIN
 * ============================================================ */
int main(void) {
    printf("=========================================\n");
    printf(" GCD / LCM — All Approaches\n");
    printf("=========================================\n");

    run_tests();

    dry_run_gcd(48, 18);
    dry_run_gcd(252, 105);

    printf("\n=== GCD/LCM Table ===\n");
    printf("%-5s %-5s %-8s %-8s  %-20s\n", "a", "b", "GCD", "LCM", "GCD*LCM==a*b");
    int pairs[][2] = {{4,6},{12,8},{7,5},{100,75},{48,18},{252,105},{36,60}};
    int pc = (int)(sizeof(pairs)/sizeof(pairs[0]));
    for (int i = 0; i < pc; i++) {
        int a = pairs[i][0], b = pairs[i][1];
        int g = gcd_euclidean(a, b);
        long long l = lcm(a, b);
        printf("%-5d %-5d %-8d %-8lld  %s\n",
               a, b, g, l,
               (l * g == (long long)a * b) ? "YES" : "NO");
    }

    printf("\n=== Extended GCD (Bezout's Identity) ===\n");
    int ex_pairs[][2] = {{35,15},{48,36},{7,5}};
    int ep_count = (int)(sizeof(ex_pairs)/sizeof(ex_pairs[0]));
    for (int i = 0; i < ep_count; i++) {
        int a = ex_pairs[i][0], b = ex_pairs[i][1];
        int xv, yv;
        int gv = gcd_extended(a, b, &xv, &yv);
        printf("gcd(%d,%d)=%d: %d*(%d) + %d*(%d) = %d\n",
               a, b, gv, a, xv, b, yv, gv);
    }

    printf("\n=========================================\n");
    printf(" Complexity Summary\n");
    printf("=========================================\n");
    printf("%-25s %-20s %-10s\n", "Approach", "Time", "Space");
    printf("%-25s %-20s %-10s\n", "Brute Force GCD",   "O(min(a,b))",     "O(1)");
    printf("%-25s %-20s %-10s\n", "Euclidean (iter)",  "O(log(min(a,b)))", "O(1)");
    printf("%-25s %-20s %-10s\n", "Euclidean (rec)",   "O(log(min(a,b)))", "O(log)");
    printf("%-25s %-20s %-10s\n", "Binary/Stein",      "O(log^2 n)",       "O(1)");
    printf("%-25s %-20s %-10s\n", "Subtraction",       "O(max(a,b))",      "O(1)");
    printf("%-25s %-20s %-10s\n", "LCM",               "O(log(min(a,b)))", "O(1)");
    printf("%-25s %-20s %-10s\n", "Extended GCD",      "O(log(min(a,b)))", "O(log)");

    return 0;
}
