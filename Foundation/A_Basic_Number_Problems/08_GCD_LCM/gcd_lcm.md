# 08 — GCD / LCM

**Category:** Foundation > A. Basic Number Problems  
**Difficulty:** Easy → Medium (Extended GCD, Modular Inverse)  
**Priority:** TIER-1 (Most Important)  
**Companies:** Qualcomm, Intel, AMD, NXP, Nvidia, MediaTek  

---

## 1. PROBLEM UNDERSTANDING

### Simple English
**GCD (Greatest Common Divisor):** The largest integer that divides both a and b without remainder.  
**LCM (Least Common Multiple):** The smallest positive integer divisible by both a and b.

### Mathematical Definition
$$\gcd(a, b) = \max\{d \in \mathbb{Z}^+ : d \mid a \text{ and } d \mid b\}$$

$$\text{lcm}(a, b) = \frac{|a \cdot b|}{\gcd(a, b)}$$

**Key Identity:** $\gcd(a, b) \times \text{lcm}(a, b) = |a \times b|$

### Visual Explanation
```
a = 12, b = 8
Divisors of 12: {1, 2, 3, 4, 6, 12}
Divisors of  8: {1, 2, 4, 8}
Common:         {1, 2, 4}
GCD = max{1,2,4} = 4

LCM = 12*8/4 = 24

Verify: multiples of 24 are divisible by both 12 and 8 ✓
```

### Euclidean Algorithm Visual
```
gcd(48, 18):
  gcd(48, 18) → gcd(18, 48%18=12) → gcd(12, 18%12=6) → gcd(6, 12%6=0) → 6
```

### What Interviewers Are Testing
- Knowledge of Euclidean algorithm
- Overflow-safe LCM: `(a / gcd) * b` not `a * b / gcd`
- Extended GCD for modular inverse
- Binary GCD for hardware without division
- GCD of multiple numbers via fold

### Common Traps
- `a * b / gcd(a, b)` may overflow for large a, b
- `gcd(0, 0)` = 0 by convention
- GCD is defined for non-negative integers; handle negatives with abs()
- LCM of large numbers may require `long long`

---

## 2. INPUT/OUTPUT ANALYSIS

| a   | b   | GCD | LCM  | Notes                         |
|-----|-----|-----|------|-------------------------------|
| 12  | 8   | 4   | 24   | Standard example              |
| 7   | 5   | 1   | 35   | Coprime                       |
| 0   | 5   | 5   | 0    | gcd(0,n) = n; lcm(0,n) = 0   |
| 0   | 0   | 0   | 0    | Convention                    |
| 12  | 12  | 12  | 12   | Equal numbers                 |
| -12 | 8   | 4   | 24   | Negative: use abs              |
| 1000000 | 999999 | 1 | 999999000000 | Coprime large numbers |

---

## 3. EDGE CASES

| Case           | a      | b     | GCD  | Notes                     |
|----------------|--------|-------|------|---------------------------|
| Zero input     | 0      | 5     | 5    | gcd(0,b) = b              |
| Both zero      | 0      | 0     | 0    | Convention: 0             |
| Equal          | n      | n     | n    | gcd(n,n) = n              |
| Coprime        | 7      | 13    | 1    | No common factor > 1      |
| One is 1       | 1      | n     | 1    | gcd(1,n) = 1 always       |
| Power of 2     | 16     | 8     | 8    | Binary GCD shines here    |
| Large LCM      | 999983 | 999979| 1    | LCM = a*b (coprime primes)|

---

## 4. BRUTE FORCE — Check All Divisors

```c
int gcd_brute(int a, int b) {
    a = abs(a); b = abs(b);
    int g = 1;
    for (int i = 1; i <= min(a,b); i++)
        if (a % i == 0 && b % i == 0) g = i;
    return g;
}
```
**Time:** O(min(a,b)) — for each candidate divisor  
**Space:** O(1)  
**Inefficient:** For a=10^9, this runs 10^9 iterations.

---

## 5. BETTER SOLUTION — Euclidean Algorithm

### Key Property
$$\gcd(a, b) = \gcd(b, a \bmod b)$$

