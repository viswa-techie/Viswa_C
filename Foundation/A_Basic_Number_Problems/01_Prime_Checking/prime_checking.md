# 01 — Prime Checking

**Category:** Foundation > A. Basic Number Problems  
**Difficulty:** Easy → Medium (with optimizations)  
**Priority:** TIER-1 (Must Master)  
**Companies:** Qualcomm, Intel, AMD, NXP, Texas Instruments, Nvidia  

---

## 1. PROBLEM UNDERSTANDING

### Simple English
A prime number is a natural number greater than 1 that has **no positive divisors other than 1 and itself**.  
Given an integer `n`, determine whether it is prime.

### Mathematical Definition
$$n \in \mathbb{P} \iff n > 1 \text{ and } \nexists k \in [2, n-1] \text{ such that } k \mid n$$

Optimized:
$$n \in \mathbb{P} \iff n > 1 \text{ and } \nexists k \in [2, \lfloor\sqrt{n}\rfloor] \text{ such that } k \mid n$$

**Why sqrt?**  
If `n = a * b` and both `a > sqrt(n)` and `b > sqrt(n)`, then `a*b > n` — contradiction.  
So at least one factor must be ≤ sqrt(n).

### Visual Explanation
```
n = 36
Factors: 1×36, 2×18, 3×12, 4×9, 6×6
          ↑                    ↑
     below sqrt(36)=6    at sqrt boundary

Once we cross sqrt(36)=6, factors mirror.
So checking up to sqrt(n) is SUFFICIENT.
```

### Real-World Analogy
Checking if a number is prime is like checking if a physical lock has no master key.  
You only need to try key sizes up to a certain threshold — beyond that, the pattern repeats.

### What Interviewers Are Testing
- Do you know the sqrt(n) optimization?
- Do you handle edge cases (0, 1, 2, negative)?
- Do you understand overflow when squaring?
- Can you implement the Sieve for batch queries?
- Do you know Miller-Rabin for large numbers?

### Common Traps
- Forgetting that `1` is NOT prime
- Forgetting that `2` IS prime (only even prime)
- Using `i * i <= n` causing integer overflow for large `n`
- Not handling negative numbers
- Infinite loop with `while(1)` if condition is wrong

---

## 2. INPUT/OUTPUT ANALYSIS

| Input | Output | Notes                            |
|-------|--------|----------------------------------|
| -5    | NO     | Negative numbers are not prime   |
| 0     | NO     | Not prime by definition          |
| 1     | NO     | Not prime by definition          |
| 2     | YES    | Smallest prime                   |
| 3     | YES    | Prime                            |
| 4     | NO     | 4 = 2×2                         |
| 97    | YES    | Prime                            |
| 100   | NO     | 100 = 2×50                       |
| INT_MAX (2147483647) | YES | Mersenne prime M31    |

### Overflow Warning
```c
// DANGEROUS: i * i can overflow for large n
for (int i = 2; i * i <= n; i++)  // overflow if i > ~46340 for int

// SAFE: cast to long long
for (long long i = 2; i * i <= (long long)n; i++)

// OR: use i <= n / i (avoids multiplication entirely)
for (int i = 2; i <= n / i; i++)
```

---

## 3. EDGE CASE ANALYSIS

| Case         | Value         | Expected | Reason                        |
|--------------|---------------|----------|-------------------------------|
| Zero         | 0             | NOT prime | Definitional                 |
| One          | 1             | NOT prime | Definitional                 |
| Negative     | -7            | NOT prime | Domain is natural numbers    |
| Smallest     | 2             | IS prime  | Only even prime              |
| Even > 2     | 4, 6, 100     | NOT prime | Divisible by 2               |
| Perfect sq   | 49 (7×7)      | NOT prime | Has divisor at sqrt          |
| Large prime  | 999983        | IS prime  | Must check up to sqrt(999983)≈999 |
| INT_MAX      | 2147483647    | IS prime  | Mersenne prime               |
| INT_MIN      | -2147483648   | NOT prime | Negative                     |

---

## 4. BRUTE FORCE SOLUTION

### Intuition
Check every number from 2 to `n-1` whether it divides `n`.

