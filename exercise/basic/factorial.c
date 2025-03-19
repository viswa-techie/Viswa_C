#include <stdio.h>

int main() {
    long int num, factorial = 1;

    printf("Enter a non-negative integer: ");
    scanf("%d", &num);

    if (num < 0) {
        printf("Factorial is not defined for negative numbers.\n");
    } else {
        for (int i = 1; i <= num; i++) {
            factorial *= i;
        }
        printf("Factorial of %ld is: %ld\n", num, factorial);
    }

    return 0;
}