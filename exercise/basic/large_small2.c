#include <stdio.h>

int main()
{
	int num1, num2,larger, smaller;
	printf("Enter the first number: ");
	scanf("%d",&num1);

	printf("Enter the second number: ");
        scanf("%d",&num2);

	if (num1 == num2) {
	    printf("Both entered number are equal!\n");
	} else {

		larger = (num1 > num2) ? num1 : num2;
		smaller = (num1 < num2) ? num1 : num2;
		printf("Larger number: %d\n", larger);
		printf("Smaller number: %d\n", smaller);
	}
}
