#!/bin/bash

#
ffmpeg -version >> "/dev/null"

if [ $? -ne 0 ] 
then
    echo "Error: Cannot invoke FFMPEG"
    exit 1
fi

#
gnuplot --version >> "/dev/null"

if [ $? -ne 0 ] 
then
    echo "Error: Cannot invoke GNUPLOT"
    exit 1
fi

#
echo -e "[BEGIN]\n"

if [ -f "in/input.raw" ]
then
    #If input file exists nothing to be done
    echo "Input file exists"
    echo
else    
    #Convert mp4 to raw format
    echo "Converting video to raw format"
    echo
    
    ./cvt_vid.sh v2r "in/input.mp4" "in/input.raw" >> /dev/null 2>> /dev/null
fi

#
dir="data_runs_sobel"
mkdir -p $dir $dir"/logs"

#
cp "plot_sob_all_gcc.gp" $dir
cp "plot_sob_all_icc.gp" $dir

#Compiler optimizations
for cc in "gcc" "icc"
do
    #
    echo "Running with compiler: "$cc

    #
    for opt in "O1" "O2" "O3" "Ofast"
    do
        #
        echo "Running with flag: "$opt
        
        #
        mkdir -p $dir"/"$cc"_"$opt
        mkdir -p $dir"/"$cc"_"$opt"/data"

        #
        cp "plot_sob.gp" $dir"/"$cc"_"$opt
        
        #Going through invert code variants
        for variant in sob_baseline
        do
    	#
    	echo -e "\tVariant: "$variant
    	
    	#Compile variant
    	make $variant CC=$cc O=$opt >> $dir"/logs/compile.log" 2>> $dir"/logs/compile_err.log"
    	
    	#Run & select run number & cycles 
    	./sobel in/input.raw sout/output.raw | cut -d';' -f1,3 > $dir"/"$cc"_"$opt"/data/"$variant
    	
    	#Convert raw file into mp4 video
    	./cvt_vid.sh r2v "sout/output.raw" "sout/output_"$variant".mp4" >> $dir"/logs/cvt.log" 2>> $dir"/logs/cvt_err.log"

    	echo
        done

        #
        cd $dir"/"$cc"_"$opt

        #Generate the plot
        gnuplot -c "plot_sob.gp" > "plot_"$opt".png"

        cd ../..

        echo
    done
done

#
cd $dir

gnuplot -c "plot_sob_all_gcc.gp" > "plot_all_gcc.png" 
gnuplot -c "plot_sob_all_icc.gp" > "plot_all_icc.png" 

cd ..

#
make clean

#
echo -e "\n[DONE]"
