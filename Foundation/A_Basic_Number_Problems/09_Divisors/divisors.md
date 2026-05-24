# 09 — Divisors (Count & Find All Divisors)

**Category:** Foundation > A. Basic Number Problems  
**Difficulty:** Easy → Medium (prime factorization, sieve)  
**Priority:** TIER-1  
**Companies:** Qualcomm, Intel, AMD, NXP, Texas Instruments  

---

## 1. PROBLEM UNDERSTANDING

### Simple English
A **divisor** (or factor) of n is any positive integer d such that `n % d == 0`.  
Given n, find:
1. All divisors of n
2. Count of divisors
3. Sum of divisors

### Mathematical Definition
$$D(n) = \{d \in \mathbb{Z}^+ : d \mid n\}$$
$$\tau(n) = |D(n)|, \quad \sigma(n) = \sum_{d \in D(n)} d$$

### Visual Explanation
```
n = 12
Divisors: 1, 2, 3, 4, 6, 12  →  count = 6
Pairs:    (1,12), (2,6), (3,4)  ← three pairs from sqrt(12)≈3.46

n = 36
Divisors: 1, 2, 3, 4, 6, 9, 12, 18, 36  →  count = 9
Pairs:    (1,36),(2,18),(3,12),(4,9),(6,6)  ← 4 pairs + 1 center (6=sqrt(36))
Perfect square → odd number of divisors!
```

### Key Insight: Pairing
For every divisor `d < sqrt(n)`, there is a corresponding divisor `n/d > sqrt(n)`.  
So we only need to check up to sqrt(n). This gives **O(sqrt(n))** instead of O(n).

### What Interviewers Are Testing
- The sqrt(n) pairing optimization (same as prime checking)
- Overflow when `i * i <= n`
- Perfect square handling (add sqrt only once)
- Prime factorization formula for count: `(e1+1)(e2+1)...(ek+1)`
- Sieve for range queries

### Common Traps
- `i * i <= n` overflows for large n → use `(long long)i * i`
- Perfect square: `i == n/i` → don't count twice
- `n = 1`: only one divisor (1 itself)
- Divisors include 1 and n itself

---

## 2. INPUT/OUTPUT ANALYSIS

| n   | Divisors              | Count | Sum   | Type        |
|-----|-----------------------|-------|-------|-------------|
| 1   | {1}                   | 1     | 1     | —           |
| 2   | {1, 2}                | 2     | 3     | prime       |
| 4   | {1, 2, 4}             | 3     | 7     | prime sq    |
| 6   | {1, 2, 3, 6}          | 4     | 12    | perfect!    |
| 12  | {1,2,3,4,6,12}        | 6     | 28    | abundant    |
| 36  | {1,2,3,4,6,9,12,18,36}| 9     | 91    | abundant    |
| 7   | {1, 7}                | 2     | 8     | prime       |
| 28  | {1,2,4,7,14,28}       | 6     | 56    | perfect!    |
| 100 | {1,2,4,5,10,20,25,50,100} | 9 | 217   | abundant    |

---

## 3. EDGE CASES

| Case          | n    | Count | Notes                         |
|---------------|------|-------|-------------------------------|
| n = 1         | 1    | 1     | Only divisor: 1               |
| n = 2 (prime) | 2    | 2     | Only 1 and 2                  |
| Perfect square| 36   | odd   | sqrt(36)=6 counted once       |
| Large prime   | 9973 | 2     | Only 1 and 9973               |
| Highly composite | 720 | 30  | Has many divisors             |
| n = 0         | 0    | N/A   | Every positive int divides 0  |
| n < 0         | -12  | Handle | Not defined — use abs        |

---

## 4. BRUTE FORCE — Check All i from 1 to n

```c
int count_divisors_brute(int n) {
    int count = 0;
    for (int i = 1; i <= n; i++)
        if (n % i == 0) count++;
    return count;
}
```
**Time:** O(n) — impractical for n = 10^9  
**Space:** O(1)

---

## 5. BETTER SOLUTION — Sqrt Optimization

