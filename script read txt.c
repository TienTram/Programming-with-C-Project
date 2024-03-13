// Include necessary libraries
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

    // Read input from "Data.txt"
    FILE *inputFile = fopen("Data.txt", "r");
    if (inputFile == NULL) {
        printf("Error: Cannot open Data.txt.\n");
        return 1;
    }

    // Open the "Result.txt" file for writing
    FILE *outputFile = fopen("Result.txt", "w");
    if (outputFile == NULL) {
        printf("Error: Cannot open Result.txt.\n");
        fclose(inputFile);
        return 1;
    }

    // Read values from Data.txt and process each line
    while (fscanf(inputFile, "%lf %lf %lf %lf", &a1, &a2, &y0, &y1) == 4) {
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

        // Save results to "Result.txt"
        if (lambda1 == lambda2) {
            fprintf(outputFile, "2 %.2f %.2f\n", C1, C2);
        } else if (lambda1 * lambda2 > 0) {
            fprintf(outputFile, "1 %.2f %.2f\n", C1, C2);
        } else {
            fprintf(outputFile, "3 %.2f %.2f\n", C1, theta);
        }
    }

    // Close the input and output files
    fclose(inputFile);
    fclose(outputFile);

    // Return success
    return 0;
}
