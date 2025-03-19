#include <stdio.h>

int main() {
    int num, sum = 0, digit;

    printf("Enter a number: ");
    scanf("%d", &num);

    int temp = num; // Store the original number for output later

    while (num != 0) {
        digit = num % 10; // Extract the last digit
        sum += digit;     // Add the digit to the sum
        num /= 10;        // Remove the last digit
    }

    printf("Sum of digits of %d is: %d\n", temp, sum);

    return 0;
}