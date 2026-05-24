# 02 — Palindrome Number

**Category:** Foundation > A. Basic Number Problems  
**Difficulty:** Easy → Medium (with constraints)  
**Priority:** TIER-1  
**Companies:** Qualcomm, Intel, NXP, Texas Instruments  

---

## 1. PROBLEM UNDERSTANDING

### Simple English
A number is a **palindrome** if it reads the same forwards and backwards.  
Example: 121 → reads "121" both ways → palindrome.  
Example: 123 → reads "123" forward, "321" backward → not palindrome.

### Mathematical Definition
Let `digits(n) = [d_k, d_{k-1}, ..., d_1, d_0]` where `d_i = floor(n / 10^i) % 10`.  
n is a palindrome iff `d_i == d_{k-i}` for all i in [0, k].

Equivalently:  `n == reverse(n)`

### Visual Explanation
```
n = 12321
Position:  [1][2][3][2][1]
Mirror:    [1][2][3][2][1]
           ↑           ↑
         same        same  → PALINDROME

n = 12345
Position:  [1][2][3][4][5]
Mirror:    [5][4][3][2][1]
           ↑           ↑
         1 ≠ 5        → NOT PALINDROME
```

### Real-World Analogy
Like reading a sentence forward and backward (e.g., "racecar").  
In hardware: a symmetric bit pattern — common in flags/masks.

### What Interviewers Are Testing
- Can you extract digits using modulo and division?
- Do you handle negative numbers and single digits?
- Can you do it without converting to a string?
- Do you know the half-reversal trick to avoid overflow?
- Can you extend to string palindromes?

### Common Traps
- Negative numbers: `-121` → NOT palindrome (leading minus sign)
- `10`, `100`: trailing zeros → reversed form has leading zeros → NOT palindrome
- `0`: IS a palindrome (0 == 0)
- Overflow when reversing: `1234567899` reversed may overflow `int`

---

## 2. INPUT/OUTPUT ANALYSIS

| Input      | Output | Notes                            |
|------------|--------|----------------------------------|
| 0          | YES    | 0 reversed is 0                  |
| 1          | YES    | Single digit                     |
| 9          | YES    | Single digit                     |
| 11         | YES    | Two same digits                  |
| 121        | YES    | Classic palindrome               |
| 123        | NO     | Not symmetric                    |
| -121       | NO     | Negative: reads "-121" vs "121-" |
| 10         | NO     | Reversed = 01 = 1 ≠ 10          |
| 1000021    | NO     | Ends in 0, reversed ≠ original   |
| INT_MAX    | NO     | 2147483647, reversed overflows   |

### Overflow Warning
```c
/* Reversing INT_MAX (2147483647) → 7463847412 overflows int */
/* Safe approach: only reverse half the digits */
/* OR: use long long for the reversed number */
```

---

## 3. EDGE CASE ANALYSIS

| Case               | Value      | Expected | Reason                         |
|--------------------|------------|----------|--------------------------------|
| Zero               | 0          | YES      | Palindrome by definition       |
| Single digit       | 1–9        | YES      | Always palindrome              |
| Negative           | -121       | NO       | Sign makes it asymmetric       |
| Ends in 0 (≠ 0)    | 10, 100    | NO       | 01 ≠ 10; reversed ≠ original   |
| Two same digits    | 11, 22     | YES      | Symmetric                      |
| Even digits        | 1221       | YES      | Symmetric pair                 |
| Odd digits         | 12321      | YES      | Mirror around center           |
| Large palindrome   | 1000000001 | YES      | Need long long                 |
| Large non-palindrome| INT_MAX   | NO       | 2147483647 ≠ 7463847412        |

---

## 4. BRUTE FORCE — String Conversion

### Intuition
Convert the number to a string, then check if the string is a palindrome.

### Dry Run (n = 12321)
```
sprintf → "12321"
left=0, right=4: '1' == '1' ✓
left=1, right=3: '2' == '2' ✓
left=2, right=2: center → PALINDROME
```

### Implementation
```c
#include <stdio.h>
#include <string.h>

/* Time: O(log n), Space: O(log n) — string buffer */
bool is_palindrome_string(int n) {
    if (n < 0) return false;

    char buf[12];   /* INT_MAX has at most 10 digits + sign + null */
    snprintf(buf, sizeof(buf), "%d", n);

    int len = (int)strlen(buf);
    int left = 0, right = len - 1;
    while (left < right) {
        if (buf[left] != buf[right]) return false;
        left++;
        right--;
    }
    return true;
}
```

