set term png size 1900,1000

set grid
 
set ylabel "Latency in cycles"
set xlabel "Frame number"

set yrange [0:6e9]

set title "Comparison between different implementaions of the color inversion"

plot "data/sob_baseline" w lp,\
	"data/sob_unroll" w lp,\
	"data/sob_unroll_full" w lp
