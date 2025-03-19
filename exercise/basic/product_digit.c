#include <stdio.h>

int main() {
    int num, product = 1, digit;

    printf("Enter a number: ");
    scanf("%d", &num);

    int temp = num; // Store the original number for output

    while (num != 0) {
        digit = num % 10;
        product *= digit;
        num /= 10;
    }

    printf("Product of digits of %d is: %d\n", temp, product);

    return 0;
}