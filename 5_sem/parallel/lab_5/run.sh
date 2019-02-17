#!/bin/bash

gcc $1 -fopenmp
echo -e "# Num proc\tTime" > plot.txt
for (( i=1; i<=20; i++))
do
	echo -n "Calculate using ${i} proc: "
	export OMP_NUM_THREADS=${i}
	(time ./a.out 23279256000) &> file.txt
	echo -ne  "${i}\t"  >> plot.txt
	time=`head -3 file.txt |  grep -o -P '(?<=m).*(?=s)'`
	echo "${time}"
	echo "${time}" >> plot.txt
done

rm -f file.txt a.out
gnuplot -e "set ylabel \"Time (sec)\"; set xlabel \
 \"Number of processes\"; plot \"plot.txt\" with lines \
 smooth sbezier lw 3 t \"Dependency graph with Bezier smoothing\"; \
 pause -1"
