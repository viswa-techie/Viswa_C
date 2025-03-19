#include <stdio.h>

int multiply(int a, int b) {
    if (b == 0) {
        return 0; // Base case: anything multiplied by 0 is 0
    }

    if (b > 0) {
        return a + multiply(a, b - 1); // Recursive addition
    } else {
        return -multiply(a, -b); // Handle negative b
    }
}

int main() {
    int num1, num2, result;

    printf("Enter two positive integers: ");
    scanf("%d %d", &num1, &num2);

    if (num1 < 0 || num2 < 0) {
        printf("Please enter positive integers.\n");
        return 1;
    }

    result = multiply(num1, num2);
    printf("Result: %d\n", result);

    return 0;
}