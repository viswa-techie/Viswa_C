# 06 — Fibonacci

**Category:** Foundation > A. Basic Number Problems  
**Difficulty:** Easy → Hard (with matrix exponentiation)  
**Priority:** TIER-1 (Most Important)  
**Companies:** Qualcomm, Intel, AMD, NXP, Nvidia, MediaTek, Apple Silicon  

---

## 1. PROBLEM UNDERSTANDING

### Simple English
Fibonacci sequence: 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, ...  
Each number = sum of the two preceding numbers.  
Given n, find F(n).

### Mathematical Definition
$$F(0) = 0, \quad F(1) = 1, \quad F(n) = F(n-1) + F(n-2) \text{ for } n \geq 2$$

### Visual Explanation
```
n:    0  1  2  3  4  5  6   7   8   9   10
F(n): 0  1  1  2  3  5  8  13  21  34   55
              ↑
         F(2) = F(1)+F(0) = 1+0 = 1
```

### Real-World Analogy
Nature: spiral patterns in sunflowers, shells follow Fibonacci ratios.  
CS: Fibonacci heap, cache replacement, divide-and-conquer balance analysis.

### What Interviewers Are Testing
- **NEVER** implement naive O(2^n) recursive for production
- Know iterative O(n) O(1)-space version
- Know matrix exponentiation O(log n) — shows mathematical depth
- Know Binet's formula limitation (FP precision)
- Overflow handling: F(93) is max for uint64

---

## 2. INPUT/OUTPUT ANALYSIS

| n   | F(n)                          | Fits in         |
|-----|-------------------------------|-----------------|
| 0   | 0                             | int             |
| 1   | 1                             | int             |
| 10  | 55                            | int             |
| 20  | 6765                          | int             |
| 46  | 1836311903                    | int32 (last)    |
| 50  | 12586269025                   | uint64          |
| 93  | 12200160415121876738          | uint64 (last)   |
| 94  | OVERFLOW for uint64           | Need big int    |

---

## 3. EDGE CASES

| Case        | n   | F(n) | Notes                          |
|-------------|-----|------|--------------------------------|
| Zero        | 0   | 0    | F(0) = 0 by definition         |
| One         | 1   | 1    | Base case                      |
| Two         | 2   | 1    | F(2) = F(1)+F(0) = 1+0 = 1    |
| Negative    | -1  | 0    | Return 0 (not defined)         |
| int32 limit | 46  | 1836311903 | Last F that fits int32 |
| uint64 limit| 93  | 12200160415121876738 | Last uint64 |
| Overflow    | 94+ | N/A  | Need big integer               |

---

## 4. BRUTE FORCE — Naive Recursive O(2^n)

### Why It's Bad
```
F(5) calls:
         F(5)
       /      \
    F(4)       F(3)
   /    \     /   \
 F(3)  F(2) F(2) F(1)
```
F(3) is computed TWICE, F(2) THREE times.  
For F(40): ~2^40 = 10^12 operations → seconds to run.

### Implementation
```c
ull fibonacci_naive(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;
    return fibonacci_naive(n-1) + fibonacci_naive(n-2);
}
```
**Time:** O(2^n) — exponential  
**Space:** O(n) — call stack depth  
**Use:** Only to demonstrate why optimization is needed.

---

## 5. BETTER SOLUTION — Memoized O(n)

### Idea
Cache already-computed values. Each F(k) computed exactly once.

```c
ull memo[94] = {[0]=0, [1]=1};
bool computed[94] = {[0]=true, [1]=true};

ull fibonacci_memoized(int n) {
    if (computed[n]) return memo[n];
    memo[n] = fibonacci_memoized(n-1) + fibonacci_memoized(n-2);
    computed[n] = true;
    return memo[n];
}
```
**Time:** O(n)  
**Space:** O(n) — memo table + O(n) stack  

---

## 6. OPTIMAL SOLUTIONS

### Optimal O(n): Two-Variable Iterative

```c
ull fibonacci_iterative(int n) {
    if (n == 0) return 0;
    ull prev = 0, curr = 1;
    for (int i = 2; i <= n; i++) {
        ull next = prev + curr;
        prev = curr;
        curr = next;
    }
    return curr;
}
```
**Time:** O(n), **Space:** O(1) — the sweet spot for most use cases.

