# 07 — Digit Sum & Digital Root

**Category:** Foundation > A. Basic Number Problems  
**Difficulty:** Easy → Medium (digital root O(1) insight)  
**Priority:** TIER-2  
**Companies:** Qualcomm, NXP, Texas Instruments, Intel  

---

## 1. PROBLEM UNDERSTANDING

### Simple English
**Digit Sum:** Add all the individual digits of a number.  
Example: `1234` → `1 + 2 + 3 + 4 = 10`

**Digital Root:** Repeat digit sum until you get a single digit.  
Example: `9999` → `36` → `9`

### Mathematical Definition
$$\text{digitsum}(n) = \sum_{i} d_i \quad \text{where } d_i \text{ are the decimal digits of } n$$

**Digital Root (Additive Persistence):**
$$\text{dr}(n) = \begin{cases} 0 & n = 0 \\ 9 & n > 0 \text{ and } n \equiv 0 \pmod{9} \\ n \bmod 9 & \text{otherwise} \end{cases}$$

Or equivalently: `dr(n) = 1 + (n - 1) % 9` for n > 0.

### Visual Explanation
```
n = 9875
Digits:  9  8  7  5
Sum:     9+8+7+5 = 29  (digit sum)
→ 2+9 = 11
→ 1+1 = 2            (digital root = 2)

Formula: 9875 % 9 = 2  ✓
```

### What Interviewers Are Testing
- Basic digit extraction loop
- The O(1) digital root trick using modulo 9
- Mathematical pattern recognition
- Divisibility rule for 9 (digit sum divisible by 9 ↔ n divisible by 9)

### Common Traps
- `n = 0` → digit sum = 0 (not undefined)
- Negative numbers → take absolute value first
- Digital root of 9 is 9, not 0 (`9 % 9 == 0` but dr(9) = 9)
- Formula: `dr(n) = 1 + (n-1) % 9` handles the 9 case correctly

---

## 2. INPUT/OUTPUT ANALYSIS

| n    | Digit Sum | Digital Root | Notes                   |
|------|-----------|--------------|-------------------------|
| 0    | 0         | 0            | Special case            |
| 1    | 1         | 1            | Single digit            |
| 9    | 9         | 9            | Digital root = 9        |
| 10   | 1         | 1            | 1+0=1                   |
| 18   | 9         | 9            | 1+8=9                   |
| 19   | 10        | 1            | 1+9=10 → 1+0=1          |
| 99   | 18        | 9            | 9+9=18 → 1+8=9          |
| 123  | 6         | 6            | 1+2+3=6                 |
| 9999 | 36        | 9            | 9+9+9+9=36 → 3+6=9      |
| -45  | 9         | 9            | abs(-45)=45 → 4+5=9     |

---

## 3. EDGE CASES

| Case        | Value  | Expected | Reason                  |
|-------------|--------|----------|-------------------------|
| Zero        | 0      | 0        | Empty sum = 0           |
| Single digit| 5      | 5        | Already minimal         |
| Multiple of 9| 18    | 9        | Digital root is always 9 |
| Negative    | -123   | 6        | Use absolute value      |
| INT_MAX     | 2147483647 | 46   | 2+1+4+7+4+8+3+6+4+7=46 |

---

## 4. BRUTE FORCE — Iterative Loop

```c
int digit_sum_iterative(int n) {
    if (n < 0) n = -n;
    int sum = 0;
    while (n > 0) {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}
```
**Time:** O(log n) — one iteration per digit  
**Space:** O(1)

---

## 5. BETTER — Recursive

```c
int digit_sum_recursive(int n) {
    if (n < 0) n = -n;
    if (n < 10) return n;
    return n % 10 + digit_sum_recursive(n / 10);
}
```
Elegant but uses O(log n) stack space.

---

## 6. OPTIMAL — Digital Root O(1)

### The Key Mathematical Insight
`n ≡ digit_sum(n) (mod 9)` — a number and its digit sum have the same remainder when divided by 9.

This means: **repeated digit sum = n mod 9**, except when the result is 0 (and n > 0) it equals 9.

```c
int digital_root_formula(int n) {
    if (n < 0) n = -n;
    if (n == 0) return 0;
    int r = n % 9;
    return (r == 0) ? 9 : r;
}
```

**Or more elegantly:**
```c
int digital_root(int n) {
    if (n <= 0) return (n == 0) ? 0 : digital_root(-n);
    return 1 + (n - 1) % 9;
}
```