### Dry Run (n = 13)
```
Check 2: 13 % 2 = 1 → not divisor
Check 3: 13 % 3 = 1 → not divisor
...
Check 12: 13 % 12 = 1 → not divisor
→ No divisor found → PRIME
```

### Implementation
```c
/* Time: O(n), Space: O(1) */
int is_prime_brute(int n) {
    if (n <= 1) return 0;
    for (int i = 2; i < n; i++) {
        if (n % i == 0) return 0;
    }
    return 1;
}
```

### Complexity
- **Time:** O(n) — checks every number up to n
- **Space:** O(1)
- **Why inefficient:** For n = 10^6, this runs 10^6 iterations. Impractical.

---

## 5. BETTER SOLUTION — Sqrt Optimization

### Optimization Idea
Factors come in pairs. If `d` divides `n`, then `n/d` also divides `n`.  
The smaller of any factor pair is ≤ sqrt(n).  
So we only need to check divisors up to sqrt(n).

### Dry Run (n = 35)
```
sqrt(35) ≈ 5.9 → check up to 5
Check 2: 35 % 2 = 1 → no
Check 3: 35 % 3 = 2 → no
Check 4: 35 % 4 = 3 → no
Check 5: 35 % 5 = 0 → DIVISOR FOUND → NOT prime
```

### Implementation
```c
#include <stdbool.h>

/* Time: O(sqrt(n)), Space: O(1) */
bool is_prime_sqrt(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;         /* 2 and 3 are prime */
    if (n % 2 == 0 || n % 3 == 0) return false;

    /* Only check i = 6k±1 (further optimization) */
    for (int i = 5; (long long)i * i <= (long long)n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}
```

### Complexity
- **Time:** O(sqrt(n))
- **Space:** O(1)
- **Improvement:** sqrt(10^6) = 1000 iterations vs 10^6 brute force

---

## 6. OPTIMAL SOLUTION

### The 6k±1 Optimization
All primes > 3 are of the form `6k ± 1`.

**Proof:**  
Every integer fits one of: `6k, 6k+1, 6k+2, 6k+3, 6k+4, 6k+5`  
- `6k` → divisible by 6
- `6k+2` → divisible by 2
- `6k+3` → divisible by 3
- `6k+4` → divisible by 2
Only `6k+1` and `6k+5 (= 6k-1)` can be prime.

This reduces trial division iterations by ~1/3 compared to checking all odd numbers.

### Optimal Implementation
```c
#include <stdbool.h>
#include <stdint.h>

/*
 * is_prime_optimal - Checks primality using 6k±1 optimization
 * @n: integer to check
 * Returns: true if prime, false otherwise
 * Time: O(sqrt(n)/3), Space: O(1)
 */
bool is_prime_optimal(int n) {
    if (n <= 1)  return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;

    /*
     * All primes > 3 are of the form 6k±1.
     * Use (long long) cast to prevent i*i overflow for large n.
     */
    for (long long i = 5; i * i <= (long long)n; i += 6) {
        if (n % (int)i == 0 || n % (int)(i + 2) == 0)
            return false;
    }
    return true;
}
```

### Branch Optimization
```c
/*
 * Branchless early exit using lookup table for small primes.
 * Avoids branch misprediction for common small inputs.
 */
static const uint8_t small_prime_lut[30] = {
    0,0,1,1,0,1,0,1,0,0, /* 0-9  */
    0,1,0,1,0,0,0,1,0,1, /* 10-19 */
    0,0,0,1,0,0,0,0,0,1  /* 20-29 */
};

bool is_prime_lut_assisted(int n) {
    if (n < 30) return (n >= 0) ? (bool)small_prime_lut[n] : false;
    return is_prime_optimal(n);
}
```

---

## 7. MULTIPLE APPROACHES

### Approach 1: Brute Force O(n)
```c
bool is_prime_brute(int n) {
    if (n <= 1) return false;
    for (int i = 2; i < n; i++)
        if (n % i == 0) return false;
    return true;
}
```
- **Pros:** Simple, easy to explain
- **Cons:** O(n) — too slow for large inputs
- **Interview use:** Show you know it, then immediately optimize

---

