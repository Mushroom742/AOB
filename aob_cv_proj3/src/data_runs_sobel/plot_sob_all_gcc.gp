set term png size 2900,2000

set grid
 
set ylabel "Latency in cycles"
set xlabel "Frame number"

#set yrange [0:6e6]

set multiplot layout 2,2 rowsfirst 

set title "O1 compiler optimization"
plot "gcc_O1/data/sob_baseline" w lp t "Baseline",\
	"gcc_O1/data/sob_unroll" w lp t "Unroll",\
	"gcc_O1/data/sob_unroll_full" w lp t "Unroll full"
	
set title "O2 compiler optimization"
plot "gcc_O2/data/sob_baseline" w lp t "Baseline",\
	"gcc_O2/data/sob_unroll" w lp t "Unroll",\
	"gcc_O2/data/sob_unroll_full" w lp t "Unroll full"

set title "O3 compiler optimization"
plot "gcc_O3/data/sob_baseline" w lp t "Baseline",\
	"gcc_O3/data/sob_unroll" w lp t "Unroll",\
	"gcc_O3/data/sob_unroll_full" w lp t "Unroll full"

set title "Ofast compiler optimization"
plot "gcc_Ofast/data/sob_baseline" w lp t "Baseline",\
	"gcc_Ofast/data/sob_unroll" w lp t "Unroll",\
	"gcc_Ofast/data/sob_unroll_full" w lp t "Unroll full"

unset multiplot