/*
 * divisors.c
 * ==========
 * Problem: Find all divisors of n / Count divisors of n.
 * Category: Foundation > A. Basic Number Problems
 *
 * Compile: gcc -Wall -Wextra -std=c11 -O2 -o divisors divisors.c
 * Debug:   gcc -fsanitize=address,undefined -g -std=c11 -o divisors divisors.c
 *
 * Key insight: divisors come in pairs (d, n/d).
 * Only one divisor (sqrt(n)) is unpaired when n is a perfect square.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* ============================================================
 * APPROACH 1: BRUTE FORCE — Check all i from 1 to n
 * Time: O(n), Space: O(1) for count, O(n) for list
 * ============================================================ */
int count_divisors_brute(int n) {
    if (n <= 0) return 0;
    int count = 0;
    for (int i = 1; i <= n; i++) {
        if (n % i == 0) count++;
    }
    return count;
}

void print_divisors_brute(int n) {
    printf("Divisors of %d (brute): ", n);
    for (int i = 1; i <= n; i++) {
        if (n % i == 0) printf("%d ", i);
    }
    printf("\n");
}

/* ============================================================
 * APPROACH 2: SQRT OPTIMIZATION
 * Divisors come in pairs (d, n/d).
 * Only check up to sqrt(n). Add both d and n/d.
 * Handle perfect square: sqrt(n) added once.
 * Time: O(sqrt(n)), Space: O(1) count / O(sqrt(n)) list
 * ============================================================ */
int count_divisors_sqrt(int n) {
    if (n <= 0) return 0;
    int count = 0;
    for (int i = 1; (long long)i * i <= (long long)n; i++) {
        if (n % i == 0) {
            count++;                           /* divisor i */
            if (i != n / i) count++;           /* divisor n/i (different) */
        }
    }
    return count;
}

/* Collect and sort all divisors */
int get_divisors(int n, int *divs, int max_divs) {
    if (n <= 0 || !divs) return 0;
    int small_count = 0;
    int large_count = 0;
    int small_divs[1000];  /* divisors <= sqrt(n) */
    int large_divs[1000];  /* corresponding n/d divisors */

    for (long long i = 1; i * i <= (long long)n; i++) {
        if (n % (int)i == 0) {
            if (small_count < 1000) small_divs[small_count++] = (int)i;
            if ((int)i != n / (int)i && large_count < 1000)
                large_divs[large_count++] = n / (int)i;
        }
    }

    /* Combine: small_divs (ascending) + large_divs (needs reversal) */
    int total = 0;
    for (int i = 0; i < small_count && total < max_divs; i++)
        divs[total++] = small_divs[i];
    for (int i = large_count - 1; i >= 0 && total < max_divs; i--)
        divs[total++] = large_divs[i];
    return total;
}

/* ============================================================
 * APPROACH 3: PRIME FACTORIZATION METHOD
 * n = p1^a1 * p2^a2 * ... * pk^ak
 * count_divisors = (a1+1)(a2+1)...(ak+1)
 * Time: O(sqrt(n)), Space: O(log n)
 * ============================================================ */
int count_divisors_prime_factorization(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;

    int count = 1;
    /* Check factor 2 */
    int exp = 0;
    while (n % 2 == 0) { exp++; n /= 2; }
    if (exp > 0) count *= (exp + 1);

    /* Check odd factors */
    for (int p = 3; (long long)p * p <= (long long)n; p += 2) {
        exp = 0;
        while (n % p == 0) { exp++; n /= p; }
        if (exp > 0) count *= (exp + 1);
    }
    if (n > 1) count *= 2;  /* remaining prime factor with exponent 1 → (1+1)=2 */
    return count;
}

/* ============================================================
 * APPROACH 4: SIEVE-BASED (Count divisors for ALL n in [1..N])
 * Time: O(N log N), Space: O(N)
 * Much faster than calling count_divisors_sqrt N times.
 * ============================================================ */
