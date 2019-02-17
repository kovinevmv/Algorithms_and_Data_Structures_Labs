#!/bin/bash

gcc $1
echo -e "# Num proc\tTime" > plot.txt
for (( i=1; i<=15; i++))
do
	echo -n "Calculate using ${i} proc: "
	(time ./a.out ${i} 15000 15000 1234) &> file.txt
	echo -ne  "${i}\t"  >> plot.txt
	time=`head -2 file.txt |  grep -o -P '(?<=m).*(?=s)'`
	echo "${time}"
	echo "${time}" >> plot.txt
done

rm -f file.txt a.out
gnuplot -e "set ylabel \"Time (sec)\"; set xlabel \
 \"Number of processes\"; plot \"plot.txt\" with lines \
 smooth sbezier lw 3 t \"Dependency graph with Bezier smoothing\"; \
 pause -1"