### Approach 2: Sqrt Trial Division O(sqrt(n))
```c
bool is_prime_sqrt_only(int n) {
    if (n <= 1) return false;
    for (int i = 2; (long long)i * i <= (long long)n; i++)
        if (n % i == 0) return false;
    return true;
}
```
- **Pros:** Classic, well-known
- **Cons:** Still checks even numbers unnecessarily
- **Interview use:** Good intermediate answer

---

### Approach 3: Skip Even Numbers O(sqrt(n)/2)
```c
bool is_prime_skip_even(int n) {
    if (n <= 1) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (int i = 3; (long long)i * i <= (long long)n; i += 2)
        if (n % i == 0) return false;
    return true;
}
```
- **Pros:** 2× faster than pure sqrt
- **Cons:** Still checks multiples of 3
- **Interview use:** Good to show iterative improvement

---

### Approach 4: 6k±1 Optimization O(sqrt(n)/3) ← PREFERRED
See Optimal Solution above.
- **Pros:** ~3× faster than basic sqrt, correct, clean
- **Cons:** Slightly harder to explain the math
- **Interview use:** Demonstrates mathematical thinking

---

### Approach 5: Sieve of Eratosthenes (Batch Queries)
```c
#include <stdlib.h>
#include <string.h>

/*
 * sieve - Mark all primes up to limit
 * @is_prime: boolean array of size limit+1
 * @limit: upper bound (inclusive)
 * Time: O(n log log n), Space: O(n)
 */
void sieve(bool *is_prime, int limit) {
    memset(is_prime, 1, (size_t)(limit + 1) * sizeof(bool));
    is_prime[0] = is_prime[1] = false;

    for (int i = 2; (long long)i * i <= (long long)limit; i++) {
        if (is_prime[i]) {
            /* Mark all multiples of i starting from i*i */
            for (int j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }
}
```
- **Pros:** O(1) per query after O(n log log n) setup; best for batch queries
- **Cons:** O(n) memory; not suitable for a single query
- **Interview use:** Must know for "list all primes up to N" problems

---

### Approach 6: Miller-Rabin Probabilistic (Large Numbers)
```c
/*
 * Deterministic Miller-Rabin for n < 3,215,031,751 (32-bit)
 * Uses witnesses: {2, 3, 5, 7}
 * Time: O(k * log^2 n), Space: O(1)
 */
typedef unsigned long long ull;

ull mulmod(ull a, ull b, ull m) {
    return (__uint128_t)a * b % m;
}

ull powmod(ull base, ull exp, ull mod) {
    ull result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = mulmod(result, base, mod);
        base = mulmod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

bool miller_rabin_witness(ull n, ull a) {
    if (n % a == 0) return n == a;
    ull d = n - 1;
    int r = 0;
    while (d % 2 == 0) { d /= 2; r++; }

    ull x = powmod(a, d, n);
    if (x == 1 || x == n - 1) return true;
    for (int i = 0; i < r - 1; i++) {
        x = mulmod(x, x, n);
        if (x == n - 1) return true;
    }
    return false;
}

bool is_prime_miller_rabin(ull n) {
    if (n < 2) return false;
    /* Deterministic witnesses for n < 3,215,031,751 */
    const ull witnesses[] = {2, 3, 5, 7};
    for (int i = 0; i < 4; i++) {
        if (!miller_rabin_witness(n, witnesses[i])) return false;
    }
    return true;
}
```
- **Pros:** Handles 64-bit numbers; deterministic for known witness sets
- **Cons:** Complex implementation; overkill for small n
- **Interview use:** Impressive for cryptography/security roles

---

### Approach 7: Lookup Table for Very Small n
```c
/* For embedded systems with n bounded small (e.g., n < 256) */
static const uint32_t prime_bits[8] = {
    /* Bits 0-31, 32-63, 64-95, 96-127, 128-159, 160-191, 192-223, 224-255 */
    0xA08A28ACU, 0x28208A20U, 0x02088288U, 0x800228A0U,
    0x20A00A08U, 0x80282088U, 0x800800A0U, 0x08028228U
};

bool is_prime_lut(unsigned int n) {
    if (n > 255) return is_prime_optimal((int)n);
    return (prime_bits[n >> 5] >> (n & 31)) & 1;
}
```
- **Pros:** O(1) for small numbers; zero branches in hot path; ideal for MCU firmware
- **Cons:** Requires precomputed table; limited range
- **Interview use:** Shows embedded/firmware mindset

