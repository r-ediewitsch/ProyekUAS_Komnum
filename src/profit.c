#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// --- Defines and Constants ---
#define MIN_QUANTITY 0.0
#define MAX_QUANTITY 500.0
#define STEP 20
#define MAX_ITERATIONS 100
#define EPSILON 0.0001
#define H 0.001

// --- Function Prototypes ---
double calculateProfit(double x, double a, double b, double c, double d, double e);
double calculateProfitDerivative(double x, double a, double b, double c, double d, double e);
double calculateProfitSecondDerivative(double x, double a, double b, double c, double d, double e);
double newtonRaphson(double x0, double a, double b, double c, double d, double e, int* iter_count);
double simpsonsRule(double x_low, double x_high, double a, double b, double c, double d, double e);
void processDataset(int datasetNum, double a, double b, double c, double d, double e, double* maxProfitQty_out, double* maxProfit_out);
void createMasterProfitPlotScript(const double results[5][2]);
void createMasterDerivativePlotScript(void);

/**
 * @brief Main entry point of the program.
 */
int main() {
    double datasets[5][5] = {
        {276, 0.001, 0.18, 105, 3381},
        {235, 0.0008, 0.22, 107, 4004},
        {266, 0.0005, 0.21, 151, 3901},
        {286, 0.0007, 0.16, 171, 3037},
        {258, 0.0006, 0.14, 197, 3431}
    };

    double results[5][2]; // To store maxProfitQty and maxProfit for each dataset

    // Process each dataset individually and store the results
    for (int i = 0; i < 5; i++) {
        processDataset(i + 1, datasets[i][0], datasets[i][1], datasets[i][2], datasets[i][3], datasets[i][4], &results[i][0], &results[i][1]);
    }

    // After processing all datasets, create the master plot scripts
    createMasterProfitPlotScript(results);
    createMasterDerivativePlotScript();

    return 0;
}

/**
 * @brief Calculates the profit for a given quantity x.
 */
double calculateProfit(double x, double a, double b, double c, double d, double e) {
    double revenue = a * x - b * pow(x, 3);
    double cost = c * x * x + d * exp(0.01 * x) + e;
    return revenue - cost;
}

/**
 * @brief Calculates the first derivative of the profit function.
 */
double calculateProfitDerivative(double x, double a, double b, double c, double d, double e) {
    return (calculateProfit(x + H, a, b, c, d, e) - calculateProfit(x - H, a, b, c, d, e)) / (2 * H);
}

/**
 * @brief Calculates the second derivative of the profit function.
 */
double calculateProfitSecondDerivative(double x, double a, double b, double c, double d, double e) {
    return (calculateProfit(x + H, a, b, c, d, e) - 2 * calculateProfit(x, a, b, c, d, e) + calculateProfit(x - H, a, b, c, d, e)) / (H * H);
}

/**
 * @brief Finds the quantity for maximum profit using the Newton-Raphson method.
 */
double newtonRaphson(double x0, double a, double b, double c, double d, double e, int* iter_count) {
    double x = x0;
    int iterations = 0;
    // This function is now silent to keep the console clean during iteration.
    do {
        double fx = calculateProfitDerivative(x, a, b, c, d, e);
        double dfx = calculateProfitSecondDerivative(x, a, b, c, d, e);
        if (fabs(dfx) < EPSILON) {
            return -1; // Indicate failure
        }
        double x_new = x - fx / dfx;
        if (fabs(x_new - x) < EPSILON) {
            *iter_count = iterations + 1;
            return x_new;
        }
        x = x_new;
        iterations++;
    } while (iterations < MAX_ITERATIONS);
    *iter_count = iterations;
    return x;
}

/**
 * @brief Calculates the total profit over a range using Simpson's 1/3 rule.
 */