### Key Insight: Factor Pairs
If `d` divides `n`, so does `n/d`. And `min(d, n/d) ≤ sqrt(n)`.  
So we only need to check `i` from 1 to sqrt(n).

### Dry Run (n = 12)
```
sqrt(12) ≈ 3.46 → check i = 1, 2, 3

i=1: 12%1=0 → pair (1, 12) → count += 2
i=2: 12%2=0 → pair (2, 6)  → count += 2
i=3: 12%3=0 → pair (3, 4)  → count += 2
Total: 6 divisors ✓
```

### Dry Run (n = 36, perfect square)
```
i=1: 36%1=0 → pair (1,36) → count += 2
i=2: 36%2=0 → pair (2,18) → count += 2
i=3: 36%3=0 → pair (3,12) → count += 2
i=4: 36%4=0 → pair (4,9)  → count += 2
i=5: 36%5≠0 → skip
i=6: 36%6=0 → i==36/6=6 (SAME) → count += 1
Total: 9 divisors ✓
```

### Implementation
```c
int count_divisors_sqrt(int n) {
    int count = 0;
    for (long long i = 1; i * i <= (long long)n; i++) {
        if (n % (int)i == 0) {
            count++;                        /* divisor i */
            if ((int)i != n / (int)i) count++; /* divisor n/i if different */
        }
    }
    return count;
}
```

---

## 6. OPTIMAL SOLUTIONS

### For Single n: Prime Factorization Method

If `n = p1^a1 × p2^a2 × ... × pk^ak`, then:
$$\tau(n) = (a_1+1)(a_2+1)\cdots(a_k+1)$$

```c
int count_divisors_prime_factorization(int n) {
    int count = 1;
    for (int p = 2; (long long)p * p <= (long long)n; p++) {
        int exp = 0;
        while (n % p == 0) { exp++; n /= p; }
        count *= (exp + 1);
    }
    if (n > 1) count *= 2;   /* remaining prime with exponent 1 */
    return count;
}
```

**Example:** 12 = 2² × 3¹ → τ(12) = (2+1)(1+1) = 6 ✓  
**Time:** O(sqrt(n)), **Space:** O(1)

### For Range [1..N]: Sieve O(N log N)

```c
for (int i = 1; i <= N; i++)
    for (int j = i; j <= N; j += i)
        div_count[j]++;
```

---

## 7. MULTIPLE APPROACHES

### Approach 1: Brute Force O(n) — Collect all
- Loop i from 1 to n
- Simple but slow for large n

### Approach 2: Sqrt Optimization O(sqrt(n)) [PREFERRED for single n]
- Loop i from 1 to sqrt(n)
- Add both i and n/i
- Handle perfect square separately

### Approach 3: Prime Factorization O(sqrt(n))
- Factor n, compute exponents
- Multiply `(e+1)` for each prime factor
- Only gives count, not the actual divisor list

### Approach 4: Sieve O(N log N) [PREFERRED for range 1..N]
- For each i, mark all multiples: O(N/i) per i
- Total: O(N × harmonic series) = O(N log N)

### Approach 5: SPF (Smallest Prime Factor) Sieve
- Precompute SPF for [2..N] once
- Factorize any n in O(log n) using SPF table
- Best when many individual queries are needed

---

## 8. LOW-LEVEL C DISCUSSION

### Overflow in Loop Condition
```c
/* UNSAFE: i * i overflows int when i > 46340 */
for (int i = 1; i * i <= n; i++)

/* SAFE: cast to long long */
for (long long i = 1; i * i <= (long long)n; i++)

/* ALSO SAFE: compare via division */
for (int i = 1; i <= n / i; i++)
```

### Sorting Divisors
```c
/* Sqrt approach gives divisors in two groups:
 * Group 1 (small): 1, 2, 3, ... (in order)
 * Group 2 (large): n/1, n/2, n/3, ... (descending)
 * Reverse group 2 and concatenate for sorted output */
```

### Memory for Divisor List
```c
/* Maximum divisors for int32: 720720 has 240 divisors */
/* Safe array size: int divs[1500] covers all int32 cases */
/* Alternatively: dynamically allocate */
```

---