### Optimal O(log n): Matrix Exponentiation

$$\begin{pmatrix} F(n+1) \\ F(n) \end{pmatrix} = \begin{pmatrix} 1 & 1 \\ 1 & 0 \end{pmatrix}^n \begin{pmatrix} 1 \\ 0 \end{pmatrix}$$

```c
/* See fibonacci.c for full implementation */
ull fibonacci_matrix(int n);   /* O(log n) time, O(1) space */
```

### Optimal O(log n): Fast Doubling
Uses two identities:
$$F(2k) = F(k)(2F(k+1) - F(k))$$
$$F(2k+1) = F(k)^2 + F(k+1)^2$$

```c
ull fibonacci_fast_doubling(int n);  /* O(log n) time, O(log n) space recursive */
```

---

## 7. MULTIPLE APPROACHES

| Approach          | Time     | Space     | Overflow-safe | Notes                     |
|-------------------|----------|-----------|---------------|---------------------------|
| Naive Recursive   | O(2^n)   | O(n)      | Partial       | NEVER use in production   |
| Memoized          | O(n)     | O(n)      | YES (table)   | Top-down DP               |
| Iterative         | O(n)     | O(1)      | YES           | Best for embedded         |
| Two-variable      | O(n)     | O(1)      | YES           | Cleaner iterative         |
| LUT               | O(1)     | O(1)      | YES           | ROM-storable, max n=93    |
| Matrix Exp        | O(log n) | O(1)      | YES           | Best for very large n     |
| Fast Doubling     | O(log n) | O(log n)  | YES           | Elegant, recursive        |
| Binet's Formula   | O(1)     | O(1)      | Partial       | FP: valid only n < ~75    |

---

## 8. LOW-LEVEL C DISCUSSION

### Overflow Boundary
```c
/* F(46) = 1836311903   → last fitting in int32 */
/* F(93) = 12200160415121876738 → last fitting in uint64 */
/* F(94) wraps around in uint64 (UB-free since unsigned) */
_Static_assert(sizeof(unsigned long long) == 8, "Need 64-bit");
```

### Stack Overflow Risk
```c
/* Naive recursive: depth = n frames */
/* Each frame: ~40-64 bytes on x86-64 */
/* n=10000: ~640 KB stack needed */
/* Most embedded RTOS: 512 B - 4 KB stack → safe up to n~80 */
/* ALWAYS prefer iterative for embedded */
```

### Tail Call Optimization
```c
/* GCC -O2 may transform tail-recursive fib to loop */
/* But naive F(n) = F(n-1) + F(n-2) is NOT tail-recursive */
/* Only the accumulator version is tail-recursive */
```

### Matrix Multiplication Overflow
```c
/* For large n (e.g., n=93): matrix entries reach F(93) ~1.2e19 */
/* A.a[i][j] * B.a[k][l] could overflow uint64 for n > 93 */
/* For larger Fibonacci: use __uint128_t or modular arithmetic */
```

---

## 9. BITWISE INSIGHTS

### Fast Doubling Bit Traversal
```c
/* Fast doubling processes bits of n from MSB to LSB */
/* At each step: F(2k) and F(2k+1) from F(k) and F(k+1) */
/* Exactly ceil(log2(n)) iterations — same as matrix exponentiation */
```

### Matrix Exponentiation Pattern
```c
/* Binary exponentiation: p >>= 1 at each step */
/* Odd bit: result = mat_mul(result, M) */
/* Even bit: just square M */
/* Same pattern used in modular exponentiation (prime checking, crypto) */
```

### Golden Ratio in Binary
```c
/* phi = (1+sqrt(5))/2 ≈ 1.618 */
/* Binary: phi = 1.100111100011011...2 */
/* Irrational — cannot represent exactly in floating point */
/* This is why Binet's formula loses precision for large n */
```

---

## 10. MATHEMATICAL INSIGHTS

### Binet's Closed-Form Formula
$$F(n) = \frac{\phi^n - \psi^n}{\sqrt{5}}, \quad \phi = \frac{1+\sqrt{5}}{2}, \quad \psi = \frac{1-\sqrt{5}}{2}$$