- **Pros:** Simple, readable
- **Cons:** Uses extra O(log n) space; string operations; not suitable for embedded

---

## 5. BETTER SOLUTION — Full Reversal

### Idea
Reverse all digits of n into a new number. Compare with original.  
Use `long long` to avoid overflow.

### Dry Run (n = 121)
```
original = 121
reversed = 0
Step 1: digit = 121 % 10 = 1, reversed = 1, n = 12
Step 2: digit = 12 % 10 = 2,  reversed = 12, n = 1
Step 3: digit = 1 % 10 = 1,   reversed = 121, n = 0
reversed (121) == original (121) → PALINDROME
```

### Dry Run (n = 10)
```
original = 10
reversed = 0
Step 1: digit = 10 % 10 = 0,  reversed = 0, n = 1
Step 2: digit = 1 % 10 = 1,   reversed = 1, n = 0
reversed (1) != original (10) → NOT PALINDROME
```

### Implementation
```c
/* Time: O(log n), Space: O(1) */
bool is_palindrome_full_reverse(int n) {
    if (n < 0) return false;

    long long original = (long long)n;
    long long reversed = 0;
    long long temp = original;

    while (temp > 0) {
        reversed = reversed * 10 + temp % 10;
        temp /= 10;
    }
    return original == reversed;
}
```

---

## 6. OPTIMAL SOLUTION — Half Reversal (LeetCode Standard)

### Key Insight
We only need to reverse the **second half** of the digits.  
If the first half equals the reversed second half → palindrome.  
This avoids overflow entirely for numbers that fit in `int`.

### Special Cases
- `n < 0` → NOT palindrome
- `n > 0 && n % 10 == 0` → NOT palindrome (10, 100, etc.)
- `n == 0` → IS palindrome

### Dry Run (n = 1221)
```
n = 1221, reversed_half = 0
Step 1: n%10=1, reversed_half=1, n=122
Step 2: n%10=2, reversed_half=12, n=12
Now: n (12) == reversed_half (12) → PALINDROME
```

### Dry Run (n = 12321)
```
n = 12321, reversed_half = 0
Step 1: n%10=1, reversed_half=1, n=1232
Step 2: n%10=2, reversed_half=12, n=123
Step 3: n%10=3, reversed_half=123, n=12
Now: n (12) == reversed_half/10 (12) → PALINDROME (odd digits: ignore center)
```

### Implementation
```c
/*
 * is_palindrome_half_reverse - Optimal O(log n) palindrome check
 * Avoids overflow by reversing only the second half of digits.
 * Time: O(log n), Space: O(1)
 */
bool is_palindrome_half_reverse(int n) {
    /* Negative numbers and numbers ending in 0 (except 0 itself) */
    if (n < 0 || (n % 10 == 0 && n != 0)) return false;

    int reversed_half = 0;
    while (n > reversed_half) {
        reversed_half = reversed_half * 10 + n % 10;
        n /= 10;
    }
    /*
     * Even digits: n == reversed_half
     * Odd digits:  n == reversed_half / 10  (center digit ignored)
     */
    return n == reversed_half || n == reversed_half / 10;
}
```

---

## 7. MULTIPLE APPROACHES

### Approach 1: String Conversion
```c
bool is_palindrome_string(int n) { /* See Section 4 */ }
```
| | |
|---|---|
| Time | O(log n) |
| Space | O(log n) |
| Pros | Simple, readable |
| Cons | Extra allocation, not embedded-friendly |
| Interview | Show as first idea, then improve |

---

### Approach 2: Full Digit Reversal
```c
bool is_palindrome_full_reverse(int n) { /* See Section 5 */ }
```
| | |
|---|---|
| Time | O(log n) |
| Space | O(1) |
| Pros | Clean, easy to reason about |
| Cons | Needs long long to avoid overflow |
| Interview | Good intermediate answer |

---

### Approach 3: Half Reversal [PREFERRED]
```c
bool is_palindrome_half_reverse(int n) { /* See Section 6 */ }
```
| | |
|---|---|
| Time | O(log n/2) |
| Space | O(1) |
| Pros | Overflow-safe with int, minimal iterations |
| Cons | Slightly tricky odd/even case |
| Interview | Standard optimal answer |

