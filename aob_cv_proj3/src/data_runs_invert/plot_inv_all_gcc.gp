set term png size 2900,2000

set grid
 
set ylabel "Latency in cycles"
set xlabel "Frame number"

#set yrange [0:6e6]

set multiplot layout 3,2 rowsfirst 

set title "O1 compiler optimization"
plot "gcc_O1/data/inv_baseline" w lp t "Baseline",\
	"gcc_O1/data/inv_one_loop" w lp t "One loop",\
	"gcc_O1/data/inv_bitflip" w lp t "Bitflip",\
	"gcc_O1/data/inv_unroll_8" w lp t "Unroll 8",\
	"gcc_O1/data/inv_unroll_8_bitflip" w lp t "Unroll 8 bitflip",\
	"gcc_O1/data/inv_parallel" w lp t "Parallel",\
	"gcc_O1/data/inv_parallel_bitflip" w lp t "Parallel bitflip",\
	"gcc_O1/data/inv_unroll_8_parallel" w lp t "Unroll 8 parallel",\
	"gcc_O1/data/inv_unroll_8_parallel_bitflip" w lp t "Unroll 8 parallel bitflip"
	

set title "O2 compiler optimization"
plot "gcc_O2/data/inv_baseline" w lp t "Baseline",\
	"gcc_O2/data/inv_one_loop" w lp t "One loop",\
	"gcc_O2/data/inv_bitflip" w lp t "Bitflip",\
	"gcc_O2/data/inv_unroll_8" w lp t "Unroll 8",\
	"gcc_O2/data/inv_unroll_8_bitflip" w lp t "Unroll 8 bitflip",\
	"gcc_O2/data/inv_parallel" w lp t "Parallel",\
	"gcc_O2/data/inv_parallel_bitflip" w lp t "Parallel bitflip",\
	"gcc_O2/data/inv_unroll_8_parallel" w lp t "Unroll 8 parallel",\
	"gcc_O2/data/inv_unroll_8_parallel_bitflip" w lp t "Unroll 8 parallel bitflip"

set title "O3 compiler optimization"
plot "gcc_O3/data/inv_baseline" w lp t "Baseline",\
	"gcc_O3/data/inv_one_loop" w lp t "One loop",\
	"gcc_O3/data/inv_bitflip" w lp t "Bitflip",\
	"gcc_O3/data/inv_unroll_8" w lp t "Unroll 8",\
	"gcc_O3/data/inv_unroll_8_bitflip" w lp t "Unroll 8 bitflip",\
	"gcc_O3/data/inv_parallel" w lp t "Parallel",\
	"gcc_O3/data/inv_parallel_bitflip" w lp t "Parallel bitflip",\
	"gcc_O3/data/inv_unroll_8_parallel" w lp t "Unroll 8 parallel",\
	"gcc_O3/data/inv_unroll_8_parallel_bitflip" w lp t "Unroll 8 parallel bitflip"

set title "Ofast compiler optimization"
plot "gcc_Ofast/data/inv_baseline" w lp t "Baseline",\
	"gcc_Ofast/data/inv_one_loop" w lp t "One loop",\
	"gcc_Ofast/data/inv_bitflip" w lp t "Bitflip",\
	"gcc_Ofast/data/inv_unroll_8" w lp t "Unroll 8",\
	"gcc_Ofast/data/inv_unroll_8_bitflip" w lp t "Unroll 8 bitflip",\
	"gcc_Ofast/data/inv_parallel" w lp t "Parallel",\
	"gcc_Ofast/data/inv_parallel_bitflip" w lp t "Parallel bitflip",\
	"gcc_Ofast/data/inv_unroll_8_parallel" w lp t "Unroll 8 parallel",\
	"gcc_Ofast/data/inv_unroll_8_parallel_bitflip" w lp t "Unroll 8 parallel bitflip"

set title "fprofile-use compiler optimization"
plot "fprofile-use/data/inv_baseline" w lp t "Baseline",\
	"fprofile-use/data/inv_one_loop" w lp t "One loop",\
	"fprofile-use/data/inv_bitflip" w lp t "Bitflip",\
	"fprofile-use/data/inv_unroll_8" w lp t "Unroll 8",\
	"fprofile-use/data/inv_unroll_8_bitflip" w lp t "Unroll 8 bitflip",\
	"fprofile-use/data/inv_parallel" w lp t "Parallel",\
	"fprofile-use/data/inv_parallel_bitflip" w lp t "Parallel bitflip",\
	"fprofile-use/data/inv_unroll_8_parallel" w lp t "Unroll 8 parallel",\
	"fprofile-use/data/inv_unroll_8_parallel_bitflip" w lp t "Unroll 8 parallel bitflip"

unset multiplot