## 9. BITWISE INSIGHTS

### Powers of 2 Have Simple Divisor Counts
```c
/* 2^k has k+1 divisors: 1, 2, 4, ..., 2^k */
/* count_divisors(1 << k) = k + 1 */
/* Detectable via: __builtin_popcount(n) == 1 → n is power of 2 */
```

### Highly Composite Numbers
```c
/* Numbers with more divisors than any smaller positive integer */
/* 1, 2, 4, 6, 12, 24, 36, 48, 60, 120, 180, 240, 360, 720, ... */
/* Used for: timer granularity, cache sizing */
```

---

## 10. MATHEMATICAL INSIGHTS

### Multiplicative Property of τ(n)
$$\tau(p^a \cdot q^b) = (a+1)(b+1) \quad \text{if } \gcd(p,q) = 1$$

τ is a multiplicative function: `τ(mn) = τ(m)τ(n)` if gcd(m,n) = 1.

### Number of Divisors is Odd iff n is a Perfect Square
**Proof:** Divisors pair up (d, n/d). Unpaired only when d = sqrt(n). So odd count ↔ perfect square.

### Average Number of Divisors
$$\frac{1}{N} \sum_{n=1}^{N} \tau(n) \approx \ln(N) + (2\gamma - 1)$$
where γ ≈ 0.5772 (Euler-Mascheroni constant).

### Perfect Numbers
- σ(n) - n = n → sum of proper divisors = n
- Even perfect numbers: `2^(p-1) × (2^p - 1)` where `2^p - 1` is a Mersenne prime
- Known even perfects: 6, 28, 496, 8128, 33550336, ...
- Odd perfect numbers: none known; likely don't exist

### Sigma Function and Divisor Sum
$$\sigma(p^a) = 1 + p + p^2 + \cdots + p^a = \frac{p^{a+1} - 1}{p - 1}$$

---

## 11. INTERVIEWER FOLLOW-UP QUESTIONS

**Q1: Why only check up to sqrt(n)?**  
A: Divisors come in pairs (d, n/d). One of each pair is ≤ sqrt(n). So sqrt(n) checks are sufficient.

**Q2: Why does a perfect square have an odd number of divisors?**  
A: Its sqrt(n) divisor has no pair (it pairs with itself) → odd count.

**Q3: How many divisors does n = 720 have?**  
A: 720 = 2⁴ × 3² × 5¹ → (4+1)(2+1)(1+1) = 5×3×2 = 30.

**Q4: Find divisor count for all numbers 1..N efficiently?**  
A: Sieve approach: O(N log N) time, O(N) space.

**Q5: What is the number with most divisors under 10^6?**  
A: 720720 (240 divisors) — a highly composite number.

**Q6: What are perfect numbers?**  
A: Sum of proper divisors = n. First few: 6, 28, 496, 8128.

**Q7: How to get divisors in sorted order without sorting?**  
A: Collect small divisors (ascending) and corresponding large ones (descending) during the sqrt loop. Reverse large ones and concatenate.

**Q8: Sum of all divisors of n?**  
A: Use same sqrt loop, adding both `i` and `n/i`. Or use formula: `σ(p^a) = (p^(a+1)-1)/(p-1)` from prime factorization.

---

## 12. COMMON MISTAKES

| Mistake | Fix |
|---------|-----|
| `i * i <= n` overflow | Use `(long long)i * i <= (long long)n` |
| Count sqrt twice for perfect sq | Check `if (i != n/i)` before adding n/i |
| Output unsorted divisors | Collect small + reverse-large |
| n = 0 causes infinite loop | Guard: `if (n <= 0) return 0` |
| n = 1 handled incorrectly | 1 has exactly 1 divisor: {1} |

---

## 13. DEBUGGING GUIDE

```c
/* Trace divisor pairs */
for (long long i = 1; i * i <= (long long)n; i++) {
    if (n % (int)i == 0) {
        printf("Pair: (%lld, %d)\n", i, n / (int)i);
    }
}
```

---

## 14. TEST CASES

