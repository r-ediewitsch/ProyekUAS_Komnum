set title "Comparative Profit Analysis of All Datasets"
set xlabel "Quantity"
set ylabel "Profit"
set grid
set key outside top center
set term wxt size 1000,600

plot 'results_1.txt' using 1:4 with lines lw 2 title 'Dataset 1', \
     '' using (242.71):(37516.67) with points pt 7 ps 1.5 title 'Max Profit 1', \
     'results_2.txt' using 1:4 with lines lw 2 title 'Dataset 2', \
     '' using (227.67):(27611.62) with points pt 7 ps 1.5 title 'Max Profit 2', \
     'results_3.txt' using 1:4 with lines lw 2 title 'Dataset 3', \
     '' using (283.93):(40667.61) with points pt 7 ps 1.5 title 'Max Profit 3', \
     'results_4.txt' using 1:4 with lines lw 2 title 'Dataset 4', \
     '' using (282.04):(46324.28) with points pt 7 ps 1.5 title 'Max Profit 4', \
     'results_5.txt' using 1:4 with lines lw 2 title 'Dataset 5', \
     '' using (283.74):(41433.63) with points pt 7 ps 1.5 title 'Max Profit 5'

pause -1 "Press Enter or close the window to exit"
