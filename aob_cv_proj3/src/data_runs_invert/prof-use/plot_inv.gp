set term png size 1900,1000

set grid
 
set ylabel "Latency in cycles"
set xlabel "Frame number"

set yrange [0:6e6]

set title "Comparison between different implementaions of the color inversion"

plot "data/inv_baseline" w lp,\
	"data/inv_one_loop" w lp,\
	"data/inv_bitflip" w lp,\
	"data/inv_unroll_8" w lp,\
	"data/inv_unroll_8_bitflip" w lp,\
	"data/inv_parallel" w lp,\
	"data/inv_parallel_bitflip" w lp,\
	"data/inv_unroll_8_parallel" w lp,\
	"data/inv_unroll_8_parallel_bitflip" w lp