---

### Approach 4: Digit Array Comparison
```c
/* Time: O(log n), Space: O(log n) — digit array */
bool is_palindrome_digit_array(int n) {
    if (n < 0) return false;
    if (n == 0) return true;

    int digits[10];   /* max 10 digits for int */
    int count = 0;

    while (n > 0) {
        digits[count++] = n % 10;
        n /= 10;
    }
    /* digits[] now holds digits in reverse order */
    /* digits[0] = LSD, digits[count-1] = MSD */
    /* Palindrome: digits[i] == digits[count-1-i] */
    for (int i = 0; i < count / 2; i++) {
        if (digits[i] != digits[count - 1 - i]) return false;
    }
    return true;
}
```
| | |
|---|---|
| Pros | Explicit and clear |
| Cons | Uses array; O(log n) space |
| Interview | Good for embedded where strlen is unavailable |

---

### Approach 5: Recursive Digit Comparison
```c
/* Helper: get number of digits */
static int num_digits(int n) {
    int count = 0;
    while (n > 0) { n /= 10; count++; }
    return count;
}

/* Get d-th digit from left (1-indexed) */
static int get_digit_from_left(int n, int d, int total_digits) {
    int divisor = 1;
    for (int i = 0; i < total_digits - d; i++) divisor *= 10;
    return (n / divisor) % 10;
}

/* Get d-th digit from right (1-indexed) */
static int get_digit_from_right(int n, int d) {
    int divisor = 1;
    for (int i = 0; i < d - 1; i++) divisor *= 10;
    return (n / divisor) % 10;
}

/* Time: O(log^2 n), Space: O(log n) recursion stack */
bool is_palindrome_recursive(int n, int left, int right, int total) {
    if (left >= right) return true;
    int l_digit = get_digit_from_left(n, left, total);
    int r_digit = get_digit_from_right(n, right);
    if (l_digit != r_digit) return false;
    return is_palindrome_recursive(n, left + 1, right - 1, total);
}

bool is_palindrome_rec_wrapper(int n) {
    if (n < 0) return false;
    if (n == 0) return true;
    int total = num_digits(n);
    return is_palindrome_recursive(n, 1, total, total);
}
```
| | |
|---|---|
| Pros | Elegant; shows recursion skill |
| Cons | O(log^2 n) time; stack usage |
| Interview | Good to mention, but not preferred |

---

### Approach 6: Two Pointer on Digit Extraction (In-place, no array)
```c
/*
 * Extract left digit: n / 10^(k-1) % 10
 * Extract right digit: n % 10
 * Shrink: remove both endpoints each iteration
 * Time: O(log n), Space: O(1)
 */
bool is_palindrome_two_pointer(int n) {
    if (n < 0) return false;
    if (n == 0) return true;

    int divisor = 1;
    while (n / divisor >= 10) divisor *= 10;  /* divisor = 10^(k-1) */

    while (n > 0) {
        int left_digit  = n / divisor;         /* MSD */
        int right_digit = n % 10;              /* LSD */
        if (left_digit != right_digit) return false;
        n = (n % divisor) / 10;               /* strip left and right digits */
        divisor /= 100;                        /* two digits removed */
    }
    return true;
}
```
| | |
|---|---|
| Pros | No array; O(1) space; clean two-pointer logic |
| Cons | Divisor overflow risk for very large n |
| Interview | Excellent — shows two-pointer thinking |

---

## 8. LOW-LEVEL C DISCUSSION

### Integer Overflow
```c
/* Full reversal of INT_MAX (2147483647) = 7463847412 → overflows int */
/* Fix: use long long for reversed variable */
long long reversed = 0;
```

### Modulo Behavior for Negative Numbers (C99/C11)
```c
/* In C99+: (-121) % 10 == -1 (not 9) */
/* So: for negative n, digit extraction gives wrong results */
/* Always check n < 0 first */
```

### Division Truncation
```c
/* In C: integer division truncates toward zero */
/* -12 / 10 == -1 (not -2) */
/* This is defined behavior in C99+ */
```

### Trailing Zero Shortcut
```c
/* n ends in 0 (and n != 0) → reversed would start with 0 */
/* e.g., 10 → reversed = 01 = 1 ≠ 10 */
/* Early exit: if (n > 0 && n % 10 == 0) return false */
```

