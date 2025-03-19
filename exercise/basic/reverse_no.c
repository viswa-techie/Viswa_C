#include <stdio.h>

int main() {
    int start;

    printf("Enter the starting number: ");
    scanf("%d", &start);

    printf("Numbers in reverse order with a difference of 2:\n");

    while (start >= 1) { // Or any number you want to end at.
        printf("%d ", start);
        start -= 2; // Subtract 2 from the current number
    }

    printf("\n");

    return 0;
}