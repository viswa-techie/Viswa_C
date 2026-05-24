# 03 — Reverse Number

**Category:** Foundation > A. Basic Number Problems  
**Difficulty:** Easy → Medium (with overflow handling)  
**Priority:** TIER-1  
**Companies:** Qualcomm, Intel, NXP, Nvidia, AMD  

---

## 1. PROBLEM UNDERSTANDING

### Simple English
Given an integer, reverse its digits and return the result.  
If the reversed number overflows a 32-bit integer, return 0 (LeetCode convention) or a defined sentinel.

### Visual Explanation
```
n = 12345
Digits: [1][2][3][4][5]
Rev:    [5][4][3][2][1]
Result: 54321

n = -456
Digits: [-][4][5][6]
Rev:    -[6][5][4]
Result: -654

n = 1200
Digits: [1][2][0][0]
Rev:    [0][0][2][1] = 21
Result: 21  (leading zeros drop)
```

### What Interviewers Are Testing
- Digit extraction using `%` and `/`
- Handling of negative numbers (sign preservation)
- Overflow detection BEFORE the assignment happens
- Trailing zero behavior (they disappear after reversal)

### Common Traps
- Overflow: `reverse(1534236469)` = 9646324351 → exceeds INT_MAX
- Negative modulo in C: `(-123) % 10 = -3` (correct sign-aware extraction)
- Trailing zeros: `120` → reversed `021` = `21` (natural, not a bug)
- INT_MIN handling: `abs(INT_MIN)` is UB in C

---

## 2. INPUT/OUTPUT ANALYSIS

| Input       | Output    | Notes                               |
|-------------|-----------|-------------------------------------|
| 123         | 321       | Basic                               |
| -456        | -654      | Sign preserved                      |
| 120         | 21        | Trailing zeros become leading       |
| 0           | 0         | Trivial                             |
| 1           | 1         | Single digit                        |
| 1534236469  | 0 / INT_MIN | Reversed overflows INT            |
| INT_MAX     | 0 / overflow | 7463847412 > INT_MAX             |
| INT_MIN     | UB risk   | abs(INT_MIN) overflows signed int   |

---

## 3. EDGE CASE ANALYSIS

| Case                   | Value        | Expected      | Reason                          |
|------------------------|--------------|---------------|---------------------------------|
| Zero                   | 0            | 0             | Trivially                       |
| Single digit           | 5            | 5             | No reversal needed              |
| Negative               | -789         | -987          | Sign preserved, digits reversed |
| Trailing zeros         | 5000         | 5             | 0005 = 5                        |
| Overflow positive      | 1999999999   | 0/sentinel    | Reversed = 9999999991 > INT_MAX |
| Overflow negative      | -1999999999  | 0/sentinel    | Reversed < INT_MIN              |
| INT_MAX = 2147483647   | INT_MAX      | 0/sentinel    | 7463847412 overflows            |
| INT_MIN = -2147483648  | INT_MIN      | handle UB     | abs(INT_MIN) is UB              |

---

## 4. BRUTE FORCE — Basic Loop

### Intuition
Extract digits one by one using `% 10` and accumulate in reversed order.

### Dry Run (n = 1234)
```
result = 0
Step 1: digit = 1234 % 10 = 4, result = 4, n = 123
Step 2: digit = 123  % 10 = 3, result = 43, n = 12
Step 3: digit = 12   % 10 = 2, result = 432, n = 1
Step 4: digit = 1    % 10 = 1, result = 4321, n = 0
Done: 4321
```

### Implementation
```c
long long reverse_basic(int n) {
    long long result = 0;
    int sign = (n < 0) ? -1 : 1;
    long long num = (n < 0) ? -(long long)n : (long long)n;
    while (num > 0) {
        result = result * 10 + num % 10;
        num /= 10;
    }
    return sign * result;
}
```
**Note:** Uses `long long` to detect overflow — doesn't clamp to INT range.

---

## 5. BETTER SOLUTION — Overflow-Safe Int Return

### Key Challenge
We must check overflow **before** performing the multiplication.

