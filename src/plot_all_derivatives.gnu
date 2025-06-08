set title "Comparative Derivative Analysis of All Datasets"
set xlabel "Quantity"
set ylabel "Slope of Profit (P'(x))"
set grid
set key outside top center
set term wxt size 1000,600

plot 'results_1.txt' using 1:5 with lines lw 2 title 'Dataset 1 Derivative', \
     'results_2.txt' using 1:5 with lines lw 2 title 'Dataset 2 Derivative', \
     'results_3.txt' using 1:5 with lines lw 2 title 'Dataset 3 Derivative', \
     'results_4.txt' using 1:5 with lines lw 2 title 'Dataset 4 Derivative', \
     'results_5.txt' using 1:5 with lines lw 2 title 'Dataset 5 Derivative', \
     0 with lines dt 2 title "P'(x) = 0"

pause -1 "Press Enter or close the window to exit"