Since `|ψ| < 1`, for large n: `ψ^n → 0`, so:
$$F(n) \approx \frac{\phi^n}{\sqrt{5}} \approx \text{round}\left(\frac{\phi^n}{\sqrt{5}}\right)$$

**Limitation:** double has 53-bit mantissa → exact only for F(n) < 2^53 ≈ F(77).

### Matrix Form
$$\begin{pmatrix} F(n+1) & F(n) \\ F(n) & F(n-1) \end{pmatrix} = \begin{pmatrix} 1 & 1 \\ 1 & 0 \end{pmatrix}^n$$

### Cassini's Identity
$$F(n-1) \times F(n+1) - F(n)^2 = (-1)^n$$

Useful for testing Fibonacci implementations.

### Sum of First n Fibonacci Numbers
$$\sum_{i=0}^{n} F(i) = F(n+2) - 1$$

---

## 11. INTERVIEWER FOLLOW-UP QUESTIONS

**Q1: What is the time complexity of naive recursive Fibonacci?**  
A: O(2^n) — exponential. Each call branches into two, many subproblems repeated.

**Q2: How to get O(n) with O(1) space?**  
A: Two-variable iterative: `prev, curr, next = curr, prev+curr`.

**Q3: Can you compute F(n) in O(log n)?**  
A: Yes — matrix exponentiation or fast doubling. Both are O(log n).

**Q4: What is the maximum n for uint64?**  
A: n = 93. F(93) = 12200160415121876738 < 2^64.

**Q5: How many digits does F(1000) have?**  
A: `log10(F(1000)) ≈ 1000 * log10(phi) ≈ 1000 * 0.209 ≈ 209 digits`.

**Q6: Fibonacci mod M for very large n?**  
A: Use matrix exponentiation with modular arithmetic. Each matrix element computed mod M.

**Q7: What is Pisano period?**  
A: Fibonacci numbers mod m repeat with period π(m). E.g., F(n) mod 10 repeats with period 60.

**Q8: Can Fibonacci be vectorized/parallelized?**  
A: The linear recurrence prevents simple parallelization. Matrix exponentiation can be parallelized at the matrix level.

---

## 12. COMMON MISTAKES

| Mistake | Fix |
|---------|-----|
| F(0) = 1 (wrong) | F(0) = 0, F(1) = 1 |
| Naive recursive for any real use | Always use iterative or memoized |
| `int` overflow at n=47 | Use `unsigned long long` |
| Binet's formula for n > 75 | Use iterative for exact results |
| Not handling n < 0 | Return 0 or error for negative n |
| Loop `for(i=1; i<n; ...)` off-by-one | Carefully verify loop bounds |

---

## 13. DEBUGGING GUIDE

```c
/* Print intermediate F values */
ull prev = 0, curr = 1;
printf("F(0)=0, F(1)=1\n");
for (int i = 2; i <= n; i++) {
    ull next = prev + curr;
    printf("F(%d) = F(%d)+F(%d) = %llu+%llu = %llu\n",
           i, i-1, i-2, curr, prev, next);
    prev = curr; curr = next;
}

/* Cassini's identity for validation */
/* F(n-1)*F(n+1) - F(n)^2 == (-1)^n */
for (int i = 1; i <= 20; i++) {
    long long cassini = (long long)(fib[i-1] * fib[i+1]) - (long long)(fib[i]*fib[i]);
    int expected = (i % 2 == 0) ? 1 : -1;
    assert(cassini == expected);
}
```

---

## 14. TEST CASES

```c
assert(fibonacci_iterative(0)  == 0ULL);
assert(fibonacci_iterative(1)  == 1ULL);
assert(fibonacci_iterative(2)  == 1ULL);
assert(fibonacci_iterative(10) == 55ULL);
assert(fibonacci_iterative(20) == 6765ULL);
assert(fibonacci_iterative(50) == 12586269025ULL);
assert(fibonacci_iterative(93) == 12200160415121876738ULL);

/* Matrix and fast doubling match iterative */
for (int n = 0; n <= 50; n++) {
    assert(fibonacci_matrix(n) == fibonacci_iterative(n));
    assert(fibonacci_fast_doubling(n) == fibonacci_iterative(n));
}
```

