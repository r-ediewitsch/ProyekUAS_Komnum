#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define MIN_QUANTITY 0.0
#define MAX_QUANTITY 500.0
#define STEP 20
#define MAX_ITERATIONS 100
#define EPSILON 0.0001
#define H 0.001

double calculateProfit(double x, double a, double b, double c, double d, double e) {
    double revenue = a * x - b * pow(x, 3);
    double cost = c * x * x + d * exp(0.01 * x) + e;
    return revenue - cost;
}

double calculateProfitDerivative(double x, double a, double b, double c, double d, double e) {
    return (calculateProfit(x + H, a, b, c, d, e) - calculateProfit(x - H, a, b, c, d, e)) / (2 * H);
}

double calculateProfitSecondDerivative(double x, double a, double b, double c, double d, double e) {
    return (calculateProfit(x + H, a, b, c, d, e) - 2 * calculateProfit(x, a, b, c, d, e) + calculateProfit(x - H, a, b, c, d, e)) / (H * H);
}

double newtonRaphson(double x0, double a, double b, double c, double d, double e, int* iter_count) {
    double x = x0;
    int iterations = 0;

    printf("\nNewton-Raphson Iterations (Max Profit):\n");
    printf("---------------------------------------------\n");
    printf("Iter |    x     |    P'(x)    |    P''(x)   \n");
    printf("---------------------------------------------\n");

    do {
        double fx = calculateProfitDerivative(x, a, b, c, d, e);
        double dfx = calculateProfitSecondDerivative(x, a, b, c, d, e);

        if (fabs(dfx) < EPSILON) {
            printf("Method failed - second derivative too small\n");
            return -1;
        }

        double x_new = x - fx / dfx;
        printf("%3d  | %8.4f | %9.6f | %9.6f\n", iterations, x, fx, dfx);

        if (fabs(x_new - x) < EPSILON) break;

        x = x_new;
        iterations++;
    } while (iterations < MAX_ITERATIONS);

    *iter_count = iterations;
    return x;
}

double simpsonsRule(double x_low, double x_high, double a, double b, double c, double d, double e, int n) {
    if (n % 2 != 0) n++; // Ensure n is even
    double h = (x_high - x_low) / n;
    double sum = calculateProfit(x_low, a, b, c, d, e) + calculateProfit(x_high, a, b, c, d, e);

    for (int i = 1; i < n; i++) {
        double x = x_low + i * h;
        sum += (i % 2 == 0) ? 2 * calculateProfit(x, a, b, c, d, e) : 4 * calculateProfit(x, a, b, c, d, e);
    }

    return (h / 3) * sum;
}

void displayCoefficients(double a, double b, double c, double d, double e) {
    printf("\nCoefficients Table:\n");
    printf("+---+--------+\n");
    printf("| a | %6.2f |\n", a);
    printf("| b | %6.4f |\n", b);
    printf("| c | %6.2f |\n", c);
    printf("| d | %6.2f |\n", d);
    printf("| e | %6.2f |\n", e);
    printf("+---+--------+\n");
}

void saveMetadataToFile(FILE *fp, double a, double b, double c, double d, double e,
                       double maxProfitQty, double maxProfit, double totalProfit) {
    fprintf(fp, "Coefficients:\n");
    fprintf(fp, "a = %.2f\n", a);
    fprintf(fp, "b = %.4f\n", b);
    fprintf(fp, "c = %.2f\n", c);
    fprintf(fp, "d = %.2f\n", d);
    fprintf(fp, "e = %.2f\n\n", e);
    
    fprintf(fp, "Maximum Profit Analysis:\n");
    fprintf(fp, "---------------------------\n");
    fprintf(fp, "Optimal Quantity\t: %.2f\n", maxProfitQty);
    fprintf(fp, "Maximum Profit\t\t: %.2f\n\n", maxProfit);
    
    fprintf(fp, "Total Profit (Simpson's Rule):\n");
    fprintf(fp, "---------------------------\n");
    fprintf(fp, "From x=%.2f to x=%.2f\t: %.2f\n\n", MIN_QUANTITY, MAX_QUANTITY, totalProfit);
    
    fprintf(fp, "Data Points:\n");
    fprintf(fp, "%-10s %-12s %-12s %-12s\n", "Quantity", "Revenue", "Cost", "Profit");
    fprintf(fp, "------------------------------------------------\n");
}