void count_divisors_sieve(int *div_count, int limit) {
    if (!div_count || limit <= 0) return;
    memset(div_count, 0, (size_t)(limit + 1) * sizeof(int));
    for (int i = 1; i <= limit; i++) {
        for (int j = i; j <= limit; j += i) {
            div_count[j]++;  /* i is a divisor of j */
        }
    }
}

/* ============================================================
 * APPROACH 5: SUM OF DIVISORS (sigma function)
 * sigma(n) = sum of all positive divisors of n
 * Perfect number: sigma(n) = 2n  (divisors including n itself)
 * Abundant: sigma(n) > 2n, Deficient: sigma(n) < 2n
 * Time: O(sqrt(n)), Space: O(1)
 * ============================================================ */
long long sum_of_divisors(int n) {
    if (n <= 0) return 0;
    long long sum = 0;
    for (long long i = 1; i * i <= (long long)n; i++) {
        if (n % (int)i == 0) {
            sum += i;
            if ((int)i != n / (int)i) sum += n / (int)i;
        }
    }
    return sum;
}

bool is_perfect_number(int n) {
    if (n <= 1) return false;
    /* sigma(n) - n = n → sum of proper divisors = n */
    return sum_of_divisors(n) - n == (long long)n;
}

/* ============================================================
 * APPROACH 6: SMALLEST PRIME FACTOR SIEVE (for range queries)
 * Precompute smallest prime factor for [2..N].
 * Then factorize any n in O(log n) using SPF table.
 * Time: O(N log log N) sieve + O(log n) factorization
 * Space: O(N)
 * ============================================================ */
#define SPF_LIMIT 10001

static int spf[SPF_LIMIT];  /* smallest prime factor */
static bool spf_initialized = false;