### Stack Consideration
- Recursive approach uses O(log n) stack frames (~10 for 32-bit int)
- Safe for embedded (max ~10 frames)
- Iterative approaches preferred for strict stack budgets

---

## 9. BITWISE INSIGHTS

### Even/Odd Digit Count Check
```c
/* Count digits using bit width approximation */
/* __builtin_clz(n) counts leading zeros; digits ≈ 32 - clz(n) */
/* Not directly useful for palindrome, but relevant in digit counting */
```

### Symmetry in Binary
```c
/*
 * Note: A decimal palindrome is NOT necessarily a binary palindrome.
 * 9 = 1001 in binary → palindrome in both bases (rare)
 * This is a variant problem: "binary palindrome"
 */
bool is_binary_palindrome(unsigned int n) {
    unsigned int original = n;
    unsigned int reversed = 0;
    while (n > 0) {
        reversed = (reversed << 1) | (n & 1);
        n >>= 1;
    }
    return original == reversed;
}
```

---

## 10. MATHEMATICAL INSIGHTS

### Palindrome Number Formula
A k-digit palindrome has the form determined by its first ⌈k/2⌉ digits.  
Number of k-digit palindromes:
- k=1: 9 (1–9)
- k=2: 9 (11, 22, ..., 99)
- k=3: 90 (101–999, symmetric)
- k=n: 9 × 10^(⌊n/2⌋ - 1) for n ≥ 2

### Reversal Arithmetic
For a number n with k digits, its reversal is:
$$R(n) = \sum_{i=0}^{k-1} d_i \cdot 10^{k-1-i} \quad \text{where } d_i = \lfloor n/10^i \rfloor \bmod 10$$

n is palindrome iff `n == R(n)`.

---

## 11. INTERVIEWER FOLLOW-UP QUESTIONS

**Q1: What about negative numbers?**  
A: Not palindromes. The minus sign makes them asymmetric. Return false for n < 0.

**Q2: Can you do it without converting to a string?**  
A: Yes — use the half-reversal or two-pointer digit extraction. O(1) space.

**Q3: What about overflow when reversing?**  
A: Use `long long` for full reversal, or use half-reversal (avoids overflow for valid int input).

**Q4: Is 0 a palindrome?**  
A: Yes. `0` reversed is `0`. It reads the same both ways.

**Q5: Extend to string palindrome?**  
A: Two-pointer approach: `left=0, right=len-1`, compare and move inward.

**Q6: What if the number has trailing zeros?**  
A: Any number > 0 ending in 0 is NOT a palindrome. If it were, it would start with 0, which means it's a smaller number.

**Q7: Can you check if a linked list of digits is a palindrome?**  
A: Use stack to store first half, compare with second half. O(n) time, O(n/2) space. Or reverse second half in-place.

**Q8: How many palindromes are in [1, 1000]?**  
A: 1-9 (9) + 11,22..99 (9) + 101,111,...,999 (90) = 108.

---

## 12. COMMON MISTAKES

| Mistake | Example | Fix |
|---------|---------|-----|
| Forget negative check | `-121` returned as palindrome | `if (n < 0) return false` |
| Overflow in reversal | `int reversed` for large n | Use `long long reversed` |
| Forget `n==0` special case | 0 returned as not palindrome | `if (n == 0) return true` |
| Ignore trailing zeros | `10` returned as palindrome | `if (n%10==0 && n!=0) return false` |
| Wrong loop termination | `while(n > 0)` not `while(n > reversed_half)` | Use half-reversal correctly |
| Off-by-one in digit count | Wrong center for odd digits | Use `reversed_half/10` for odd case |

---

## 13. DEBUGGING GUIDE

```c
bool is_palindrome_debug(int n) {
    printf("Input: %d\n", n);
    if (n < 0) { printf("Negative → false\n"); return false; }
    if (n != 0 && n % 10 == 0) { printf("Trailing zero → false\n"); return false; }

    int original = n;
    int rev = 0;
    printf("Digits reversed:\n");
    while (n > rev) {
        int digit = n % 10;
        rev = rev * 10 + digit;
        n /= 10;
        printf("  digit=%d, remaining=%d, reversed_half=%d\n", digit, n, rev);
    }
    bool result = (n == rev || n == rev / 10);
    printf("n=%d, rev=%d → %s\n", n, rev, result ? "PALINDROME" : "NOT PALINDROME");
    return result;
}
```

