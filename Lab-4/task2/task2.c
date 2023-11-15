#include <stdio.h>
#include <stdlib.h>

/* Function declarations */
void add(int a, int b);
void subtract(int a, int b);
void multiply(int a, int b);
void divide(int a, int b);

/* Function pointer array */
void (*operations[4])(int, int) = {add, subtract, multiply, divide};

int main(void) {
    int a = 10;  // predefined integer a
    int b = 5;   // predefined integer b
    char operation;

    printf("Enter operation (0=add, 1=subtract, 2=multiply, 3=divide, 4=exit): ");
    scanf(" %c", &operation);

    /* Convert character input to integer index */
    int opIndex = operation - '0';

    /* Ensure index is within bounds for the operations array */
    if (opIndex < 0 || opIndex > 3) {
        printf("Invalid operation. Exiting program.\n");
        return 0;
    }

    /* Call the function corresponding to the entered character */
    operations[opIndex](a, b);

    return 0;
}

/* Define functions here */
void add(int a, int b) { printf("Adding 'a' and 'b': %d\n", a + b); }
void subtract(int a, int b) { printf("Subtracting 'b' from 'a': %d\n", a - b); }
void multiply(int a, int b) { printf("Multiplying 'a' and 'b': %d\n", a * b); }
void divide(int a, int b) {
    if (b != 0) {
        printf("Dividing 'a' by 'b': %d\n", a / b);
    } else {
        printf("Error: Division by zero\n");
    }
}