---

## 8. LOW-LEVEL C DISCUSSION

### Integer Overflow
```c
/* UNSAFE for large n: i*i overflows when i > sqrt(INT_MAX) ≈ 46340 */
for (int i = 2; i * i <= n; i++)

/* SAFE: use long long for comparison */
for (long long i = 2; i * i <= (long long)n; i++)

/* SAFE: use division to avoid multiplication */
for (int i = 2; i <= n / i; i++)
/* Warning: n/i truncates; still correct since we want floor(sqrt(n)) */
```

### Signedness
```c
/* n % 2 == 0 works for both positive and negative in C99/C11 */
/* But logically, primes are only defined for positive integers */
/* Always guard with: if (n <= 1) return false; */
```

### Undefined Behavior
- Signed integer overflow is **UB** in C: `i * i` can overflow `int`
- Solution: promote to `long long` or use `i <= n / i`

### Cache and Branch
- The sieve is cache-friendly when iterating linearly
- The 6k±1 loop has very few branches
- For embedded MCU: LUT is best (zero computation, predictable timing)

### Stack vs Heap
- All approaches above use O(1) stack
- Sieve requires heap allocation: `malloc((limit+1) * sizeof(bool))`

---

## 9. BITWISE INSIGHTS

### Fast Even Check
```c
/* Faster than n % 2 == 0 on some architectures */
if (n & 1 == 0 && n != 2) return false;  /* n is even and not 2 */
```

### Power of 2 Special Case
```c
/* If n is a power of 2 and n > 2, it's NOT prime */
/* Powers of 2: n > 0 && (n & (n-1)) == 0 */
if (n > 2 && (n & (n-1)) == 0) return false;
```

### Bit Manipulation in Sieve
```c
/* Bit-packed sieve: 1 bit per number → 8× memory reduction */
/* Mark composite: sieve_bits[n/64] |= (1ULL << (n%64)) */
/* Check: (sieve_bits[n/64] >> (n%64)) & 1 */
```

---

## 10. MATHEMATICAL INSIGHTS

### Why All Primes > 3 Are 6k±1
Every integer `n` can be written as one of `{6k, 6k+1, 6k+2, 6k+3, 6k+4, 6k+5}`:
- `6k` ≡ 0 (mod 2) → composite (divisible by 2)
- `6k+2` ≡ 0 (mod 2) → composite
- `6k+4` ≡ 0 (mod 2) → composite
- `6k+3` ≡ 0 (mod 3) → composite
Only `6k+1` and `6k+5` can be prime.  
`6k+5 = 6(k+1)-1 = 6k'-1` for `k'=k+1`, confirming the ±1 form.

### Prime Counting Function π(n)
$$\pi(n) \approx \frac{n}{\ln(n)}$$
Primes become less dense for larger n — relevant for primality test frequency.

### Bertrand's Postulate
For every `n > 1`, there exists a prime `p` such that `n < p < 2n`.

### Sieve Time Complexity: O(n log log n)
$$\sum_{p \leq n, p \text{ prime}} \frac{n}{p} \approx n \cdot \ln(\ln(n))$$

---

## 11. INTERVIEWER FOLLOW-UP QUESTIONS

**Q1: Can you do it in O(1)?**  
A: For fixed range, use a precomputed lookup table. O(1) for small n.  
For general n: No O(1) deterministic algorithm is known.

**Q2: How to avoid overflow in the loop condition?**  
A: Use `i <= n / i` instead of `i * i <= n`, or cast to `long long`.

**Q3: Handle streaming input — is the number prime?**  
A: For single queries: use the 6k±1 approach.  
For repeated queries: precompute sieve up to expected max.

**Q4: Extend to 64-bit integers?**  
A: Use Miller-Rabin with deterministic witnesses. For n < 3,317,044,064,679,887,385,961,981: witnesses {2,3,5,7,11,13,17,19,23,29,31,37} suffice.

**Q5: Can this be parallelized?**  
A: The segmented sieve can be parallelized: each thread processes a segment [L, R].

