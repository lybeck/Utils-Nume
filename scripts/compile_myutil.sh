#!/bin/bash

dir=$(pwd)

myutil_dir=/home/<username>/Projects/Utils-Nume/src
comp_dir=/home/<username>/Projects/Utils-Nume/comp
cd $myutil_dir

for f in $myutil_dir/*.cpp; do
	g++ -I/home/<username>/myexamples/gnuplot -I/home/<username>/myexamples/democpp -I/usr/local/numerical-recipes/recipes_cpp/utils -I/home/<username>/Projects/Utils-Nume/headers -L/usr/local/numerical-recipes/recipes_cpp -L/home/<username>/myexamples/lib -c $f -std=c++0x
	st=$?
	if [ $st != 0 ]; then
		rm *.o
		cd $dir
		exit $st
	fi
	mv *.o $comp_dir
done

cd $dir

