#include <stdio.h>

int main() {
    int i = 1; // Initialize the counter

    do {
        printf("%d ", i); // Print the current value of i
        i++; // Increment i
    } while (i <= 10); // Condition: loop continues as long as i is less than or equal to 10

    printf("\n"); // Print a newline for formatting

    return 0;
}