### Overflow Condition
```
result = result * 10 + digit
→ if result > INT_MAX / 10: result*10 overflows
→ if result == INT_MAX / 10 and digit > 7: overflow
  (INT_MAX = 2147483647, ends in 7)
```

### Implementation
```c
int reverse_overflow_safe(int n) {
    int result = 0;
    while (n != 0) {
        int digit = n % 10;
        n /= 10;
        if (result > INT_MAX / 10 || (result == INT_MAX / 10 && digit > 7))
            return 0;  /* overflow */
        if (result < INT_MIN / 10 || (result == INT_MIN / 10 && digit < -8))
            return 0;  /* underflow */
        result = result * 10 + digit;
    }
    return result;
}
```

---

## 6. OPTIMAL SOLUTION

The overflow-safe approach above is optimal:
- **Time:** O(log n) — one pass through digits
- **Space:** O(1)
- **Overflow-safe:** check before assignment
- **Sign-aware:** works naturally with C99's truncation-toward-zero division

---

## 7. MULTIPLE APPROACHES

### Summary Table

| Approach          | Time      | Space     | Overflow-safe | Notes                    |
|-------------------|-----------|-----------|---------------|--------------------------|
| Basic Loop        | O(log n)  | O(1)      | Partial (long long) | Returns long long   |
| Overflow-Safe     | O(log n)  | O(1)      | YES           | Returns INT_MIN sentinel |
| String-Based      | O(log n)  | O(log n)  | YES (string)  | Uses char buffer         |
| Recursive         | O(log n)  | O(log n)  | Partial       | Stack usage              |
| Positional        | O(log n)  | O(1)      | Partial       | Two-pass                 |

---

## 8. LOW-LEVEL C DISCUSSION

### Negative Modulo (C99/C11)
```c
-123 % 10 == -3  /* C99: result has sign of dividend */
-123 / 10 == -12 /* truncates toward zero */
```
This is correct for our purpose — `digit` carries the sign, and so does the final result.

### INT_MIN Special Case
```c
/* abs(INT_MIN) = abs(-2147483648) = 2147483648 which overflows int! */
/* Safe: cast to long long before negation */
long long abs_val = (n == INT_MIN) ? 2147483648LL : (long long)abs(n);
```

### Signed Overflow is UB
```c
/* result * 10 can overflow if result is large */
/* Must check BEFORE multiplication: */
if (result > INT_MAX / 10) /* overflow will happen */
```

---

## 9. BITWISE INSIGHTS

### Fast Sign Detection
```c
int sign = 1 - ((n >> 31) & 1) * 2;  /* 1 for positive, -1 for negative */
/* Branchless, works on 32-bit two's complement */
```

### Digit Extraction Using Bit Operations
```c
/* Modulo by power of 2 uses masking */
/* n % 16 == n & 15  (fast) */
/* But decimal digits: no bitwise shortcut for n % 10 */
```

---

## 10. MATHEMATICAL INSIGHTS

### Digit Extraction Formula
For a k-digit number n:
$$d_i = \lfloor n / 10^i \rfloor \bmod 10, \quad i = 0, 1, ..., k-1$$

Reversed:
$$R(n) = \sum_{i=0}^{k-1} d_i \cdot 10^{k-1-i}$$

### Overflow Bound
INT_MAX = 2,147,483,647 (10 digits)  
Any reversed number with 10 digits that exceeds this overflows.  
Specifically: if reversed number starts with digit > 2 and has 10 digits → overflow.

---

## 11. INTERVIEWER FOLLOW-UP QUESTIONS

**Q1: What if reversed number overflows?**  
A: Return 0 (LeetCode), INT_MIN sentinel, or use error code. Check before each multiply.

**Q2: How to handle INT_MIN?**  
A: Special-case: `if (n == INT_MIN) return 0;` since reversed = 8463847412 overflows.

**Q3: Can you do it with a string?**  
A: Yes — `snprintf` → reverse chars → `atoi`. But requires O(log n) extra space.

**Q4: Can you do it recursively?**  
A: Yes — `reverse(n) = (n%10) * 10^(digits(n)-1) + reverse(n/10)`.  
Caution: O(log n) stack depth.

**Q5: What is the time complexity?**  
A: O(log₁₀ n) — one iteration per decimal digit.