**Time:** O(1)  
**Space:** O(1)

---

## 7. MULTIPLE APPROACHES

### Approach 1: Iterative [PREFERRED for single digit sum]
- O(log n) time, O(1) space
- Simple, portable, no library

### Approach 2: Recursive
- O(log n) time, O(log n) space
- More elegant, readable
- Avoid for deep recursion

### Approach 3: String-Based
```c
/* Loop over string chars, subtract '0' */
```
- O(log n) time, O(log n) space (string buffer)

### Approach 4: Digital Root Formula [OPTIMAL for digital root]
```c
int dr = (n == 0) ? 0 : (1 + (n - 1) % 9);
```
- O(1) — single modulo operation
- Only works for digital root, not for intermediate digit sum

### Approach 5: Digit Product (Bonus Variant)
```c
int digit_product(int n) {
    if (n == 0) return 0;
    int prod = 1;
    while (n > 0) {
        int d = n % 10;
        if (d == 0) return 0;
        prod *= d;
        n /= 10;
    }
    return prod;
}
```

### Approach 6: Sum of Digits of All Numbers 1..N
```c
/* Digit-by-digit mathematical formula — O(log n) */
long long sum_of_digits_1_to_n(long long n);
```
- Key interview variant
- Uses positional counting at each digit place

---

## 8. LOW-LEVEL C DISCUSSION

### Divisibility Rules via Digit Sum
- **Rule of 9:** `n % 9 == digit_sum(n) % 9`
- **Rule of 3:** `n % 3 == digit_sum(n) % 3`
- **Rule of 11:** alternating digit sum (signed)

### Negative Modulo
```c
/* In C: -45 % 10 == -5 (C99: truncation toward zero) */
/* So: if n < 0, negate first */
if (n < 0) n = -n;
```

### INT_MIN Edge Case
```c
/* -n where n = INT_MIN = -2147483648 is UB (overflow) */
/* Safe: cast to long long: (long long)(-n) or use unsigned */
if (n < 0) {
    unsigned int abs_n = (n == INT_MIN) ? (unsigned int)INT_MAX + 1u : (unsigned int)(-n);
    /* proceed with abs_n */
}
```

---

## 9. BITWISE INSIGHTS

### Why Digital Root Works (mod 9)
In decimal: `10 ≡ 1 (mod 9)`, so `10^k ≡ 1^k = 1 (mod 9)`.  
Therefore: `sum(d_i * 10^i) ≡ sum(d_i * 1) = sum(d_i) (mod 9)`.  
So n and its digit sum always have the same mod-9 residue.

### Checking Divisibility by 9
```c
bool divisible_by_9(int n) {
    return n % 9 == 0;
    /* Equivalently: digit_sum(n) % 9 == 0 */
    /* Both are exact — no approximation */
}
```

---

## 10. MATHEMATICAL INSIGHTS

### Digital Root Formula Proof
For n > 0:  
- `n = 9q + r` where r = n % 9  
- Digit sum is also `≡ r (mod 9)` (by the base-10 argument above)
- If r ≠ 0: digital root = r
- If r = 0 and n > 0: digital root = 9 (not 0, since 9 is a factor)

`1 + (n-1) % 9` implements this without the conditional:
- n=9: `1 + 8%9 = 1+8 = 9` ✓
- n=10: `1 + 9%9 = 1+0 = 1` ✓
- n=18: `1 + 17%9 = 1+8 = 9` ✓

### Additive Persistence
The number of times you must apply digit sum to reach a single digit.  
Most numbers: persistence ≤ 3.  
Largest known: 11 (takes 233 applications for some 2-digit numbers).

### Sum of Digits Formula for 1..N
$$S(n) = \sum_{k=1}^{n} \text{digitsum}(k)$$

For complete decades (n = 10^d - 1):  
$$S(10^d - 1) = 45 \times d \times 10^{d-1}$$

---

## 11. INTERVIEWER FOLLOW-UP QUESTIONS

**Q1: What is the digital root of n?**  
A: The repeated sum of digits until a single digit. Formula: `1 + (n-1) % 9` for n > 0.

**Q2: Digital root in O(1)?**  
A: Yes — `(n == 0) ? 0 : (1 + (n-1) % 9)`. Proof via mod-9 arithmetic.

**Q3: What's the digit sum of all numbers from 1 to N?**  
A: Use positional counting. O(log N) formula — see `sum_of_digits_1_to_n()`.

