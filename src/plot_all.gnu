# Gnuplot script to plot all 5 profit datasets on a single graph.
# HOW TO USE:
# 1. Ensure this script is run after the main program has generated all 'results_n.txt' files.
# 2. In your terminal, run the command: gnuplot plot_all.gnu

# --- Script Configuration ---
set title "Comparative Profit Analysis of All Datasets"
set xlabel "Quantity"
set ylabel "Profit"
set grid
set key outside top center
set term wxt size 1000,600

# --- Plot Command ---
plot 'results_1.txt' using 1:4 with lines lw 2 title 'Dataset 1', \
     '' using (242.71):(37516.67) with points pt 7 ps 1.5 notitle, \
     'results_2.txt' using 1:4 with lines lw 2 title 'Dataset 2', \
     '' using (227.67):(27611.62) with points pt 7 ps 1.5 notitle, \
     'results_3.txt' using 1:4 with lines lw 2 title 'Dataset 3', \
     '' using (283.93):(40667.61) with points pt 7 ps 1.5 notitle, \
     'results_4.txt' using 1:4 with lines lw 2 title 'Dataset 4', \
     '' using (282.04):(46324.28) with points pt 7 ps 1.5 notitle, \
     'results_5.txt' using 1:4 with lines lw 2 title 'Dataset 5', \
     '' using (283.74):(41433.63) with points pt 7 ps 1.5 notitle

# Keep the plot window open until it is closed manually
pause -1 "Press Enter or close the window to exit"
