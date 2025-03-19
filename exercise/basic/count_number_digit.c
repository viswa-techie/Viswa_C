#include <stdio.h>

int main() {
    int num, count = 0;

    printf("Enter an integer: ");
    scanf("%d", &num);

    if (num == 0) {
        count = 1; // Special case: 0 has one digit
    } else {
        if(num < 0){
            num = -num; // make the number positive, so the count is correct.
        }
        while (num != 0) {
            num /= 10; // Remove the last digit
            count++;     // Increment the count
        }
    }

    printf("Number of digits: %d\n", count);

    return 0;
}