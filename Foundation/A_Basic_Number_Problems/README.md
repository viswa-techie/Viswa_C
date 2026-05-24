# Foundation Layer — A. Basic Number Problems

**Category:** Foundation  
**Layer:** Basic Number Problems  
**Target:** Semiconductor, Embedded, Firmware Interviews  
**Companies:** Qualcomm, Intel, AMD, NXP, Texas Instruments  

---

## Why This Category Matters

Basic number problems form the foundation of all higher-level algorithmic thinking.
Every semiconductor and embedded systems interview starts with these concepts.
Mastery here demonstrates:
- Loop control and termination reasoning
- Modulo and division intuition
- Integer overflow awareness
- Edge-case engineering mindset
- Clean, portable C code

---

## Problems in This Category

| # | Problem            | Core Skill                              | Difficulty | Priority |
|---|--------------------|-----------------------------------------|------------|----------|
| 01 | Prime Checking    | Loop optimization, sqrt reasoning       | Easy       | TIER-1   |
| 02 | Palindrome Number | Digit extraction, reversal              | Easy       | TIER-1   |
| 03 | Reverse Number    | Digit manipulation, overflow            | Easy       | TIER-1   |
| 04 | Armstrong Number  | Power computation, digit loop          | Easy       | TIER-2   |
| 05 | Factorial         | Recursion vs iteration, overflow        | Easy       | TIER-1   |
| 06 | Fibonacci         | DP, memoization, matrix exponentiation  | Medium     | TIER-1   |
| 07 | Digit Sum         | Modulo tricks, digital root             | Easy       | TIER-2   |
| 08 | GCD / LCM         | Euclidean algorithm, overflow           | Easy       | TIER-1   |
| 09 | Divisors          | Loop optimization, sqrt factoring       | Easy       | TIER-1   |

---

## Skills Developed

- **Loops:** for, while, do-while with proper termination
- **Modulo:** `%` behavior for positive and negative numbers
- **Integer manipulation:** extracting digits, building numbers
- **Overflow:** when `int` vs `long long` matters
- **Recursion:** depth, stack safety, memoization
- **Edge cases:** 0, 1, negative, INT_MAX, INT_MIN

---

## Compilation Command

```bash
gcc -Wall -Wextra -Woverflow -std=c11 -O2 -o solution solution.c
```

With sanitizers (development):
```bash
gcc -fsanitize=address,undefined -g -std=c11 -o solution solution.c
```

---

## Quick Navigation

- [01 Prime Checking](./01_Prime_Checking/)
- [02 Palindrome Number](./02_Palindrome_Number/)
- [03 Reverse Number](./03_Reverse_Number/)
- [04 Armstrong Number](./04_Armstrong_Number/)
- [05 Factorial](./05_Factorial/)
- [06 Fibonacci](./06_Fibonacci/)
- [07 Digit Sum](./07_Digit_Sum/)
- [08 GCD / LCM](./08_GCD_LCM/)
- [09 Divisors](./09_Divisors/)