double simpsonsRule(double x_low, double x_high, double a, double b, double c, double d, double e) {
    int n = 200; // Using a fixed, reasonable number of intervals
    if (n % 2 != 0) n++;
    double h = (x_high - x_low) / n;
    double sum = calculateProfit(x_low, a, b, c, d, e) + calculateProfit(x_high, a, b, c, d, e);
    for (int i = 1; i < n; i++) {
        double x = x_low + i * h;
        sum += (i % 2 == 0) ? 2 * calculateProfit(x, a, b, c, d, e) : 4 * calculateProfit(x, a, b, c, d, e);
    }
    return (h / 3) * sum;
}

/**
 * @brief Main processing function for a single dataset.
 */
void processDataset(int datasetNum, double a, double b, double c, double d, double e, double* maxProfitQty_out, double* maxProfit_out) {
    char resultsFile[50];
    sprintf(resultsFile, "results_%d.txt", datasetNum);

    printf("\n===============================================\n");
    printf("Analyzing Dataset %d...\n", datasetNum);

    FILE *output = fopen(resultsFile, "w");
    if (!output) {
        printf("Error creating %s\n", resultsFile);
        return;
    }

    int maxIter = 0;
    *maxProfitQty_out = newtonRaphson((MIN_QUANTITY + MAX_QUANTITY) / 2, a, b, c, d, e, &maxIter);
    *maxProfit_out = calculateProfit(*maxProfitQty_out, a, b, c, d, e);
    double totalProfit = simpsonsRule(MIN_QUANTITY, MAX_QUANTITY, a, b, c, d, e);

    // --- Print analysis summary to console ---
    printf("\nMaximum Profit Analysis:\n");
    printf("---------------------------\n");
    printf("Optimal Quantity\t: %.2f\n", *maxProfitQty_out);
    printf("Maximum Profit\t\t: %.2f\n", *maxProfit_out);
    printf("Iterations\t\t: %d\n", maxIter);
    printf("\nTotal Profit (Simpson's Rule):\n");
    printf("---------------------------\n");
    printf("From x=%.2f to x=%.2f\t: %.2f\n", MIN_QUANTITY, MAX_QUANTITY, totalProfit);


    // --- ADDED: Write summary results to the top of the file ---
    fprintf(output, "# Maximum Profit Analysis:\n");
    fprintf(output, "# ---------------------------\n");
    fprintf(output, "# Optimal Quantity\t: %.2f\n", *maxProfitQty_out);
    fprintf(output, "# Maximum Profit\t: %.2f\n", *maxProfit_out);
    fprintf(output, "# Iterations\t\t: %d\n", maxIter);
    fprintf(output, "#\n");
    fprintf(output, "# Total Profit (Simpson's Rule):\n");
    fprintf(output, "# ---------------------------\n");
    fprintf(output, "# From x=%.2f to x=%.2f\t: %.2f\n\n", MIN_QUANTITY, MAX_QUANTITY, totalProfit);

    // Save data points header and data
    fprintf(output, "# Qty      Revenue      Cost         Profit       Derivative\n");
    for (double q = MIN_QUANTITY; q <= MAX_QUANTITY; q += 5) {
        double r = a * q - b * pow(q, 3);
        double cst = c * q * q + d * exp(0.01 * q) + e;
        double p = r - cst;
        double deriv = calculateProfitDerivative(q, a, b, c, d, e);
        fprintf(output, "%-10.2f %-12.2f %-12.2f %-12.2f %-12.2f\n", q, r, cst, p, deriv);
    }
    fclose(output);

    printf("\nDataset %d processed. Results file saved to %s\n", datasetNum, resultsFile);
}

/**
 * @brief Creates a master Gnuplot script to plot all profit curves comparatively.
 */
