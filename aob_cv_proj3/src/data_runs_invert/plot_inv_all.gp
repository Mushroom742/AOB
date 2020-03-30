set term png size 1900,1000

set grid
 
set ylabel "Latency in cycles"
set xlabel "Frame number"

#set yrange [0:6e6]

set multiplot layout 2,2 rowsfirst 

set title "O1 compiler optimization"
plot "O1/data/inv_baseline" w lp t "Baseline",\
	"O1/data/inv_v1" w lp t "V1",\
	"O1/data/inv_v2" w lp t "V2"

set title "O2 compiler optimization"
plot "O2/data/inv_baseline" w lp t "Baseline",\
	"O2/data/inv_v1" w lp t "V1",\
	"O2/data/inv_v2" w lp t "V2"

set title "O3 compiler optimization"
plot "O3/data/inv_baseline" w lp t "Baseline",\
	"O3/data/inv_v1" w lp t "V1",\
	"O3/data/inv_v2" w lp t "V2"

set title "Ofast compiler optimization"
plot "Ofast/data/inv_baseline" w lp t "Baseline",\
	"Ofast/data/inv_v1" w lp t "V1",\
	"Ofast/data/inv_v2" w lp t "V2"

unset multiplot