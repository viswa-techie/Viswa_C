# 05 — Factorial

**Category:** Foundation > A. Basic Number Problems  
**Difficulty:** Easy → Medium (overflow, big numbers)  
**Priority:** TIER-1  
**Companies:** Qualcomm, Intel, AMD, NXP, Nvidia  

---

## 1. PROBLEM UNDERSTANDING

### Simple English
`n!` (n factorial) = product of all positive integers from 1 to n.  
`0! = 1` by mathematical convention.

### Mathematical Definition
$$n! = \prod_{i=1}^{n} i = 1 \times 2 \times 3 \times \cdots \times n, \quad 0! = 1$$

Recursive definition: `n! = n × (n-1)!`

### Visual Explanation
```
5! = 5 × 4 × 3 × 2 × 1 = 120

Trace:
result = 1
× 2 → 2
× 3 → 6
× 4 → 24
× 5 → 120
```

### What Interviewers Are Testing
- Iterative vs recursive tradeoff
- Stack overflow awareness for large n
- Overflow management (int vs long long vs big integer)
- Tail recursion optimization understanding
- Precomputed table for embedded use

### Common Traps
- `0!` is 1, not 0
- `1!` is 1
- Using `int`: overflows at n=13 (13! = 6,227,020,800 > INT_MAX)
- Using `unsigned long long`: overflows at n=21
- Recursive without base case → infinite loop

---

## 2. INPUT/OUTPUT ANALYSIS

| Input | Output                           | Fits in      |
|-------|----------------------------------|--------------|
| 0     | 1                                | int          |
| 1     | 1                                | int          |
| 5     | 120                              | int          |
| 10    | 3,628,800                        | int          |
| 12    | 479,001,600                      | int (barely) |
| 13    | 6,227,020,800                    | uint64       |
| 20    | 2,432,902,008,176,640,000         | uint64       |
| 21    | 51,090,942,171,709,440,000        | OVERFLOW uint64 |
| 25    | 15,511,210,043,330,985,984,000,000| Need big int |
| -1    | Undefined → return 0/error        | N/A          |

---

## 3. EDGE CASES

| Case       | Value | Expected | Reason                   |
|------------|-------|----------|--------------------------|
| 0          | 0     | 1        | Convention: 0! = 1       |
| 1          | 1     | 1        | Base case                |
| Negative   | -5    | 0/error  | Not defined              |
| int limit  | 12    | 479001600 | Last that fits int32    |
| uint64 limit| 20   | 2432902008176640000 | Last uint64    |
| Overflow   | 21    | overflow | Need big integer         |

---

## 4. BRUTE FORCE — Simple Loop

```c
unsigned long long factorial_iterative(int n) {
    if (n < 0) return 0;
    unsigned long long result = 1;
    for (int i = 2; i <= n; i++) result *= i;
    return result;
}
```
- **Time:** O(n)
- **Space:** O(1)
- **Note:** Safe up to n=20 with `unsigned long long`

---

## 5. BETTER SOLUTION — Overflow-Safe

```c
bool factorial_safe(int n, unsigned long long *result) {
    if (n < 0 || n > 20 || !result) return false;
    *result = fact_table[n];   /* precomputed table */
    return true;
}
```
Returns `false` if overflow would occur.

---

## 6. OPTIMAL SOLUTIONS

### For Fixed n ≤ 20: Lookup Table (O(1))
```c
static const unsigned long long fact_table[21] = {
    1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880,
    3628800, 39916800, 479001600, 6227020800, 87178291200,
    1307674368000, 20922789888000, 355687428096000,
    6402373705728000, 121645100408832000, 2432902008176640000
};
```
- **Time:** O(1)
- **Space:** O(1)
- Ideal for embedded firmware (store in ROM/flash)

### For Arbitrary n: Big Integer
```c
void factorial_bigint(int n, char *result_str, int str_size);
```
- Multiplies digit-by-digit
- No overflow
- Slower: O(n² log n)

---

## 7. MULTIPLE APPROACHES

### Approach 1: Iterative [PREFERRED for firmware]
- O(n) time, O(1) space
- Safe, simple, no recursion risk

### Approach 2: Recursive
```c
unsigned long long factorial_recursive(int n) {
    if (n <= 1) return 1;
    return (unsigned long long)n * factorial_recursive(n - 1);
}
```
- O(n) stack depth → risk for large n
- Not suitable for RTOS or constrained stack (e.g., 512 bytes)

### Approach 3: Tail-Recursive
```c
unsigned long long factorial_tail_helper(int n, unsigned long long acc) {
    if (n <= 1) return acc;
    return factorial_tail_helper(n - 1, acc * n);
}
```
- Compiler may optimize to iteration (GCC with -O2 does)
- Still n stack frames without TCO

