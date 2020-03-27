set term png size 1900,1000

set grid
 
set ylabel "Latency in cycles"
set xlabel "Frame number"

#set yrange [0:6e6]

set multiplot layout 2,2 rowsfirst 

set title "O1 compiler optimization"
plot "O1/data/inv_baseline" w lp t "Baseline"

set title "O2 compiler optimization"
plot "O2/data/inv_baseline" w lp t "Baseline"

set title "O3 compiler optimization"
plot "O3/data/inv_baseline" w lp t "Baseline"

set title "Ofast compiler optimization"
plot "Ofast/data/inv_baseline" w lp t "Baseline"

unset multiplot