---

## 14. TEST CASES

```c
/* Single digits — all palindromes */
for (int i = 0; i <= 9; i++) assert(is_palindrome_half_reverse(i) == true);

/* Two-digit */
assert(is_palindrome_half_reverse(11)  == true);
assert(is_palindrome_half_reverse(12)  == false);
assert(is_palindrome_half_reverse(99)  == true);

/* Three-digit */
assert(is_palindrome_half_reverse(121) == true);
assert(is_palindrome_half_reverse(123) == false);
assert(is_palindrome_half_reverse(100) == false);

/* Four-digit */
assert(is_palindrome_half_reverse(1221) == true);
assert(is_palindrome_half_reverse(1231) == false);

/* Edge */
assert(is_palindrome_half_reverse(-121)     == false);
assert(is_palindrome_half_reverse(0)        == true);
assert(is_palindrome_half_reverse(10)       == false);
assert(is_palindrome_half_reverse(1000021)  == false);
assert(is_palindrome_half_reverse(2147483647) == false);
```

---

## 15. PERFORMANCE BENCHMARKING

| Approach          | Iterations for n=12321 | Memory   | Notes                  |
|-------------------|------------------------|----------|------------------------|
| String            | ~10 char ops           | O(log n) | Extra allocation       |
| Full Reversal     | 5 (all digits)         | O(1)     | Needs long long        |
| Half Reversal     | 2–3 (half digits)      | O(1)     | Optimal                |
| Two Pointer       | 2–3 (half digits)      | O(1)     | Same as half reversal  |
| Digit Array       | 5 + 2 compare          | O(log n) | Array on stack         |

All approaches: **O(log n)** time — dominated by number of digits.

---

## 16. REAL-WORLD APPLICATIONS

| Domain          | Application                                        |
|-----------------|----------------------------------------------------|
| Networking      | Palindrome check for symmetric packet patterns     |
| Compilers       | Token symmetry in bracket matching                 |
| DNA Bioinformatics | Palindromic sequences in restriction enzymes  |
| Cryptography    | Palindrome-based obfuscation detection             |
| Embedded        | Symmetric register values in protocol frames       |
| Testing         | Generating palindromic test vectors                |
| Game dev        | Score palindromes as bonus conditions              |

---

## 17. RELATED PROBLEMS

| Problem                        | Relation              |
|--------------------------------|-----------------------|
| Reverse a number               | Core sub-operation    |
| String palindrome              | Extension to strings  |
| Longest palindromic substring  | Advanced extension    |
| Palindrome linked list         | Structural variant    |
| Count palindromes in range     | Math + sieve combo    |
| Binary palindrome              | Base-2 variant        |
| Near-palindrome (off by one)   | Follow-up variant     |

---

## 18. FINAL TAKEAWAYS

- Negative numbers and trailing zeros → NOT palindrome (early exit)
- Half-reversal avoids overflow and halves iterations
- `n == reversed_half || n == reversed_half/10` handles even/odd digit counts
- String approach is fine for interviews if you note the space tradeoff
- Two-pointer on digits is the most elegant O(1) space solution
- **Interview mantra:** String → Full Reverse → Half Reverse → explain overflow concern

---

## 20. ADVANCED EXTENSIONS

### Embedded Version
```c
/* No stdlib — manual digit extraction only, no snprintf */
bool is_palindrome_embedded(uint32_t n) {
    if (n == 0) return true;
    if (n % 10 == 0) return false;
    uint32_t rev = 0;
    while (n > rev) {
        rev = rev * 10 + n % 10;
        n /= 10;
    }
    return n == rev || n == rev / 10;
}
```

### Streaming Version
```c
/* Check if a digit stream forms a palindrome */
/* Use a circular buffer or stack to hold first half */
/* Compare second half against stack in reverse */
```

### Binary Palindrome Check
```c
bool is_binary_palindrome(uint32_t n) {
    uint32_t rev = 0, temp = n;
    int bits = 0;
    while (temp) { bits++; temp >>= 1; }
    for (int i = 0; i < bits; i++) {
        rev = (rev << 1) | (n & 1);
        n >>= 1;
    }
    /* Compare rev to original: need to store original */
    /* Use separate copy */
    return false; /* placeholder — see palindrome.c for full impl */
}
```
