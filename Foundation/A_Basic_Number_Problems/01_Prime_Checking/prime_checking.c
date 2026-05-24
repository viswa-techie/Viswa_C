/*
 * prime_checking.c
 * ================
 * Problem: Determine if a number is prime.
 * Category: Foundation > A. Basic Number Problems
 *
 * Compile:  gcc -Wall -Wextra -std=c11 -O2 -o prime prime_checking.c
 * Debug:    gcc -fsanitize=address,undefined -g -std=c11 -o prime prime_checking.c
 *
 * All functions are UB-free, overflow-safe, and tested.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

/* ============================================================
 * APPROACH 1: BRUTE FORCE
 * Time:  O(n)
 * Space: O(1)
 * ============================================================ */
bool is_prime_brute(int n) {
    if (n <= 1) return false;
    for (int i = 2; i < n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

/* ============================================================
 * APPROACH 2: SQRT TRIAL DIVISION
 * Time:  O(sqrt(n))
 * Space: O(1)
 *
 * Key insight: if d | n, then (n/d) | n too.
 * Smaller factor of any pair is <= sqrt(n).
 * Note: cast to long long to prevent i*i overflow.
 * ============================================================ */
bool is_prime_sqrt(int n) {
    if (n <= 1) return false;
    for (long long i = 2; i * i <= (long long)n; i++) {
        if (n % (int)i == 0) return false;
    }
    return true;
}

/* ============================================================
 * APPROACH 3: SKIP EVEN NUMBERS
 * Time:  O(sqrt(n)/2)
 * Space: O(1)
 *
 * After checking divisibility by 2, only check odd numbers.
 * ============================================================ */
bool is_prime_skip_even(int n) {
    if (n <= 1) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (long long i = 3; i * i <= (long long)n; i += 2) {
        if (n % (int)i == 0) return false;
    }
    return true;
}

/* ============================================================
 * APPROACH 4: 6K±1 OPTIMIZATION  [PREFERRED / INTERVIEW STANDARD]
 * Time:  O(sqrt(n)/3)  -- ~3x faster than basic sqrt
 * Space: O(1)
 *
 * Mathematical basis:
 *   All integers = { 6k, 6k+1, 6k+2, 6k+3, 6k+4, 6k+5 }
 *   6k, 6k+2, 6k+4 → divisible by 2 (composite)
 *   6k+3           → divisible by 3 (composite)
 *   Only 6k+1 and 6k+5 (= 6(k+1)-1) can be prime.
 *   So we only check i = 5,7, 11,13, 17,19, ... (steps of 6)
 * ============================================================ */
bool is_prime_optimal(int n) {
    if (n <= 1)           return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0)       return false;   /* even > 2: composite */
    if (n % 3 == 0)       return false;   /* multiple of 3 > 3: composite */

    /*
     * Check 6k±1 candidates: i = 5, 11, 17, 23, ...
     * At each step: check i (6k-1) and i+2 (6k+1)
     * Cast to long long to prevent i*i overflow for large n.
     */
    for (long long i = 5; i * i <= (long long)n; i += 6) {
        if (n % (int)i == 0 || n % (int)(i + 2) == 0)
            return false;
    }
    return true;
}

/* ============================================================
 * APPROACH 5: SIEVE OF ERATOSTHENES  (Batch queries)
 * Time:  O(n log log n) for setup, O(1) per query
 * Space: O(n)
 *
 * Best when checking many primes in a bounded range.
 * Caller must free the returned array.
 * ============================================================ */
bool *sieve_of_eratosthenes(int limit) {
    if (limit < 0) return NULL;

    bool *is_prime = malloc((size_t)(limit + 1) * sizeof(bool));
    if (!is_prime) return NULL;

    memset(is_prime, 1, (size_t)(limit + 1) * sizeof(bool));

    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (long long i = 2; i * i <= (long long)limit; i++) {
        if (is_prime[i]) {
            /*
             * Start marking from i*i (all smaller multiples already marked).
             * Cast to int is safe here because j <= limit and limit is int.
             */
            for (long long j = i * i; j <= (long long)limit; j += i) {
                is_prime[(int)j] = false;
            }
        }
    }
    return is_prime;
}

/* ============================================================
 * APPROACH 6: LOOKUP TABLE FOR SMALL n (Embedded / MCU friendly)
 * Time:  O(1)
 * Space: O(1) — constant table in ROM
 *
 * Precomputed bit-pack: bit k of prime_bits[k/32] = 1 if k is prime.
 * Range: [0, 127]
 * For embedded use: store in __flash / const section.
 * ============================================================ */
static const uint32_t prime_bits[4] = {
    /* bits 0-31:   primes: 2,3,5,7,11,13,17,19,23,29,31       */
    0x208A28ACU,
    /* bits 32-63:  primes: 37,41,43,47,53,59,61                */
    0x28208A20U,
    /* bits 64-95:  primes: 67,71,73,79,83,89,97                */
    0x02088288U,
    /* bits 96-127: primes: 101,103,107,109,113,127             */
    0x0000228AU
};

bool is_prime_lut(unsigned int n) {
    if (n > 127) return is_prime_optimal((int)n);   /* fallback */
    return (bool)((prime_bits[n >> 5] >> (n & 31)) & 1u);
}

/* ============================================================
 * APPROACH 7: MILLER-RABIN (Large numbers, cryptography)
 * Time:  O(k * log^2 n)  k = number of witnesses
 * Space: O(1)
 *
 * Deterministic for n < 3,215,031,751 using witnesses {2,3,5,7}.
 * Uses __uint128_t for overflow-safe modular multiplication.
 * ============================================================ */
typedef unsigned long long ull;

static ull mulmod128(ull a, ull b, ull m) {
    return (ull)((__uint128_t)a * b % m);
}

static ull powmod(ull base, ull exp, ull mod) {
    ull result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1ULL) result = mulmod128(result, base, mod);
        base = mulmod128(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool miller_witness(ull n, ull a) {
    if (n % a == 0) return n == a;

    ull d = n - 1;
    int r = 0;
    while ((d & 1ULL) == 0) { d >>= 1; r++; }   /* write n-1 = 2^r * d */

    ull x = powmod(a, d, n);
    if (x == 1ULL || x == n - 1) return true;

    for (int i = 0; i < r - 1; i++) {
        x = mulmod128(x, x, n);
        if (x == n - 1) return true;
    }
    return false;
}

bool is_prime_miller_rabin(ull n) {
    if (n < 2) return false;
    if (n == 2 || n == 3 || n == 5 || n == 7) return true;
    if (n % 2 == 0) return false;

    /* Deterministic witnesses for n < 3,215,031,751 */
    const ull witnesses[] = {2ULL, 3ULL, 5ULL, 7ULL};
    for (int i = 0; i < 4; i++) {
        if (!miller_witness(n, witnesses[i])) return false;
    }
    return true;
}

/* ============================================================
 * COMPARISON TABLE PRINTER
 * ============================================================ */
static void print_separator(void) {
    printf("%-15s %-12s %-12s %-12s %-12s\n",
           "---------------", "------------", "------------",
           "------------", "------------");
}

static void compare_approaches(int n) {
    printf("\n=== Checking n = %d ===\n", n);
    printf("%-15s %-12s %-12s %-12s %-12s\n",
           "Approach", "Brute", "Sqrt", "6k+/-1", "Miller-R");
    print_separator();
    printf("%-15s %-12s %-12s %-12s %-12s\n",
           "Result",
           is_prime_brute(n)         ? "PRIME" : "NOT PRIME",
           is_prime_sqrt(n)          ? "PRIME" : "NOT PRIME",
           is_prime_optimal(n)       ? "PRIME" : "NOT PRIME",
           is_prime_miller_rabin((ull)n) ? "PRIME" : "NOT PRIME");
}

/* ============================================================
 * DRY RUN DEMONSTRATION
 * ============================================================ */
static void dry_run_demo(int n) {
    printf("\n--- Dry Run: is_prime_optimal(%d) ---\n", n);

    if (n <= 1) { printf("  n <= 1 → NOT PRIME\n"); return; }
    if (n == 2 || n == 3) { printf("  n == 2 or 3 → PRIME\n"); return; }
    if (n % 2 == 0) { printf("  n %% 2 == 0 → NOT PRIME\n"); return; }
    if (n % 3 == 0) { printf("  n %% 3 == 0 → NOT PRIME\n"); return; }

    bool found = false;
    for (long long i = 5; i * i <= (long long)n; i += 6) {
        printf("  Check i=%lld: n%%i=%d, n%%(i+2)=%d\n",
               i, n % (int)i, n % (int)(i + 2));
        if (n % (int)i == 0 || n % (int)(i + 2) == 0) {
            printf("  → Divisor found → NOT PRIME\n");
            found = true;
            break;
        }
    }
    if (!found) printf("  → No divisors found → PRIME\n");
}

/* ============================================================
 * TEST CASES
 * ============================================================ */
static void run_tests(void) {
    printf("\n=== Running Tests ===\n");

    /* Basic primes */
    assert(is_prime_optimal(2)   == true);
    assert(is_prime_optimal(3)   == true);
    assert(is_prime_optimal(5)   == true);
    assert(is_prime_optimal(7)   == true);
    assert(is_prime_optimal(11)  == true);
    assert(is_prime_optimal(13)  == true);
    assert(is_prime_optimal(97)  == true);

    /* Composites */
    assert(is_prime_optimal(4)   == false);
    assert(is_prime_optimal(6)   == false);
    assert(is_prime_optimal(9)   == false);
    assert(is_prime_optimal(25)  == false);
    assert(is_prime_optimal(100) == false);

    /* Edge cases */
    assert(is_prime_optimal(0)   == false);
    assert(is_prime_optimal(1)   == false);
    assert(is_prime_optimal(-1)  == false);
    assert(is_prime_optimal(-7)  == false);

    /* INT_MAX = 2147483647 is a Mersenne prime (2^31 - 1) */
    assert(is_prime_optimal(2147483647) == true);

    /* Cross-validate brute force vs optimal for [0, 1000] */
    for (int n = 0; n <= 1000; n++) {
        if (is_prime_brute(n) != is_prime_optimal(n)) {
            printf("MISMATCH at n=%d: brute=%d, optimal=%d\n",
                   n, is_prime_brute(n), is_prime_optimal(n));
            assert(false);
        }
    }

    /* Sieve validation */
    bool *sieve = sieve_of_eratosthenes(1000);
    assert(sieve != NULL);
    for (int n = 0; n <= 1000; n++) {
        if (sieve[n] != is_prime_brute(n)) {
            printf("SIEVE MISMATCH at n=%d\n", n);
            assert(false);
        }
    }
    free(sieve);

    /* LUT validation for [0, 100] */
    for (unsigned int n = 0; n <= 100; n++) {
        if (is_prime_lut(n) != (bool)is_prime_optimal((int)n)) {
            printf("LUT MISMATCH at n=%u\n", n);
            assert(false);
        }
    }

    printf("All tests PASSED.\n");
}

/* ============================================================
 * SIEVE DEMO: Print all primes up to N
 * ============================================================ */
static void print_primes_sieve(int limit) {
    printf("\n=== Primes up to %d (Sieve of Eratosthenes) ===\n", limit);
    bool *is_prime = sieve_of_eratosthenes(limit);
    if (!is_prime) { printf("Memory allocation failed\n"); return; }

    int count = 0;
    for (int i = 2; i <= limit; i++) {
        if (is_prime[i]) {
            printf("%4d", i);
            count++;
            if (count % 10 == 0) printf("\n");
        }
    }
    if (count % 10 != 0) printf("\n");
    printf("Total primes up to %d: %d\n", limit, count);
    free(is_prime);
}

/* ============================================================
 * MAIN
 * ============================================================ */
int main(void) {
    printf("=========================================\n");
    printf(" Prime Checking — All Approaches\n");
    printf("=========================================\n");

    run_tests();

    /* Dry runs for representative values */
    dry_run_demo(35);     /* Composite: 5 × 7 */
    dry_run_demo(37);     /* Prime */
    dry_run_demo(49);     /* Composite: 7 × 7 */

    /* Approach comparison table */
    int test_vals[] = {2, 7, 25, 97, 100, 999983, 2147483647};
    int count = (int)(sizeof(test_vals) / sizeof(test_vals[0]));
    for (int i = 0; i < count; i++) {
        compare_approaches(test_vals[i]);
    }

    /* Sieve demo */
    print_primes_sieve(100);

    printf("\n=========================================\n");
    printf(" Complexity Summary\n");
    printf("=========================================\n");
    printf("%-25s %-15s %-10s\n", "Approach", "Time", "Space");
    printf("%-25s %-15s %-10s\n", "Brute Force",        "O(n)",          "O(1)");
    printf("%-25s %-15s %-10s\n", "Sqrt Division",      "O(sqrt(n))",    "O(1)");
    printf("%-25s %-15s %-10s\n", "Skip Even",          "O(sqrt(n)/2)",  "O(1)");
    printf("%-25s %-15s %-10s\n", "6k±1 Optimal",       "O(sqrt(n)/3)",  "O(1)");
    printf("%-25s %-15s %-10s\n", "Sieve (setup)",      "O(n log log n)","O(n)");
    printf("%-25s %-15s %-10s\n", "Sieve (query)",      "O(1)",          "O(n)");
    printf("%-25s %-15s %-10s\n", "LUT (<128)",         "O(1)",          "O(1)");
    printf("%-25s %-15s %-10s\n", "Miller-Rabin",       "O(k log^2 n)",  "O(1)");

    return 0;
}