**Q6: What about negative numbers?**  
A: By mathematical convention, primality is defined only for positive integers. Return false for n ≤ 1.

**Q7: Can you list all primes up to N efficiently?**  
A: Sieve of Eratosthenes: O(n log log n) time, O(n) space.  
Segmented sieve for large N with O(sqrt(N)) space.

**Q8: What if you cannot use division or modulo?**  
A: Use addition-based sieve (mark multiples by addition, not division).

---

## 12. COMMON MISTAKES

| Mistake | Code | Fix |
|---------|------|-----|
| n=1 is prime | `if (n < 2) return false` missing | Add proper guard |
| n=2 skipped | Loop starts at 2 and exits if even | Special-case n==2 |
| Overflow in condition | `i * i <= n` | Use `(long long)i*i` |
| Loop to n instead of sqrt | `for(i=2;i<n;i++)` | Use `i*i <= n` |
| Return wrong type | `int` instead of `bool` | Use `<stdbool.h>` |
| Sieve off-by-one | Array size `n` not `n+1` | Use `n+1` |
| `i*i` starting from 4 in sieve | `j = i*i` overflows | Cast `j` to `long long` or verify range |

---

## 13. DEBUGGING GUIDE

### Print-Based Debugging
```c
bool is_prime_debug(int n) {
    printf("Checking primality of %d\n", n);
    if (n <= 1) { printf("  n <= 1: not prime\n"); return false; }
    if (n == 2) { printf("  n == 2: prime\n"); return true; }
    if (n % 2 == 0) { printf("  even: not prime\n"); return false; }
    for (int i = 3; (long long)i*i <= (long long)n; i += 2) {
        if (n % i == 0) {
            printf("  divisor found: %d divides %d\n", i, n);
            return false;
        }
    }
    printf("  no divisors found: prime\n");
    return true;
}
```

### GDB Approach
```
gdb ./solution
break is_prime_optimal
run
print n
next          # step through loop
watch i       # watch loop variable
```

### ASan/UBSan
```bash
gcc -fsanitize=address,undefined -g solution.c -o solution
./solution
# Will catch: signed overflow, null deref, array out-of-bounds
```

### Common Failing Inputs
- `n = 0`, `n = 1` → incorrectly returned as prime
- `n = 2` → missed special case
- `n = INT_MAX` → overflow in `i*i`

---

## 14. TEST CASES

### Normal Cases
```c
assert(is_prime_optimal(2)   == true);
assert(is_prime_optimal(3)   == true);
assert(is_prime_optimal(5)   == true);
assert(is_prime_optimal(7)   == true);
assert(is_prime_optimal(11)  == true);
assert(is_prime_optimal(97)  == true);
assert(is_prime_optimal(4)   == false);
assert(is_prime_optimal(6)   == false);
assert(is_prime_optimal(100) == false);
```

### Edge Cases
```c
assert(is_prime_optimal(0)          == false);
assert(is_prime_optimal(1)          == false);
assert(is_prime_optimal(-1)         == false);
assert(is_prime_optimal(-7)         == false);
assert(is_prime_optimal(2147483647) == true);  /* INT_MAX, Mersenne prime */
```

### Stress Cases
```c
/* Verify against brute force for all n in [0, 10000] */
for (int n = 0; n <= 10000; n++) {
    assert(is_prime_brute(n) == is_prime_optimal(n));
}
```

---

## 15. PERFORMANCE BENCHMARKING

| Approach       | n=10^3  | n=10^6  | n=10^9  | Notes                    |
|----------------|---------|---------|---------|--------------------------|
| Brute force    | ~500 ns | ~500 µs | ~500 ms | Linear                   |
| Sqrt           | ~15 ns  | ~1 µs   | ~32 µs  | Square root              |
| 6k±1           | ~10 ns  | ~0.7 µs | ~21 µs  | 1/3 fewer iterations     |
| Sieve (setup)  | N/A     | ~5 ms   | N/A     | O(n log log n) one-time  |
| Sieve (query)  | O(1)    | O(1)    | N/A     | After setup              |
| Miller-Rabin   | ~100 ns | ~100 ns | ~100 ns | O(k log^2 n), k=4        |