```c
assert(count_divisors_sqrt(1)   == 1);
assert(count_divisors_sqrt(2)   == 2);
assert(count_divisors_sqrt(4)   == 3);
assert(count_divisors_sqrt(6)   == 4);
assert(count_divisors_sqrt(12)  == 6);
assert(count_divisors_sqrt(36)  == 9);
assert(count_divisors_sqrt(7)   == 2);  /* prime */
assert(count_divisors_sqrt(100) == 9);

/* Perfect numbers */
assert(is_perfect_number(6)    == true);
assert(is_perfect_number(28)   == true);
assert(is_perfect_number(496)  == true);
assert(is_perfect_number(5)    == false);
```

---

## 15. PERFORMANCE

| Approach     | n=100  | n=10^6 | n=10^9 | Range 1..10^6   |
|--------------|--------|--------|--------|-----------------|
| Brute        | 100 ops| 10^6   | 10^9   | N/A             |
| Sqrt         | 10 ops | 1000   | 31623  | N/A             |
| Prime Factor | 10 ops | 1000   | 31623  | N/A             |
| Sieve        | N/A    | N/A    | N/A    | O(N log N)      |

For a single query: sqrt is best.  
For range 1..N: sieve is far faster (O(N log N) vs O(N√N) for N sqrt calls).

---

## 16. REAL-WORLD APPLICATIONS

| Domain        | Application                                           |
|---------------|-------------------------------------------------------|
| Embedded      | Timer period factorization for synchronized clocks    |
| Networking    | Packet size divisors for fragmentation alignment      |
| Cryptography  | RSA: factorization security relies on few divisors    |
| Math          | Perfect/abundant/deficient classification             |
| OS            | Block size selection: divisors of disk sector size    |
| Compilers     | Loop unrolling factor: divisors of iteration count    |
| DSP           | FFT size selection: needs highly factorable numbers   |
| Game Dev      | Grid sizing: divisors for even subdivision            |

---

## 17. RELATED PROBLEMS

| Problem                  | Relation                           |
|--------------------------|------------------------------------|
| Count prime factors      | Related factorization              |
| Perfect number check     | sigma(n) - n == n                  |
| Euler's Totient φ(n)     | Count coprime divisors             |
| Smallest prime factor    | SPF sieve                          |
| Number of divisors ≤ k   | Threshold variant                  |
| Divisor sum formula      | Multiplicative sigma function      |
| GCD/LCM                  | Uses divisibility                  |
| Prime factorization      | Foundation for divisor count       |

---

## 18. FINAL TAKEAWAYS

- Loop only to sqrt(n): O(sqrt(n)) vs O(n) brute force
- For perfect squares: `i == n/i` → add divisor only once
- Divisor count formula: n = p1^a1 × ... → τ(n) = (a1+1)(a2+1)...
- Odd count of divisors ↔ n is a perfect square
- Sieve for range queries [1..N]: O(N log N), far faster than N × sqrt calls
- **Mantra:** loop to sqrt → add i + n/i → guard for i == n/i

---

## 20. ADVANCED EXTENSIONS

### Euler's Totient φ(n) (Coprime Count)
```c
/* Count integers [1..n] coprime to n */
int euler_totient(int n) {
    int result = n;
    for (int p = 2; (long long)p*p <= (long long)n; p++) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
    }
    if (n > 1) result -= result / n;
    return result;
}
```

### Divisor Sum Formula (O(sqrt) via Factorization)
```c
long long sigma_formula(int n) {
    long long result = 1;
    for (long long p = 2; p*p <= n; p++) {
        if (n % p == 0) {
            long long pk = 1;
            while (n % p == 0) { pk *= p; n /= (int)p; }
            result *= (pk * p - 1) / (p - 1);  /* geometric series */
        }
    }
    if (n > 1) result *= (n + 1);  /* prime with exp 1: (p^2-1)/(p-1) = p+1 */
    return result;
}
```

### Embedded Highly Composite Number Table
```c
/* Precomputed HCNs for timer/buffer sizing */
static const int hcn[] = {1,2,4,6,12,24,36,48,60,120,180,240,360,720,840,...};
/* Find smallest HCN >= required_size: binary search in hcn[] */
```
