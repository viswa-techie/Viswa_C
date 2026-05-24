# 04 — Armstrong Number (Narcissistic Number)

**Category:** Foundation > A. Basic Number Problems  
**Difficulty:** Easy  
**Priority:** TIER-2  
**Companies:** Qualcomm, NXP, Texas Instruments  

---

## 1. PROBLEM UNDERSTANDING

### Simple English
An **Armstrong number** (also called a narcissistic number) is a number that equals the sum of its own digits each raised to the power of the number of digits.

**Examples:**
- 153 = 1³ + 5³ + 3³ = 1 + 125 + 27 = 153 ✓
- 370 = 3³ + 7³ + 0³ = 27 + 343 + 0 = 370 ✓
- 1634 = 1⁴ + 6⁴ + 3⁴ + 4⁴ = 1 + 1296 + 81 + 256 = 1634 ✓

### Mathematical Definition
Let `n` be a number with `k` digits `d_{k-1}, d_{k-2}, ..., d_1, d_0`.  
n is Armstrong iff:
$$n = \sum_{i=0}^{k-1} d_i^k$$

### Visual Explanation
```
n = 153
Digits: [1][5][3]   k = 3

   1^3 = 1
   5^3 = 125
   3^3 = 27
   ─────────
   Sum = 153 = n  → ARMSTRONG ✓

n = 154
   1^3 = 1
   5^3 = 125
   4^3 = 64
   ─────────
   Sum = 190 ≠ 154  → NOT ARMSTRONG ✗
```

### What Interviewers Are Testing
- Digit extraction (same pattern as reverse/palindrome)
- Power computation: integer vs floating-point accuracy
- Understanding that the exponent = number of digits
- Range awareness: only finitely many Armstrong numbers exist

### Common Traps
- Using `pow()` from `<math.h>`: floating-point rounding errors for large exponents
- Forgetting single-digit numbers are all Armstrong (0–9: each equals `d^1 = d`)
- Off-by-one in digit count
- Negative numbers: not Armstrong by convention

### Fun Fact
There are exactly **88 narcissistic numbers** in base 10.  
The largest is a 39-digit number.

---

## 2. INPUT/OUTPUT ANALYSIS

| Input | Output | Notes                         |
|-------|--------|-------------------------------|
| 0–9   | YES    | d^1 = d for any single digit  |
| 153   | YES    | Classic example               |
| 370   | YES    | 3³+7³+0³                      |
| 371   | YES    | 3³+7³+1³                      |
| 407   | YES    | 4³+0³+7³                      |
| 1634  | YES    | 4-digit Armstrong             |
| 8208  | YES    | 4-digit Armstrong             |
| 9474  | YES    | 4-digit Armstrong             |
| 100   | NO     | 1²+0²+0² = 1 ≠ 100            |
| 152   | NO     | 1³+5³+2³ = 134 ≠ 152          |
| -153  | NO     | Negative: not Armstrong       |

---

## 3. EDGE CASES

| Case          | Value | Expected | Reason                      |
|---------------|-------|----------|-----------------------------|
| All single-digit | 0–9 | YES   | d^1 = d                     |
| Two-digit     | 10–99 | NO (except none) | No 2-digit Armstrong in base 10 |
| Zero          | 0    | YES      | 0^1 = 0                     |
| Negative      | -153 | NO       | Not defined                 |
| Large         | 9474 | YES      | 4-digit Armstrong           |
| Just missed   | 152  | NO       | Sum = 134                   |

---

## 4. BRUTE FORCE — Using pow()

```c
#include <math.h>

bool is_armstrong_pow(int n) {
    if (n < 0) return false;
    int k = count_digits(n);
    int temp = n;
    long long sum = 0;
    while (temp > 0) {
        int d = temp % 10;
        sum += (long long)round(pow((double)d, k));
        temp /= 10;
    }
    return sum == (long long)n;
}
```
**Warning:** `pow(5, 3)` may return `124.99999...` due to FP — always use `round()`.

---

## 5. BETTER SOLUTION — Integer Power

```c
/* ipow: integer exponentiation using fast doubling */
static long long ipow(long long base, int exp) {
    long long result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

bool is_armstrong_ipow(int n) {
    if (n < 0) return false;
    if (n < 10) return true;
    int k = count_digits(n);
    int temp = n;
    long long sum = 0;
    while (temp > 0) {
        sum += ipow((long long)(temp % 10), k);
        temp /= 10;
    }
    return sum == (long long)n;
}
```