**Q6: What if we need to reverse a 64-bit integer?**  
A: Use `__int128` or check overflow with `LLONG_MAX / 10` bounds.

---

## 12. COMMON MISTAKES

| Mistake | Description | Fix |
|---------|-------------|-----|
| Overflow ignored | `int result` without check | Use long long or pre-check |
| abs(INT_MIN) UB | `abs(-2147483648)` overflows | Cast to `long long` first |
| Sign lost | Not preserving sign for negative | Apply sign at end |
| Leading zeros | 5000 → "0005" interpreted as 5 | Natural: loop stops at 0 |
| Wrong loop | `while (n >= 0)` for negatives | Use `while (n != 0)` |

---

## 13. DEBUGGING GUIDE

```c
/* Add trace prints to reverse_overflow_safe */
int digit = n % 10;
printf("digit=%d, n=%d, result_before=%d\n", digit, n/10, result);
/* Check: does result*10+digit exceed INT_MAX? */
printf("INT_MAX/10=%d, digit>7? %d\n", INT_MAX/10, digit > 7);
```

---

## 14. TEST CASES

```c
assert(reverse_overflow_safe(123)        == 321);
assert(reverse_overflow_safe(-456)       == -654);
assert(reverse_overflow_safe(120)        == 21);
assert(reverse_overflow_safe(0)          == 0);
assert(reverse_overflow_safe(1)          == 1);
assert(reverse_overflow_safe(-1)         == -1);
assert(reverse_overflow_safe(1534236469) == 0);  /* overflow */
assert(reverse_overflow_safe(2147483647) == 0);  /* INT_MAX, overflows */
```

---

## 15. PERFORMANCE

All approaches are O(log n) — bounded by number of decimal digits.  
For 32-bit int: maximum 10 digits → at most 10 iterations.  
**No meaningful performance difference** between approaches at this scale.  
For embedded: prefer the no-stdlib overflow-safe loop.

---

## 16. REAL-WORLD APPLICATIONS

| Domain        | Application                                          |
|---------------|------------------------------------------------------|
| Embedded      | BCD (Binary Coded Decimal) digit reversal            |
| Networking    | Byte-order reversal (endian conversion analog)       |
| Cryptography  | Digit permutation in number-theoretic functions      |
| DSP           | Bit-reversal for FFT (conceptual analog)             |
| Compilers     | Constant folding of reversed integer literals        |
| Testing       | Generate test cases with reversed inputs             |

---

## 17. RELATED PROBLEMS

| Problem              | Relation             |
|----------------------|----------------------|
| Palindrome Number    | Uses digit reversal  |
| Reverse Bits         | Bit-level analog     |
| Reverse String       | String analog        |
| Digit Sum            | Same digit loop      |
| Armstrong Number     | Same digit loop      |

---

## 18. FINAL TAKEAWAYS

- Use `n % 10` for LSD, `n /= 10` to shift right
- Always handle negative: sign = (n < 0) ? -1 : 1
- Overflow check: `result > INT_MAX/10` BEFORE multiply
- INT_MIN special case: its abs value overflows int
- For interview: present overflow-safe solution with the bounds check reasoning
- **Mantra:** digit = n%10 → n/=10 → result = result*10 + digit

---

## 20. ADVANCED EXTENSIONS

### Reverse 64-bit Integer
```c
long long reverse_int64(long long n) {
    long long result = 0;
    int sign = (n < 0) ? -1 : 1;
    unsigned long long num = (n < 0) ? (unsigned long long)(-(n+1)) + 1 : (unsigned long long)n;
    while (num > 0) {
        result = result * 10 + (long long)(num % 10);
        num /= 10;
    }
    return (long long)sign * result;
}
```

### Embedded Version (No stdlib, no long long)
```c
int reverse_embedded(int n) {
    int result = 0;
    while (n != 0) {
        int digit = n % 10;
        n /= 10;
        /* Overflow guard using hardware-matched INT_MAX */
        if (result > 214748364 || (result == 214748364 && digit > 7)) return 0;
        if (result < -214748364 || (result == -214748364 && digit < -8)) return 0;
        result = result * 10 + digit;
    }
    return result;
}
```
