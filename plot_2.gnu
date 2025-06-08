set title 'Profit Analysis - Dataset 2'
set xlabel 'Quantity'
set ylabel 'Amount'
set grid
plot 'results_2.txt' using 1:2 title 'Revenue' with lines, \
     'results_2.txt' using 1:3 title 'Cost' with lines, \
     'results_2.txt' using 1:4 title 'Profit' with lines
pause -1