---

## 15. PERFORMANCE BENCHMARKING

| Approach    | n=10 | n=50 | n=93 | n=1000 | Notes             |
|-------------|------|------|------|--------|-------------------|
| Naive       | ~1 µs | SLOW | DON'T | — | Exponential       |
| Iterative   | ~10 ns | ~50 ns | ~90 ns | ~1 µs | Linear |
| LUT         | ~1 ns | ~1 ns | ~1 ns | N/A | O(1) max n=93 |
| Matrix Exp  | ~100 ns | ~100 ns | ~100 ns | ~200 ns | O(log n) |
| Fast Doubling| ~50 ns | ~50 ns | ~50 ns | ~100 ns | O(log n) |

For n ≤ 93: LUT always wins. For n > 93: matrix exp or fast doubling.

---

## 16. REAL-WORLD APPLICATIONS

| Domain       | Application                                           |
|--------------|-------------------------------------------------------|
| Nature       | Spiral patterns, phyllotaxis, rabbit population models |
| Algorithms   | Fibonacci heap (Dijkstra, Prim), search, scheduling   |
| Cryptography | Fibonacci-based pseudorandom generators               |
| Computer Architecture | Cache replacement analysis (working set size) |
| Compilers    | Fibonacci search (variant of binary search)           |
| DSP          | Fibonacci lattice filters                             |
| Embedded     | Timer period sequences, real-time task periods        |
| Graphics     | Golden ratio proportioning in UI layout               |

---

## 17. RELATED PROBLEMS

| Problem                     | Relation                          |
|-----------------------------|-----------------------------------|
| Tribonacci                  | F(n) = F(n-1)+F(n-2)+F(n-3)      |
| Climbing Stairs (LeetCode)  | 1s or 2s to reach n: Fibonacci!  |
| Nth Fibonacci mod M         | Pisano period                     |
| Count ways to tile 2×N      | Fibonacci recurrence              |
| Matrix Exponentiation       | O(log n) Fibonacci                |
| Staircase problem           | Fibonacci variant                 |
| Zeckendorf's Theorem        | Every integer = unique Fib sum    |

---

## 18. FINAL TAKEAWAYS

- F(0)=0, F(1)=1 — memorize this
- Naive recursive is O(2^n) — NEVER in production
- Two-variable iterative: O(n), O(1) — the workhorse approach
- LUT for n ≤ 93: O(1), embedded-friendly
- Matrix exponentiation: O(log n) — for very large n or mod M queries
- Binet's formula: mathematically elegant but FP-limited
- Overflow: uint64 is safe up to n=93
- **Mantra:** F(n) = prev + curr → slide window → no extra space

---

## 20. ADVANCED EXTENSIONS

### Fibonacci Mod M (No Overflow)
```c
ull fibonacci_mod(int n, ull M) {
    if (n == 0) return 0 % M;
    ull prev = 0, curr = 1 % M;
    for (int i = 2; i <= n; i++) {
        ull next = (prev + curr) % M;
        prev = curr;
        curr = next;
    }
    return curr;
}
```

### Generalized Fibonacci (Lucas Numbers)
```c
/* L(0)=2, L(1)=1, L(n)=L(n-1)+L(n-2) */
ull lucas(int n) {
    if (n == 0) return 2;
    if (n == 1) return 1;
    ull prev = 2, curr = 1;
    for (int i = 2; i <= n; i++) {
        ull next = prev + curr;
        prev = curr; curr = next;
    }
    return curr;
}
```

### Matrix Exponentiation for Modular Fibonacci
```c
/* Same matrix approach but all operations mod M */
/* Used when n is very large (e.g., n = 10^18) with M = 10^9+7 */
```

### SIMD Fibonacci (Vectorization concept)
```c
/* Fibonacci has inherent data dependency: each term depends on prior two */
/* Not directly SIMD-friendly */
/* But: matrix exponentiation matrices can be processed in parallel */
/* Alternate: compute multiple independent Fibonacci sequences in parallel */
```