static void init_spf(void) {
    if (spf_initialized) return;
    for (int i = 0; i < SPF_LIMIT; i++) spf[i] = i;
    for (int i = 2; (long long)i * i < SPF_LIMIT; i++) {
        if (spf[i] == i) {   /* i is prime */
            for (int j = i * i; j < SPF_LIMIT; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }
    spf_initialized = true;
}

int count_divisors_spf(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    if (n >= SPF_LIMIT) return count_divisors_sqrt(n);
    init_spf();
    int count = 1;
    while (n > 1) {
        int p = spf[n];
        int exp = 0;
        while (n % p == 0) { exp++; n /= p; }
        count *= (exp + 1);
    }
    return count;
}

/* ============================================================
 * DRY RUN
 * ============================================================ */
static void dry_run_sqrt(int n) {
    int sq = 1; while ((long long)(sq+1)*(sq+1) <= (long long)n) sq++;
    printf("\n--- Dry Run: count_divisors_sqrt(%d) ---\n", n);
    printf("  floor_sqrt(%d) = %d → check i from 1 to %d\n", n, sq, sq);
    int count = 0;
    for (long long i = 1; i * i <= (long long)n; i++) {
        if (n % (int)i == 0) {
            if ((int)i == n / (int)i) {
                printf("  i=%lld: %d/%lld=%lld (perfect sqrt) → +1 divisor\n",
                       i, n, i, (long long)(n/i));
                count++;
            } else {
                printf("  i=%lld: pair (%lld, %d/%lld=%lld) → +2 divisors\n",
                       i, i, n, i, (long long)(n/i));
                count += 2;
            }
        }
    }
    printf("  Total divisors: %d\n", count);
}

/* ============================================================
 * TEST SUITE
 * ============================================================ */
static void run_tests(void) {
    printf("\n=== Running Tests ===\n");

    /* Known divisor counts */
    assert(count_divisors_sqrt(1)   == 1);   /* only 1 */
    assert(count_divisors_sqrt(2)   == 2);   /* 1,2 */
    assert(count_divisors_sqrt(4)   == 3);   /* 1,2,4 */
    assert(count_divisors_sqrt(6)   == 4);   /* 1,2,3,6 */
    assert(count_divisors_sqrt(12)  == 6);   /* 1,2,3,4,6,12 */
    assert(count_divisors_sqrt(36)  == 9);   /* perfect square */
    assert(count_divisors_sqrt(100) == 9);
    assert(count_divisors_sqrt(7)   == 2);   /* prime: only 1,7 */

    /* Cross-validate brute vs sqrt */
    for (int n = 1; n <= 1000; n++) {
        int r1 = count_divisors_brute(n);
        int r2 = count_divisors_sqrt(n);
        int r3 = count_divisors_prime_factorization(n);
        if (r1 != r2 || r2 != r3) {
            printf("MISMATCH at n=%d: brute=%d sqrt=%d pf=%d\n", n, r1, r2, r3);
            assert(false);
        }
    }

    /* Sieve validation */
    int div_count[1001];
    count_divisors_sieve(div_count, 1000);
    for (int n = 1; n <= 1000; n++) {
        if (div_count[n] != count_divisors_brute(n)) {
            printf("SIEVE MISMATCH at n=%d\n", n);
            assert(false);
        }
    }

    /* Perfect numbers: 6, 28, 496, 8128 */
    assert(is_perfect_number(6)    == true);
    assert(is_perfect_number(28)   == true);
    assert(is_perfect_number(496)  == true);
    assert(is_perfect_number(8128) == true);
    assert(is_perfect_number(12)   == false);  /* abundant */
    assert(is_perfect_number(5)    == false);  /* deficient */

    printf("All tests PASSED.\n");
}

/* ============================================================
 * MAIN
 * ============================================================ */
int main(void) {
    printf("=========================================\n");
    printf(" Divisors — All Approaches\n");
    printf("=========================================\n");

    run_tests();

    dry_run_sqrt(36);
    dry_run_sqrt(12);
    dry_run_sqrt(7);

    /* Print divisors for sample values */
    int sample[] = {1, 6, 12, 28, 36, 100};
    int sc = (int)(sizeof(sample)/sizeof(sample[0]));
    for (int i = 0; i < sc; i++) {
        int n = sample[i];
        int divs[200];
        int cnt = get_divisors(n, divs, 200);
        printf("\nDivisors of %d [count=%d]: ", n, cnt);
        for (int j = 0; j < cnt; j++) printf("%d ", divs[j]);
        printf("\n  Sum=%lld, Perfect=%s\n",
               sum_of_divisors(n),
               is_perfect_number(n) ? "YES" : "NO");
    }

    printf("\n=== Divisor Count Table (1 to 20) ===\n");
    printf("%-5s %-8s %-8s %-10s\n", "n", "#divs", "sigma(n)", "type");
    for (int n = 1; n <= 20; n++) {
        long long s = sum_of_divisors(n);
        const char *type = (s == 2*n) ? "PERFECT" :
                           (s >  2*n) ? "abundant" : "deficient";
        printf("%-5d %-8d %-8lld %-10s\n", n, count_divisors_sqrt(n), s, type);
    }

    printf("\n=========================================\n");
    printf(" Complexity Summary\n");
    printf("=========================================\n");
    printf("%-30s %-20s %-10s\n", "Approach", "Time", "Space");
    printf("%-30s %-20s %-10s\n", "Brute (single n)",        "O(n)",          "O(1)");
    printf("%-30s %-20s %-10s\n", "Sqrt (single n)",         "O(sqrt(n))",    "O(1)");
    printf("%-30s %-20s %-10s\n", "Prime Factorization",     "O(sqrt(n))",    "O(log n)");
    printf("%-30s %-20s %-10s\n", "Sieve (range 1..N)",      "O(N log N)",    "O(N)");
    printf("%-30s %-20s %-10s\n", "SPF + factorize",         "O(sqrt(N)+log n)","O(N)");
    printf("%-30s %-20s %-10s\n", "Sum of divisors",         "O(sqrt(n))",    "O(1)");

    return 0;
}