**Q4: If digit sum is divisible by 9, what can you say about n?**  
A: n itself is divisible by 9 (classic divisibility rule).

**Q5: Digit sum of n in binary?**  
A: That's the popcount (number of set bits) — use `__builtin_popcount(n)`.

**Q6: Handle INT_MIN (-2147483648) safely?**  
A: `abs(INT_MIN)` overflows signed int. Cast to `unsigned int` or `long long` first.

---

## 12. COMMON MISTAKES

| Mistake | Fix |
|---------|-----|
| `digital_root(9) = 0` | Return 9, not 0: `r == 0 ? 9 : r` |
| `n % 9` for digital root | Use `1 + (n-1) % 9` for clean formula |
| Negative input not handled | `if (n < 0) n = -n` |
| Loop condition `while (n >= 0)` | Should be `while (n > 0)` |
| Not handling n = 0 | Returns 0 (empty digit sum) |

---

## 13. DEBUGGING GUIDE

```c
while (n > 0) {
    int d = n % 10;
    printf("digit=%d, remaining=%d, running_sum=%d\n", d, n/10, sum+=d);
    n /= 10;
}
```

---

## 14. TEST CASES

```c
assert(digit_sum_iterative(0)          == 0);
assert(digit_sum_iterative(123)        == 6);
assert(digit_sum_iterative(9999)       == 36);
assert(digit_sum_iterative(-45)        == 9);
assert(digit_sum_iterative(2147483647) == 46);

assert(digital_root_formula(0)  == 0);
assert(digital_root_formula(1)  == 1);
assert(digital_root_formula(9)  == 9);
assert(digital_root_formula(10) == 1);
assert(digital_root_formula(18) == 9);
assert(digital_root_formula(19) == 1);
assert(digital_root_formula(99) == 9);
```

---

## 15. PERFORMANCE

All iterative approaches: O(log n) — at most 10 iterations for 32-bit int.  
Digital root formula: **O(1)** — single modulo operation.  
For high-frequency computation: always use the O(1) formula for digital root.

---

## 16. REAL-WORLD APPLICATIONS

| Domain         | Application                                        |
|----------------|----------------------------------------------------|
| Cryptography   | Checksum computation (Luhn algorithm uses digit sum) |
| Networking     | IP/TCP/UDP checksum validation                     |
| Embedded       | CRC-style digit-based error detection              |
| Barcode/ISBN   | Check digit computation                            |
| Math education | Divisibility rules                                 |
| Finance        | Bank routing number validation                     |

---

## 17. RELATED PROBLEMS

| Problem                   | Relation                    |
|---------------------------|-----------------------------|
| Happy Number              | Digit sum of squares loop   |
| Armstrong Number          | Digit power sum             |
| Reverse Number            | Same digit extraction loop  |
| Divisibility rules        | Digit sum mod 9/3           |
| Luhn Algorithm            | Weighted digit sum          |
| Additive Persistence      | Count digit-sum iterations  |

---

## 18. FINAL TAKEAWAYS

- Basic digit sum: `while(n > 0) { sum += n % 10; n /= 10; }`
- Digital root: `(n == 0) ? 0 : (1 + (n-1) % 9)` — O(1), elegant
- Negative numbers: take absolute value first
- INT_MIN: cast to `long long` before negation
- Digit sum divisible by 9 → n divisible by 9 (and vice versa)
- **Mantra:** modulo gives last digit → divide removes it → accumulate

---

## 20. ADVANCED EXTENSIONS

### Digit Sum in Any Base
```c
int digit_sum_base(long long n, int base) {
    if (n < 0) n = -n;
    int sum = 0;
    while (n > 0) {
        sum += (int)(n % base);
        n /= base;
    }
    return sum;
}
/* Digital root in base b: 1 + (n-1) % (b-1) for n > 0 */
```

### Luhn Algorithm (Credit Card Validation)
```c
/* Process digits right-to-left.
 * Odd positions: sum as-is.
 * Even positions: double digit; if >9, subtract 9.
 * Valid if total sum % 10 == 0 */
bool luhn_check(const char *card_number);
```

### Sum of Digits 1..N (Competitive Programming)
```c
/* O(log N) formula using positional counting */
/* Each digit position contributes floor(N/10^(k+1)) * 45 * 10^k */
/* Plus partial contribution for the current most-significant digit */
```