void processDataset(int datasetNum, double a, double b, double c, double d, double e) {
    char resultsFile[50], plotFile[50];
    sprintf(resultsFile, "results_%d.txt", datasetNum);
    sprintf(plotFile, "plot_%d.gnu", datasetNum);

    printf("\n===============================================\n");
    printf("Analyzing Dataset %d\n", datasetNum);
    printf("===============================================\n");

    displayCoefficients(a, b, c, d, e);

    FILE *output = fopen(resultsFile, "w");
    if (!output) {
        printf("Error creating %s\n", resultsFile);
        return;
    }

    // Find maximum profit
    int maxIter = 0;
    double maxProfitQty = newtonRaphson((MIN_QUANTITY + MAX_QUANTITY) / 2, a, b, c, d, e, &maxIter);
    double maxProfit = calculateProfit(maxProfitQty, a, b, c, d, e);

    // Calculate total profit
    int n = 100;
    double totalProfit = simpsonsRule(MIN_QUANTITY, MAX_QUANTITY, a, b, c, d, e, n);

    printf("\nMaximum Profit Analysis:\n");
    printf("---------------------------\n");
    printf("Optimal Quantity\t: %.2f\n", maxProfitQty);
    printf("Maximum Profit\t\t: %.2f\n", maxProfit);
    printf("Iterations\t\t: %d\n", maxIter);

    printf("\nTotal Profit (Simpson's Rule):\n");
    printf("---------------------------\n");
    printf("From x=%.2f to x=%.2f\t: %.2f\n", MIN_QUANTITY, MAX_QUANTITY, totalProfit);

    saveMetadataToFile(output, a, b, c, d, e, maxProfitQty, maxProfit, totalProfit);

    printf("\n+----------+------------+------------+------------+\n");
    printf("| Quantity |   Revenue  |    Cost    |   Profit   |\n");
    printf("+----------+------------+------------+------------+\n");

    for (double q = MIN_QUANTITY; q <= MAX_QUANTITY; q += STEP) {
        double r = a * q - b * pow(q, 3);
        double cst = c * q * q + d * exp(0.01 * q) + e;
        double p = r - cst;
        printf("| %8.2f | %10.2f | %10.2f | %10.2f |\n", q, r, cst, p);
        fprintf(output, "%-10.2f %-12.2f %-12.2f %-12.2f\n", q, r, cst, p);
    }
    printf("+----------+------------+------------+------------+\n");

    fclose(output);

    FILE *gp = fopen(plotFile, "w");
    fprintf(gp, "set title 'Profit Analysis - Dataset %d'\n", datasetNum);
    fprintf(gp, "set xlabel 'Quantity'\n");
    fprintf(gp, "set ylabel 'Amount'\n");
    fprintf(gp, "set grid\n");
    fprintf(gp, "plot '%s' using 1:2 title 'Revenue' with lines, \\\n", resultsFile);
    fprintf(gp, "     '%s' using 1:3 title 'Cost' with lines, \\\n", resultsFile);
    fprintf(gp, "     '%s' using 1:4 title 'Profit' with lines\n", resultsFile);
    fprintf(gp, "pause -1\n");
    fclose(gp);

    printf("\nResults saved to %s\n", resultsFile);
    printf("Visualize with: gnuplot %s\n", plotFile);
}

int main() {
    double datasets[5][5] = {
        {276, 0.001, 0.18, 105, 3381},
        {235, 0.0008, 0.22, 107, 4004},
        {266, 0.0005, 0.21, 151, 3901},
        {286, 0.0007, 0.16, 171, 3037},
        {258, 0.0006, 0.14, 197, 3431}
    };

    for (int i = 0; i < 5; i++) {
        processDataset(i + 1, datasets[i][0], datasets[i][1], datasets[i][2], datasets[i][3], datasets[i][4]);
    }

    return 0;
}