### Modulo Cost
- `%` (modulo) is one of the most expensive integer operations (~20-40 cycles on x86)
- The 6k±1 trick reduces modulo calls by ~66% vs checking all i
- LUT approach: zero modulo operations

### Branch Prediction
- Brute force: highly predictable (always `n % i != 0` for large prime)
- 6k±1 loop: tight, predictable branch pattern
- Sieve: memory-bound, not compute-bound

---

## 16. REAL-WORLD APPLICATIONS

| Domain         | Application                                           |
|----------------|-------------------------------------------------------|
| Cryptography   | RSA key generation (large prime selection)            |
| Hash tables    | Prime-sized tables reduce collision clustering        |
| PRNG           | Prime moduli in linear congruential generators        |
| Networking     | Diffie-Hellman key exchange (prime field arithmetic)  |
| Embedded       | Timer period selection using primes for less aliasing |
| Compilers      | Hash function prime constants                         |
| DSP            | FFT size selection (often uses prime factorization)   |
| OS             | Buddy allocator: prime-based block sizing strategies  |

---

## 17. RELATED PROBLEMS

| Problem                   | Relation                  | Harder/Easier |
|---------------------------|---------------------------|---------------|
| Count primes up to N      | Sieve application         | Easier        |
| Goldbach's conjecture check | Prime pair finding       | Medium        |
| Prime factorization        | Extension of trial div    | Medium        |
| Smallest prime factor sieve| SPF sieve                | Medium        |
| Nth prime                  | Sieve + indexing          | Easy          |
| Twin primes up to N        | Sieve + pair check        | Easy          |
| Primorial                  | Product of primes         | Medium        |
| Miller-Rabin for 64-bit   | Advanced primality        | Hard          |
| AKS primality test         | Polynomial-time exact     | Very Hard     |

---

## 18. FINAL TAKEAWAYS

- Always handle `n <= 1` as NOT prime
- Always guard overflow with `(long long)i * i` or use `i <= n/i`
- For single queries: 6k±1 is the sweet spot
- For batch queries: Sieve of Eratosthenes
- For 64-bit large numbers: Miller-Rabin
- For embedded/MCU: LUT for small range, avoids all computation
- The sqrt observation is the single most important insight
- **Interview mantra:** Start brute → sqrt → 6k±1 → Sieve if needed

---

## 19. CODE QUALITY REQUIREMENTS

All implementations in `prime_checking.c`:
- Compile with: `gcc -Wall -Wextra -std=c11 -O2`
- UB-free: no signed integer overflow
- Clean naming: `is_prime_*` prefix
- Include `#include <stdbool.h>` for `bool`
- Complexity documented in comment above each function

---

## 20. ADVANCED EXTENSIONS

### Embedded Systems Version
- Use LUT for small fixed range (reduces power consumption on MCU)
- Avoid recursion (stack overflow risk on 512-byte stacks)
- Use `uint8_t`/`uint16_t` for memory-constrained devices

### Multi-Threaded Segmented Sieve
```c
/* Divide [2, N] into segments. Each thread processes one segment. */
/* Thread i: segment [i * seg_size, (i+1) * seg_size - 1] */
/* Requires: pre-sieve primes up to sqrt(N) on main thread first */
```

### SIMD Vectorization Idea
```c
/* Vectorize the sieve marking: use 128-bit SSE registers */
/* Process 16 bytes (booleans) per instruction instead of 1 */
/* ~16× throughput improvement for the inner marking loop */
```

### Real-Time Constraint
- If time budget is strict: precompute all primes offline
- Store in ROM/flash on embedded device
- Zero runtime computation required

### Bit-Packed Sieve (Memory Optimization)
```c
/* Use 1 bit per entry instead of 1 byte */
/* 8× memory reduction: sieve up to 10^8 needs 12.5 MB instead of 100 MB */
uint32_t sieve_bits[LIMIT / 32 + 1];
#define SET_COMPOSITE(n)   (sieve_bits[(n)>>5] |=  (1u << ((n)&31)))
#define IS_COMPOSITE(n)    (sieve_bits[(n)>>5] &   (1u << ((n)&31)))
```