**Proof:** Any divisor d of both a and b must also divide `a mod b = a - floor(a/b)*b`.  
So common divisors of {a, b} are the same as common divisors of {b, a mod b}.  
The GCD is the same.

### Dry Run: gcd(48, 18)
```
gcd(48, 18) → gcd(18, 48%18=12) → gcd(12, 6) → gcd(6, 0) → 6
Step count: 4 steps for inputs up to 48
```

### Iterative Implementation [PREFERRED]
```c
int gcd_euclidean(int a, int b) {
    a = abs(a); b = abs(b);
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}
```

**Time:** O(log(min(a, b))) — provably, the number of steps is bounded by 5× digits of min(a,b).  
**Space:** O(1)

---

## 6. OPTIMAL SOLUTION

### Iterative Euclidean [O(log n), O(1) space] — Standard Answer

```c
int gcd(int a, int b) {
    a = abs(a); b = abs(b);
    while (b) { int t = b; b = a % b; a = t; }
    return a;
}

long long lcm(int a, int b) {
    if (!a || !b) return 0;
    int g = gcd(a, b);
    return (long long)(a / g) * b;   /* divide first to prevent overflow */
}
```

### Binary GCD / Stein's Algorithm [Embedded-Optimal]
When hardware lacks a division instruction (common in microcontrollers):  
Uses only: subtraction, bit shifts, comparison — no `%` operator.

```c
ull gcd_binary(ull a, ull b) {
    if (!a) return b; if (!b) return a;
    int shift = __builtin_ctzll(a | b);
    a >>= __builtin_ctzll(a);
    do {
        b >>= __builtin_ctzll(b);
        if (a > b) { ull t=a; a=b; b=t; }
        b -= a;
    } while (b);
    return a << shift;
}
```

---

## 7. MULTIPLE APPROACHES

### Approach 1: Brute Force O(min(a,b))
- Simple but impractical for large inputs

### Approach 2: Euclidean (Iterative) O(log n) [PREFERRED]
- Industry standard, O(1) space

### Approach 3: Euclidean (Recursive) O(log n)
- Elegant but uses O(log n) stack
- Risk: stack overflow if n is very large (rare for GCD)

### Approach 4: Subtraction-Based O(max(a,b))
- Oldest form, extremely slow
- Educational: shows the fundamental GCD property

### Approach 5: Binary GCD (Stein's) O(log² n)
- Division-free — ideal for embedded MCU without divider
- Uses `__builtin_ctzll` (count trailing zeros)

