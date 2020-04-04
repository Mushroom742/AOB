set term png size 2900,2000

set grid
 
set ylabel "Latency in cycles"
set xlabel "Frame number"

#set yrange [0:6e6]

set multiplot layout 3,2 rowsfirst 

set title "O1 compiler optimization"
plot "icc_O1/data/inv_baseline" w lp t "Baseline",\
	"icc_O1/data/inv_one_loop" w lp t "One loop",\
	"icc_O1/data/inv_bitflip" w lp t "Bitflip",\
	"icc_O1/data/inv_unroll_8" w lp t "Unroll 8",\
	"icc_O1/data/inv_unroll_8_bitflip" w lp t "Unroll 8 bitflip",\
	"icc_O1/data/inv_parallel" w lp t "Parallel",\
	"icc_O1/data/inv_parallel_bitflip" w lp t "Parallel bitflip",\
	"icc_O1/data/inv_unroll_8_parallel" w lp t "Unroll 8 parallel",\
	"icc_O1/data/inv_unroll_8_parallel_bitflip" w lp t "Unroll 8 parallel bitflip"
	

set title "O2 compiler optimization"
plot "icc_O2/data/inv_baseline" w lp t "Baseline",\
	"icc_O2/data/inv_one_loop" w lp t "One loop",\
	"icc_O2/data/inv_bitflip" w lp t "Bitflip",\
	"icc_O2/data/inv_unroll_8" w lp t "Unroll 8",\
	"icc_O2/data/inv_unroll_8_bitflip" w lp t "Unroll 8 bitflip",\
	"icc_O2/data/inv_parallel" w lp t "Parallel",\
	"icc_O2/data/inv_parallel_bitflip" w lp t "Parallel bitflip",\
	"icc_O2/data/inv_unroll_8_parallel" w lp t "Unroll 8 parallel",\
	"icc_O2/data/inv_unroll_8_parallel_bitflip" w lp t "Unroll 8 parallel bitflip"

set title "O3 compiler optimization"
plot "icc_O3/data/inv_baseline" w lp t "Baseline",\
	"icc_O3/data/inv_one_loop" w lp t "One loop",\
	"icc_O3/data/inv_bitflip" w lp t "Bitflip",\
	"icc_O3/data/inv_unroll_8" w lp t "Unroll 8",\
	"icc_O3/data/inv_unroll_8_bitflip" w lp t "Unroll 8 bitflip",\
	"icc_O3/data/inv_parallel" w lp t "Parallel",\
	"icc_O3/data/inv_parallel_bitflip" w lp t "Parallel bitflip",\
	"icc_O3/data/inv_unroll_8_parallel" w lp t "Unroll 8 parallel",\
	"icc_O3/data/inv_unroll_8_parallel_bitflip" w lp t "Unroll 8 parallel bitflip"

set title "Ofast compiler optimization"
plot "icc_Ofast/data/inv_baseline" w lp t "Baseline",\
	"icc_Ofast/data/inv_one_loop" w lp t "One loop",\
	"icc_Ofast/data/inv_bitflip" w lp t "Bitflip",\
	"icc_Ofast/data/inv_unroll_8" w lp t "Unroll 8",\
	"icc_Ofast/data/inv_unroll_8_bitflip" w lp t "Unroll 8 bitflip",\
	"icc_Ofast/data/inv_parallel" w lp t "Parallel",\
	"icc_Ofast/data/inv_parallel_bitflip" w lp t "Parallel bitflip",\
	"icc_Ofast/data/inv_unroll_8_parallel" w lp t "Unroll 8 parallel",\
	"icc_Ofast/data/inv_unroll_8_parallel_bitflip" w lp t "Unroll 8 parallel bitflip"

set title "prof-use compiler optimization"
plot "prof-use/data/inv_baseline" w lp t "Baseline",\
	"prof-use/data/inv_one_loop" w lp t "One loop",\
	"prof-use/data/inv_bitflip" w lp t "Bitflip",\
	"prof-use/data/inv_unroll_8" w lp t "Unroll 8",\
	"prof-use/data/inv_unroll_8_bitflip" w lp t "Unroll 8 bitflip",\
	"prof-use/data/inv_parallel" w lp t "Parallel",\
	"prof-use/data/inv_parallel_bitflip" w lp t "Parallel bitflip",\
	"prof-use/data/inv_unroll_8_parallel" w lp t "Unroll 8 parallel",\
	"prof-use/data/inv_unroll_8_parallel_bitflip" w lp t "Unroll 8 parallel bitflip"

unset multiplot