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
dir="data_runs_invert"
mkdir -p $dir $dir"/logs"

#
cp "plot_inv_all_gcc.gp" $dir
cp "plot_inv_all_icc.gp" $dir

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
        cp "plot_inv.gp" $dir"/"$cc"_"$opt
        
        #Going through invert code variants
        for variant in inv_baseline inv_one_loop inv_bitflip inv_unroll_8 inv_unroll_8_bitflip \
                        inv_parallel inv_parallel_bitflip inv_unroll_8_parallel inv_unroll_8_parallel_bitflip
        do
    	#
    	echo -e "\tVariant: "$variant
    	
    	#Compile variant
    	make $variant CC=$cc O=$opt >> $dir"/logs/compile.log" 2>> $dir"/logs/compile_err.log"
    	
    	#Run & select run number & cycles 
    	./invert in/input.raw iout/output.raw | cut -d';' -f1,3 > $dir"/"$cc"_"$opt"/data/"$variant
    	
    	#Convert raw file into mp4 video
    	./cvt_vid.sh r2v "iout/output.raw" "iout/output_"$variant".mp4" >> $dir"/logs/cvt.log" 2>> $dir"/logs/cvt_err.log"

    	echo
        done

        #
        cd $dir"/"$cc"_"$opt

        #Generate the plot
        gnuplot -c "plot_inv.gp" > "plot_"$opt".png"

        cd ../..

        echo
    done
done

#fautoprofile
opt="fprofile-use"
#
echo "Running with flag: "$opt

#
mkdir -p $dir"/"$opt
mkdir -p $dir"/"$opt"/data"

#
cp "plot_inv.gp" $dir"/"$opt

#Going through invert code variants
for variant in inv_baseline inv_one_loop inv_bitflip inv_unroll_8 inv_unroll_8_bitflip \
                inv_parallel inv_parallel_bitflip inv_unroll_8_parallel inv_unroll_8_parallel_bitflip
do
#
echo -e "\tVariant: "$variant

#Generate profile
make $variant OFLAGS="-march=native -Ofast" O="fprofile-generate" >> $dir"/logs/compile.log" 2>> $dir"/logs/compile_err.log"

#Run & select run number & cycles 
./invert in/input.raw iout/output.raw | cut -d';' -f1,3 > $dir"/"$opt"/data/"$variant

#Use generated profile
make $variant OFLAGS="-march=native -Ofast" O=$opt >> $dir"/logs/compile.log" 2>> $dir"/logs/compile_err.log"

#Run & select run number & cycles 
./invert in/input.raw iout/output.raw | cut -d';' -f1,3 > $dir"/"$opt"/data/"$variant

#Convert raw file into mp4 video
./cvt_vid.sh r2v "iout/output.raw" "iout/output_"$variant".mp4" >> $dir"/logs/cvt.log" 2>> $dir"/logs/cvt_err.log"

echo
done

#
cd $dir"/"$opt

#Generate the plot
gnuplot -c "plot_inv.gp" > "plot_"$opt".png"

cd ../..

echo


#icc profile
opt="prof-use"
#
echo "Running with flag: "$opt

#
mkdir -p $dir"/"$opt
mkdir -p $dir"/"$opt"/data"

#
cp "plot_inv.gp" $dir"/"$opt

#Going through invert code variants
for variant in inv_baseline inv_one_loop inv_bitflip inv_unroll_8 inv_unroll_8_bitflip \
                inv_parallel inv_parallel_bitflip inv_unroll_8_parallel inv_unroll_8_parallel_bitflip
do
#
echo -e "\tVariant: "$variant

#Generate profile
make $variant CC="icc" OFLAGS="-march=native -Ofast" O="prof-gen=threadsafe" >> $dir"/logs/compile.log" 2>> $dir"/logs/compile_err.log"

#Run & select run number & cycles 
./invert in/input.raw iout/output.raw | cut -d';' -f1,3 > $dir"/"$opt"/data/"$variant

#Use generated profile
make $variant CC="icc" OFLAGS="-march=native -Ofast" O=$opt >> $dir"/logs/compile.log" 2>> $dir"/logs/compile_err.log"

#Run & select run number & cycles 
./invert in/input.raw iout/output.raw | cut -d';' -f1,3 > $dir"/"$opt"/data/"$variant

#Convert raw file into mp4 video
./cvt_vid.sh r2v "iout/output.raw" "iout/output_"$variant".mp4" >> $dir"/logs/cvt.log" 2>> $dir"/logs/cvt_err.log"

echo
done

#
cd $dir"/"$opt

#Generate the plot
gnuplot -c "plot_inv.gp" > "plot_"$opt".png"

cd ../..

echo


#
cd $dir

gnuplot -c "plot_inv_all_gcc.gp" > "plot_all_gcc.png" 
gnuplot -c "plot_inv_all_icc.gp" > "plot_all_icc.png" 

cd ..

#
make clean

#
echo -e "\n[DONE]"