### Approach 6: Extended Euclidean
- Finds x, y such that `ax + by = gcd(a,b)` (Bezout's identity)
- Essential for: modular inverse, Chinese Remainder Theorem

---

## 8. LOW-LEVEL C DISCUSSION

### LCM Overflow Prevention
```c
/* WRONG: a * b may overflow long long for large a, b */
long long lcm_wrong = (long long)a * b / gcd(a, b);

/* CORRECT: divide a by gcd first (a/gcd is always integer) */
long long lcm_correct = (long long)(a / gcd(a, b)) * b;
```

### Division vs Modulo Cost
```c
/* % and / are the same instruction on x86: idiv */
/* Both computed together: a/b and a%b in one operation */
/* Binary GCD avoids them entirely — useful on ARM Cortex-M without HW divider */
```

### `__builtin_ctzll` on GCC/Clang
```c
/* Counts trailing zeros (number of low-order 0 bits) */
/* Maps to BSF instruction on x86, CLZ on ARM */
/* __builtin_ctzll(0) is UB — always check for 0 first */
int trailing_zeros = __builtin_ctzll(a);  /* a must be != 0 */
```

### Signed Integer Handling
```c
/* GCD is defined for non-negative integers */
/* For negative inputs: use abs() or manual negate */
/* abs(INT_MIN) is UB — use (a < 0) ? -(unsigned)a : (unsigned)a */
```

---

## 9. BITWISE INSIGHTS

### Binary GCD Key Operations
```c
/* n >> k  : divide n by 2^k (right shift) */
/* n &= ~0x1: make n odd (clear LSB — but use ctzll for all trailing zeros) */
/* a ^ b   : can detect sign difference (not used here, but useful elsewhere) */
```

### GCD of Power-of-2 Numbers
```c
/* gcd(2^a, 2^b) = 2^min(a,b) */
/* Detected instantly by binary GCD with shift count */
/* Binary GCD is O(1) for power-of-2 inputs! */
```

---

## 10. MATHEMATICAL INSIGHTS

### Euclidean Algorithm Convergence (Lame's Theorem)
The number of steps in Euclidean GCD is at most `5 × (number of digits in min(a,b))`.  
Equivalently: for a ≥ b, the number of divisions is at most `O(log_phi(b))` where phi is the golden ratio.

**Worst case:** consecutive Fibonacci numbers. `gcd(F(n+1), F(n))` takes exactly n steps.

### Bezout's Identity
$$\forall a, b \in \mathbb{Z}, \exists x, y \in \mathbb{Z}: ax + by = \gcd(a,b)$$

Proof: Extended Euclidean algorithm constructs x and y.

### GCD Properties
- `gcd(a, 0) = a`
- `gcd(a, b) = gcd(b, a)` (commutative)
- `gcd(a, b) = gcd(a-b, b)` for a > b (subtraction property)
- `gcd(a, b) = gcd(a mod b, b)` (Euclidean property)
- `gcd(na, nb) = n * gcd(a, b)` (scaling)

### LCM and GCD Relationship
$$\text{lcm}(a,b) = \frac{a \cdot b}{\gcd(a,b)}$$

Proof: Prime factorization of LCM takes max exponent, GCD takes min exponent. Max + Min = Sum = product of exponents.

---

## 11. INTERVIEWER FOLLOW-UP QUESTIONS

**Q1: What is the time complexity of Euclidean GCD?**  
A: O(log(min(a, b))). Lame's theorem: at most 5× number of digits in min(a, b) steps.

**Q2: What is the worst-case input for Euclidean GCD?**  
A: Consecutive Fibonacci numbers: gcd(F(n+1), F(n)). Each step reduces to the previous Fibonacci pair.

**Q3: How to compute GCD without modulo/division?**  
A: Binary GCD (Stein's algorithm) using only subtraction and bit shifts.

**Q4: What is LCM of n numbers?**  
A: `lcm(a1, a2, ..., an) = lcm(lcm(a1, a2), a3, ...)`. Fold using pairwise LCM.

**Q5: Compute LCM without overflow for large numbers?**  
A: Use `(long long)(a / gcd(a,b)) * b` — division before multiplication.

**Q6: What is modular inverse and how to compute it?**  
A: The modular inverse of a mod m is x such that `ax ≡ 1 (mod m)`.  
Exists iff gcd(a, m) = 1. Computed via Extended Euclidean Algorithm.

**Q7: GCD of 0 and 0?**  
A: By convention, gcd(0, 0) = 0.

**Q8: GCD in cryptography?**  
A: RSA key generation: gcd(e, phi(n)) = 1 ensures e is coprime to the totient.  
Diffie-Hellman: GCD used to verify coprimality of parameters.

---

## 12. COMMON MISTAKES

| Mistake | Fix |
|---------|-----|
| `a * b / gcd(a,b)` overflow | Use `(a/gcd) * b` instead |
| Forgetting `gcd(0, n) = n` | Guard: `if (b == 0) return a` |
| Not handling negative inputs | Use `a = abs(a); b = abs(b)` |
| `gcd_recursive` without abs | Negative modulo produces wrong result |
| Dividing by 0 in LCM | Check `if (a == 0 || b == 0) return 0` |

---

## 13. DEBUGGING GUIDE

```c
/* Trace Euclidean steps */
while (b != 0) {
    printf("gcd(%d, %d)\n", a, b);
    int temp = b;
    b = a % b;
    a = temp;
}
printf("= %d\n", a);
```

---

## 14. TEST CASES

```c
assert(gcd_euclidean(12, 8)   == 4);
assert(gcd_euclidean(7, 5)    == 1);    /* coprime */
assert(gcd_euclidean(0, 5)    == 5);
assert(gcd_euclidean(0, 0)    == 0);
assert(gcd_euclidean(-12, 8)  == 4);   /* negative */
assert(gcd_euclidean(12, 12)  == 12);  /* equal */
assert(lcm(4, 6)   == 12);
assert(lcm(3, 5)   == 15);
assert(lcm(0, 5)   == 0);

/* LCM * GCD = a * b */
for (int a = 1; a <= 50; a++)
    for (int b = 1; b <= 50; b++)
        assert(lcm(a, b) * gcd_euclidean(a, b) == (long long)a * b);
```

---

## 15. PERFORMANCE

| Approach     | a=1000, b=999 | a=10^9, b=10^9-1 | Notes                  |
|--------------|---------------|------------------|------------------------|
| Brute Force  | ~999 ops      | ~10^9 ops        | Impractical            |
| Euclidean    | ~7 ops        | ~30 ops          | log(min) steps         |
| Binary GCD   | ~20 ops       | ~60 ops          | No division; MCU-safe  |
| Subtraction  | ~1000 ops     | ~2×10^9 ops      | Never use              |

For most applications: **Euclidean iterative** is the fastest on modern CPUs (hardware divider present).  
On MCUs without HW divider: **Binary GCD** wins.

---

## 16. REAL-WORLD APPLICATIONS

| Domain        | Application                                                  |
|---------------|--------------------------------------------------------------|
| Cryptography  | RSA: gcd(e, phi(n)) = 1 check; modular inverse computation   |
| Networking    | Packet size alignment: GCD of block sizes                    |
| Embedded      | Timer period LCM for synchronized task scheduling           |
| Signal Processing | Sampling rate conversion: GCD of sample rates            |
| Compilers     | Loop unrolling factor: GCD of loop trip counts               |
| OS Scheduling | Finding common task periods for hyperperiod computation      |
| Mathematics   | Fraction simplification: numerator/gcd, denominator/gcd      |
| Graphics      | Pixel ratio normalization (e.g., 1920:1080 → 16:9 via GCD)  |

---

## 17. RELATED PROBLEMS

| Problem                     | Relation                               |
|-----------------------------|----------------------------------------|
| Modular Inverse             | Extended Euclidean                     |
| Chinese Remainder Theorem   | Uses Extended Euclidean                |
| Coprime pairs count         | Euler's Totient function               |
| LCM of range [1, N]        | Prime factorization based              |
| Simplify fraction a/b       | Divide numerator and denominator by GCD|
| Sync two periodic timers    | LCM of periods = hyperperiod           |
| Bezout's coefficients       | Extended Euclidean                     |

---

## 18. FINAL TAKEAWAYS

- Euclidean: `while(b) { t=b; b=a%b; a=t; }` — 10 chars of logic
- LCM: `(a/gcd(a,b)) * b` — divide FIRST to prevent overflow
- gcd(0, n) = n, gcd(0, 0) = 0
- Extended GCD → Modular Inverse → RSA
- Binary GCD for embedded without hardware division
- Worst case: consecutive Fibonacci numbers
- **Mantra:** `a, b → b, a%b → repeat → a is GCD when b=0`

---

## 20. ADVANCED EXTENSIONS

### GCD of Fibonacci Pair (Mathematical Trick)
```c
/* gcd(F(m), F(n)) = F(gcd(m, n)) */
/* Elegant number theory property */
```

### Extended GCD — Iterative (No Recursion)
```c
int gcd_extended_iter(int a, int b, int *x, int *y) {
    int old_r=a, r=b, old_s=1, s=0, old_t=0, t=1;
    while (r != 0) {
        int q = old_r / r;
        int tmp;
        tmp=r;    r=old_r - q*r;       old_r=tmp;
        tmp=s;    s=old_s - q*s;       old_s=tmp;
        tmp=t;    t=old_t - q*t;       old_t=tmp;
    }
    *x = old_s; *y = old_t;
    return old_r;
}
```

### Multi-precision GCD (Big Numbers)
```c
/* Lehmer's algorithm: O(n^2) for n-digit numbers */
/* Used in cryptographic libraries like GMP */
/* Combines Euclidean steps at the word level */
```

### Embedded Timer Synchronization
```c
/* Two timers: T1 fires every t1 ms, T2 every t2 ms */
/* Hyperperiod (first sync point) = LCM(t1, t2) */
/* If LCM is too large → adjust t1 or t2 */
int hyperperiod = lcm(t1, t2);
```