- No floating-point → no rounding errors
- `ipow` uses fast exponentiation: O(log k) multiplications per digit

---

## 6. OPTIMAL SOLUTION

The integer-power approach is optimal for a single query.  
For range queries, use the precomputed table (exact 88 Armstrong numbers).

**Time:** O(d × log k) where d = digits of n, k = digit count  
Since d = k = O(log n): **O(log n × log log n)**  
**Space:** O(1)

---

## 7. MULTIPLE APPROACHES

### Approach 1: pow() from math.h
- Use `round()` to fix FP errors
- Cons: floating-point dependency, library call overhead

### Approach 2: Integer ipow [PREFERRED]
- Exact, no FP, O(log k) per digit
- Cons: slightly more code

### Approach 3: Digit Array + ipow
- Store digits first, then compute sum
- Pros: clear separation of concerns

### Approach 4: Recursive
```c
bool is_armstrong_recursive(int n) {
    if (n < 0) return false;
    int k = count_digits(n);
    return armstrong_rec_sum(n, k) == (long long)n;
}
static long long armstrong_rec_sum(int n, int k) {
    if (n == 0) return 0;
    return ipow(n % 10, k) + armstrong_rec_sum(n / 10, k);
}
```

### Approach 5: Precomputed Table (O(1))
```c
static const int armstrong_nums[] = {
    0,1,2,3,4,5,6,7,8,9,
    153,370,371,407,
    1634,8208,9474,
    54748,92727,93084,
    548834, ...
};
```
- O(1) per query, no computation
- Ideal for embedded ROM storage

---

## 8. LOW-LEVEL C DISCUSSION

### Floating-Point Pitfall
```c
pow(5.0, 3) might return 124.99999... in some implementations
(long long)pow(5.0, 3) == 124  /* WRONG */
(long long)round(pow(5.0, 3)) == 125  /* CORRECT */
```
**Rule:** Never cast `pow()` result directly to integer for equality checks.

### Integer Power Implementation
Fast exponentiation uses O(log exp) multiplications vs O(exp) naive:
```c
/* Naive: 9^9 = 9 multiplications */
/* Fast:  9^9 in log2(9)≈3 doublings + 3 multiplies = ~6 ops */
```

### Overflow Analysis
- Maximum digit: 9, maximum common k: 10 (for 10-digit int)
- 9^10 = 3,486,784,401 > INT_MAX (2,147,483,647)
- Must use `long long` for sum: `long long sum = 0`

---

## 9. BITWISE INSIGHTS

### Fast Odd Check in ipow
```c
if (exp & 1) result *= base;  /* check LSB instead of exp % 2 */
exp >>= 1;                    /* right shift instead of exp / 2 */
```

### Digit Power Precompute
For fixed k, digit powers are constant:
```c
/* Precompute d^k for d in [0,9] once */
long long pow_cache[10];
for (int d = 0; d <= 9; d++) pow_cache[d] = ipow(d, k);
/* Then sum = pow_cache[digit] for each digit */
```

---

## 10. MATHEMATICAL INSIGHTS

### Finite Armstrong Numbers
In base 10:
- 1-digit: 9 (trivially 0–9)
- 2-digit: 0 (none exist)
- 3-digit: 4 (153, 370, 371, 407)
- 4-digit: 3 (1634, 8208, 9474)
- Total in all bases up to some limit: 88 numbers

**Why finite?** For very large k, `9^k` grows much faster than any k-digit number.  
For k > 60 digits, no Armstrong numbers exist in base 10.

### Upper Bound for k-Digit Armstrong Numbers
The maximum sum of k digits each raised to power k:  
`9^k * k` (max digit is 9, k digits)

For k = 10: 9^10 * 10 ≈ 34 billion > 10^10 (max 10-digit number)  
For k = 60: 9^60 * 60 << 10^60 → impossible

---

## 11. INTERVIEWER FOLLOW-UP QUESTIONS

**Q1: Why can't you use pow() directly?**  
A: Floating-point imprecision. `pow(5,3)` may return `124.999` → incorrect. Use integer ipow or `round()`.

**Q2: List all Armstrong numbers up to 10000.**  
A: 0–9, 153, 370, 371, 407, 1634, 8208, 9474.

**Q3: Are there infinitely many Armstrong numbers?**  
A: No — finitely many in any fixed base.

**Q4: How do you find the digit count?**  
A: Loop dividing by 10, or `(int)log10(n) + 1` (avoid FP for exact result; use loop).

**Q5: What is the time complexity?**  
A: O(d × log d) where d is the number of digits. Since d = O(log n), this is O(log n × log log n).

