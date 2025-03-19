#include <stdio.h>

int main() {
    long long binary;
    int decimal = 0, i = 0, remainder, power = 1; // power variable added

    printf("Enter a binary number: ");
    scanf("%lld", &binary);

    while (binary != 0) {
        remainder = binary % 10;
        decimal += remainder * power; // Use power instead of pow()
        binary /= 10;
        power *= 2; // Calculate the next power of 2
        i++; // no longer needed but kept if you want to use it for debugging
    }

    printf("Decimal equivalent: %d\n", decimal);

    return 0;
}