void createMasterProfitPlotScript(const double results[5][2]) {
    FILE *gp_master = fopen("plot_all_profits.gnu", "w");
    if (!gp_master) {
        printf("Error creating master profit plot script.\n");
        return;
    }

    fprintf(gp_master, "set title \"Comparative Profit Analysis of All Datasets\"\n");
    fprintf(gp_master, "set xlabel \"Quantity\"\n");
    fprintf(gp_master, "set ylabel \"Profit\"\n");
    fprintf(gp_master, "set grid\n");
    fprintf(gp_master, "set key outside top center\n");
    fprintf(gp_master, "set term wxt size 1000,600\n\n");
    fprintf(gp_master, "plot 'results_1.txt' using 1:4 with lines lw 2 title 'Dataset 1', \\\n");
    fprintf(gp_master, "     '' using (%.2f):(%.2f) with points pt 7 ps 1.5 title 'Max Profit 1', \\\n", results[0][0], results[0][1]);
    fprintf(gp_master, "     'results_2.txt' using 1:4 with lines lw 2 title 'Dataset 2', \\\n");
    fprintf(gp_master, "     '' using (%.2f):(%.2f) with points pt 7 ps 1.5 title 'Max Profit 2', \\\n", results[1][0], results[1][1]);
    fprintf(gp_master, "     'results_3.txt' using 1:4 with lines lw 2 title 'Dataset 3', \\\n");
    fprintf(gp_master, "     '' using (%.2f):(%.2f) with points pt 7 ps 1.5 title 'Max Profit 3', \\\n", results[2][0], results[2][1]);
    fprintf(gp_master, "     'results_4.txt' using 1:4 with lines lw 2 title 'Dataset 4', \\\n");
    fprintf(gp_master, "     '' using (%.2f):(%.2f) with points pt 7 ps 1.5 title 'Max Profit 4', \\\n", results[3][0], results[3][1]);
    fprintf(gp_master, "     'results_5.txt' using 1:4 with lines lw 2 title 'Dataset 5', \\\n");
    fprintf(gp_master, "     '' using (%.2f):(%.2f) with points pt 7 ps 1.5 title 'Max Profit 5'\n\n", results[4][0], results[4][1]);
    fprintf(gp_master, "pause -1 \"Press Enter or close the window to exit\"\n");
    fclose(gp_master);

    printf("\nMaster profit plot script saved to plot_all_profits.gnu\n");
}

/**
 * @brief Creates a master Gnuplot script to plot all derivative curves comparatively.
 */
void createMasterDerivativePlotScript(void) {
    FILE *gp_master = fopen("plot_all_derivatives.gnu", "w");
    if (!gp_master) {
        printf("Error creating master derivative plot script.\n");
        return;
    }

    fprintf(gp_master, "set title \"Comparative Derivative Analysis of All Datasets\"\n");
    fprintf(gp_master, "set xlabel \"Quantity\"\n");
    fprintf(gp_master, "set ylabel \"Slope of Profit (P'(x))\"\n");
    fprintf(gp_master, "set grid\n");
    fprintf(gp_master, "set key outside top center\n");
    fprintf(gp_master, "set term wxt size 1000,600\n\n");
    fprintf(gp_master, "plot 'results_1.txt' using 1:5 with lines lw 2 title 'Dataset 1 Derivative', \\\n");
    fprintf(gp_master, "     'results_2.txt' using 1:5 with lines lw 2 title 'Dataset 2 Derivative', \\\n");
    fprintf(gp_master, "     'results_3.txt' using 1:5 with lines lw 2 title 'Dataset 3 Derivative', \\\n");
    fprintf(gp_master, "     'results_4.txt' using 1:5 with lines lw 2 title 'Dataset 4 Derivative', \\\n");
    fprintf(gp_master, "     'results_5.txt' using 1:5 with lines lw 2 title 'Dataset 5 Derivative', \\\n");
    fprintf(gp_master, "     0 with lines dt 2 title \"P'(x) = 0\"\n\n");
    fprintf(gp_master, "pause -1 \"Press Enter or close the window to exit\"\n");
    fclose(gp_master);

    printf("Master derivative plot script saved to plot_all_derivatives.gnu\n");
}