**Q6: Extend to any base b.**  
A: Replace `% 10` with `% b` and `/ 10` with `/ b`. Count digits in base b: `floor(log_b(n)) + 1`.

**Q7: Memory-constrained embedded device — which approach?**  
A: Precomputed table in ROM. Zero runtime computation. Perfect for MCU.

---

## 12. COMMON MISTAKES

| Mistake | Fix |
|---------|-----|
| `pow()` without `round()` | Always use `(long long)round(pow(d,k))` |
| Single digit not handled | All 0–9 are Armstrong — add early return |
| `int sum` overflow | Use `long long sum` |
| Exponent = n, not digit count k | Exponent must equal number of digits, not value |
| Negative n considered | Return false for n < 0 |

---

## 13. DEBUGGING GUIDE

```c
/* Print each digit's contribution */
while (n > 0) {
    int d = n % 10;
    long long p = ipow(d, k);
    printf("d=%d, d^%d=%lld, running_sum=%lld\n", d, k, p, sum+=p);
    n /= 10;
}
```

---

## 14. TEST CASES

```c
/* All Armstrong 3-digit */
assert(is_armstrong_ipow(153) == true);
assert(is_armstrong_ipow(370) == true);
assert(is_armstrong_ipow(371) == true);
assert(is_armstrong_ipow(407) == true);

/* All Armstrong 4-digit */
assert(is_armstrong_ipow(1634) == true);
assert(is_armstrong_ipow(8208) == true);
assert(is_armstrong_ipow(9474) == true);

/* Non-Armstrong */
assert(is_armstrong_ipow(100)  == false);
assert(is_armstrong_ipow(1000) == false);

/* Single digits — all true */
for (int i = 0; i <= 9; i++) assert(is_armstrong_ipow(i) == true);

/* Negatives */
assert(is_armstrong_ipow(-153) == false);
```

---

## 15. PERFORMANCE

For 32-bit int (max 10 digits):
- At most 10 digit extractions
- Each ipow call: O(log 10) ≈ 3–4 multiplications
- Total: ~40 integer operations — extremely fast

For range queries up to N:
- Brute force O(N × log N × log log N)
- Precomputed table: O(1) per query (88 entries total)

---

## 16. REAL-WORLD APPLICATIONS

| Domain        | Application                                        |
|---------------|----------------------------------------------------|
| Cryptography  | Digital root properties in hash functions          |
| Number theory | Base-n narcissistic numbers — theoretical study    |
| Puzzles/Games | Programming puzzles and coding competitions        |
| Education     | Teaching digit manipulation and power computation  |
| Embedded      | Fixed table lookup for resource-constrained check  |

---

## 17. RELATED PROBLEMS

| Problem            | Relation                          |
|--------------------|-----------------------------------|
| Happy Number       | Digit-square sum loop             |
| Digit Sum          | Same digit extraction loop        |
| Disarium Number    | Digit raised to positional power  |
| Perfect Number     | Sum of divisors = number          |
| Kaprekar Number    | Digit manipulation variant        |

---

## 18. FINAL TAKEAWAYS

- Exponent = **number of digits** (not the number itself)
- All single-digit numbers (0–9) are Armstrong
- Use integer `ipow`, not `pow()` from math.h
- Use `long long` for the sum (9^10 exceeds INT_MAX)
- Only 88 Armstrong numbers exist in base 10 — consider LUT for production
- **Interview mantra:** count digits k → extract each digit d → compute d^k → sum → compare

---

## 20. ADVANCED EXTENSIONS

### Armstrong in Any Base
```c
bool is_armstrong_base(int n, int base) {
    if (n < 0) return false;
    int k = 0, temp = n;
    while (temp > 0) { k++; temp /= base; }
    temp = n;
    long long sum = 0;
    while (temp > 0) {
        sum += ipow((long long)(temp % base), k);
        temp /= base;
    }
    return sum == (long long)n;
}
```

### Embedded Optimized (Table in Flash/ROM)
```c
/* Store in read-only section — zero RAM usage */
static const int arm_nums[] __attribute__((section(".rodata"))) = {
    0,1,2,3,4,5,6,7,8,9,153,370,371,407,1634,8208,9474
};
```

### Vectorized Digit Power Sum (SIMD Concept)
```c
/* For very large number of queries:
 * Precompute digit_power[10][k] table once,
 * then each digit lookup is O(1).
 * Sum over 10-entry table: 10 additions total.
 * Can be vectorized with SSE/AVX if processing many numbers in batch. */
```