### Approach 4: Lookup Table [PREFERRED for embedded O(1)]
- Precomputed 21-entry table
- Zero computation at runtime
- Perfect for MCU ROM storage

### Approach 5: Memoized Recursive
- Cache results in static array
- O(1) after first call for same range
- Good for repeated calls in same program run

### Approach 6: Big Integer (Arbitrary Precision)
- Store digits as int array
- Multiply one digit at a time
- Supports any n without overflow
- Used in competitive programming and math tools

---

## 8. LOW-LEVEL C DISCUSSION

### Overflow Boundary
```c
/* int32: n=12 → 479001600 (fits), n=13 → 6227020800 (overflows) */
/* uint64: n=20 → 2432902008176640000 (fits), n=21 overflows */
/* Verify at compile time: */
_Static_assert(sizeof(unsigned long long) >= 8, "Need 64-bit ull");
```

### Recursion Stack Risk
```c
/* Each recursive call uses ~48 bytes on x86-64 (frame pointer, return addr, locals) */
/* factorial_recursive(10000): ~48 * 10000 = ~480 KB stack */
/* Default Linux stack: ~8 MB — ok for n < 100000 */
/* RTOS embedded stacks: often 512 bytes → max ~10 frames → n < 10 unsafe */
```

### Tail Call Optimization (TCO)
```c
/* GCC -O2: tail recursion is often converted to a loop */
/* Verify: gcc -O2 -foptimize-sibling-calls */
/* Never rely on TCO in safety-critical firmware (compile with -fno-optimize-sibling-calls) */
```

### Signed vs Unsigned
```c
/* Use unsigned long long (uint64_t) for factorial — always positive */
/* Signed overflow (int overflow) is UB in C */
/* Unsigned overflow is defined (wraps around) but produces wrong answer */
```

---

## 9. BITWISE INSIGHTS

### Count Trailing Zeros in n! (Legendre's Formula)
The number of trailing zeros in n! = number of times 5 divides n!  
(since 2s are more abundant than 5s):

$$\text{trailing zeros} = \sum_{i=1}^{\infty} \lfloor n / 5^i \rfloor$$

```c
int trailing_zeros_factorial(int n) {
    int count = 0;
    for (long long p = 5; p <= n; p *= 5) {
        count += (int)(n / p);
    }
    return count;
}
/* Example: 25! has floor(25/5)+floor(25/25) = 5+1 = 6 trailing zeros */
```

### Highest Power of Prime p in n! (Legendre's Theorem)
$$v_p(n!) = \sum_{i=1}^{\infty} \lfloor n / p^i \rfloor$$

---

## 10. MATHEMATICAL INSIGHTS

### Stirling's Approximation
$$n! \approx \sqrt{2\pi n} \left(\frac{n}{e}\right)^n$$

Useful for asymptotic analysis: `log(n!) ≈ n log n - n`

### Factorial in Combinatorics
$$\binom{n}{k} = \frac{n!}{k!(n-k)!}$$

Direct factorial computation for large C(n,k) causes overflow — use multiplicative formula instead.

### Wilson's Theorem
p is prime iff `(p-1)! ≡ -1 (mod p)`  
*Impractical for primality testing but mathematically elegant.*

### Digit Count of n!
$$\text{digits}(n!) = \lfloor \log_{10}(n!) \rfloor + 1 = \lfloor \sum_{i=1}^{n} \log_{10}(i) \rfloor + 1$$

---

## 11. INTERVIEWER FOLLOW-UP QUESTIONS

**Q1: What is 0!?**  
A: 1. By convention, the empty product is 1.

**Q2: What's the maximum n that fits in a 64-bit unsigned int?**  
A: n = 20. 20! = 2,432,902,008,176,640,000 < 2^64. 21! overflows.

**Q3: How many trailing zeros does 100! have?**  
A: `floor(100/5) + floor(100/25) = 20 + 4 = 24`

**Q4: Recursive vs iterative — which is better for embedded?**  
A: Iterative — fixed O(1) stack usage regardless of n. Recursive risks stack overflow.

**Q5: How to compute n! for n > 20 without big integer library?**  
A: Use double (approximate) or implement big integer multiplication on digit arrays.

**Q6: Can you compute n! mod M efficiently?**  
A: Yes — compute iteratively, take mod at each step:  
`result = (result * i) % M`  
This keeps numbers small and avoids overflow.

**Q7: Compute C(n, k) without computing n! directly?**  
A: Use multiplicative formula: `C(n,k) = (n * (n-1) * ... * (n-k+1)) / k!`  
Or Pascal's triangle for repeated queries.

---

## 12. COMMON MISTAKES

