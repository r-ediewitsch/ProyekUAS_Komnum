set title 'Profit Analysis - Dataset 3'
set xlabel 'Quantity'
set ylabel 'Amount'
set grid
plot 'results_3.txt' using 1:2 title 'Revenue' with lines, \
     'results_3.txt' using 1:3 title 'Cost' with lines, \
     'results_3.txt' using 1:4 title 'Profit' with lines
pause -1
