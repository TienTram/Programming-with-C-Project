#include <stdio.h>
#include <math.h>
#include <complex.h>

// Function to find constants C1, C2, and theta for the zero-input response
void find_C(double y0, double y1, double lambda1, double lambda2, double *C1, double *C2, double *theta) {
    // If lambdas are equal (real, repeated roots case)
    if (lambda1 == lambda2) {
        *C1 = y0;
        *C2 = y1 - lambda1 * y0;
        *theta = 0;
    }
    // If lambdas have the same sign (real, distinct roots case)
    else if (lambda1 * lambda2 > 0) {
        *C1 = (y1 - lambda2 * y0) / (lambda1 - lambda2);
        *C2 = (y0 - *C1) / lambda2;
        *theta = 0;
    }
    // If lambdas have different signs (complex conjugate roots case)
    else {
        double beta = sqrt(-lambda1 * lambda2);
        double C = sqrt(y0 * y0 + y1 * y1 / beta / beta);
        *theta = atan2(y1 / beta, y0);
        *C1 = C * cos(*theta);
        *C2 = C * sin(*theta);
    }
}

int main() {
    // Declare variables for coefficients and initial conditions
    double a1, a2, y0, y1;

    // Prompt user for input
    printf("Enter the coefficients of the second-order system:\n");
    printf("a1: ");
    scanf("%lf", &a1);
    printf("a2: ");
    scanf("%lf", &a2);
    printf("Enter the initial conditions:\n");
    printf("y(0): ");
    scanf("%lf", &y0);
    printf("y'(0): ");
    scanf("%lf", &y1);

    // Calculate the discriminant
    double discriminant = a1 * a1 - 4 * a2;
    double lambda1, lambda2;

    // Check the type of roots based on the discriminant value
    if (discriminant > 0) {
        // Real, distinct roots case
        lambda1 = (-a1 + sqrt(discriminant)) / 2;
        lambda2 = (-a1 - sqrt(discriminant)) / 2;
    } else if (discriminant == 0) {
        // Real, repeated roots case
        lambda1 = lambda2 = -a1 / 2;
    } else {
        // Complex conjugate roots case
        double realPart = -a1 / 2;
        double imaginaryPart = sqrt(-discriminant) / 2;
        lambda1 = realPart + imaginaryPart * I;
        lambda2 = realPart - imaginaryPart * I;
    }

    // Declare variables for constants C1, C2, and theta
    double C1, C2, theta;

    // Call find_C function to determine C1, C2, and theta
    find_C(y0, y1, creal(lambda1), creal(lambda2), &C1, &C2, &theta);

    // Print the zero-input response
    printf("The zero-input response is:\n");
    if (lambda1 == lambda2) {
        printf("y(t) = (%.2f + %.2ft)e^(%.2ft)\n", C1, C2, creal(lambda1));
    } else if (lambda1 * lambda2 > 0) {
        // Real, distinct roots case
        printf("y(t) = %.2fe^(%.2ft) + %.2fe^(%.2ft)\n", C1, creal(lambda1), C2, creal(lambda2));
    } else {
        // Complex conjugate roots case
        printf("y(t) = %.2f cos(%.2ft + %.2f)\n", C1, fabs(creal(lambda1)), theta);
    }

    // Return success
    return 0;
}