| Mistake | Fix |
|---------|-----|
| `0! = 0` | It's 1 — add `if (n == 0) return 1` |
| `int` overflow at n=13 | Use `unsigned long long` |
| No base case in recursion | Always handle `n <= 1` |
| Infinite recursion for n<0 | Guard: `if (n < 0) return 0` |
| `result *= i` where i is int | Cast: `result *= (unsigned long long)i` |

---

## 13. DEBUGGING GUIDE

```c
/* Print intermediate results to spot overflow */
for (int i = 2; i <= n; i++) {
    unsigned long long prev = result;
    result *= (unsigned long long)i;
    if (result < prev) {  /* overflow: result wrapped */
        printf("OVERFLOW at step i=%d\n", i);
        break;
    }
    printf("  %d! = %llu\n", i, result);
}
```

---

## 14. TEST CASES

```c
assert(factorial_iterative(0)  == 1ULL);
assert(factorial_iterative(1)  == 1ULL);
assert(factorial_iterative(5)  == 120ULL);
assert(factorial_iterative(10) == 3628800ULL);
assert(factorial_iterative(12) == 479001600ULL);
assert(factorial_iterative(20) == 2432902008176640000ULL);
assert(factorial_iterative(-1) == 0ULL);  /* error case */

/* Trailing zeros */
assert(trailing_zeros_factorial(5)   == 1);
assert(trailing_zeros_factorial(10)  == 2);
assert(trailing_zeros_factorial(25)  == 6);
assert(trailing_zeros_factorial(100) == 24);
```

---

## 15. PERFORMANCE

| Approach   | n=10 | n=20 | n=50 | n=100 | Notes              |
|------------|------|------|------|-------|--------------------|
| Iterative  | ~10 ns | ~20 ns | N/A | N/A | Overflow at n=21   |
| LUT        | ~1 ns  | ~1 ns  | N/A | N/A | O(1) array lookup  |
| Big Integer| ~10 µs | ~20 µs | ~50 µs | ~100 µs | No overflow  |

**For n ≤ 20:** LUT is always fastest.  
**For n > 20:** Big integer is required.

---

## 16. REAL-WORLD APPLICATIONS

| Domain          | Application                                      |
|-----------------|--------------------------------------------------|
| Cryptography    | Permutation counts in key space analysis         |
| Combinatorics   | C(n,k) for statistical analysis                  |
| Probability     | Counting equally-likely outcomes                 |
| Compilers       | Constant folding of factorial expressions        |
| Embedded        | LUT factorials for DSP coefficient generation    |
| OS              | Scheduling permutation counts                    |
| Networking      | Number of routing table permutations             |
| Math libraries  | Gamma function computation (Γ(n+1) = n!)         |

---

## 17. RELATED PROBLEMS

| Problem               | Relation                          |
|-----------------------|-----------------------------------|
| Trailing zeros in n!  | Legendre's formula for factor 5   |
| Binomial coefficient  | Uses factorial division           |
| Fibonacci             | Related recurrence structure      |
| Catalan numbers       | Uses factorial formula            |
| Permutations/Combinations | Direct factorial application  |
| Modular factorial     | n! mod M for large n             |

---

## 18. FINAL TAKEAWAYS

- `0! = 1` by convention — never forget
- Use `unsigned long long` for n ≤ 20; big integer for n > 20
- Iterative is preferred over recursive for embedded (fixed stack)
- LUT is O(1) and ideal for MCU ROM storage
- Trailing zeros = `sum(floor(n / 5^i))` — very common follow-up
- **Mantra:** start = 1 → multiply 2..n → use uint64 → guard n > 20

---

## 20. ADVANCED EXTENSIONS

### Factorial mod p (Modular Factorial)
```c
unsigned long long factorial_mod(int n, unsigned long long mod) {
    if (n < 0) return 0;
    unsigned long long result = 1;
    for (int i = 2; i <= n; i++) {
        result = (result * (unsigned long long)i) % mod;
    }
    return result;
}
```

### Embedded ROM Table
```c
/* Place factorial LUT in flash/ROM (GCC ARM attribute) */
static const uint64_t fact_lut[21]
    __attribute__((section(".rodata"), aligned(8))) = {
    1ULL, 1ULL, 2ULL, 6ULL, 24ULL, 120ULL, ...
};
```

### Parallel Factorial (Divide-and-Conquer)
```c
/* Compute product(L..R) recursively, parallelizable with pthreads/OpenMP */
unsigned long long range_product(int L, int R) {
    if (L == R) return (unsigned long long)L;
    int mid = (L + R) / 2;
    return range_product(L, mid) * range_product(mid + 1, R);
}
unsigned long long factorial_parallel(int n) {
    if (n <= 1) return 1;
    return range_product(2, n);
}
```
