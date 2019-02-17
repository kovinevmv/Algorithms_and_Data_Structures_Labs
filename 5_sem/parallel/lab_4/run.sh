#!/bin/bash

gcc $1 -pthread -o merge
gcc $2 -o qsort
echo -e "# Array size\tTime" > plot1.txt
echo -e "# Array size\tTime" > plot2.txt
for (( i=5; i<=8; i++))
do
	current_data=`dc -e "10 ${i} ^ p"`
	echo -n "Sort ${current_data} array. Parallel sort. Time: "
	(time ./merge "${current_data}" 123) &> file.txt
	echo -ne  "${current_data}\t"  >> plot1.txt
	time=`head -2 file.txt |  grep -o -P '(?<=m).*(?=s)'`
	echo "${time}"
	echo "${time}" >> plot1.txt

	echo -n "Sort ${current_data} array. QSort. Time: "
        (time ./qsort "${current_data}" 123) &> file.txt
        echo -ne  "${current_data}\t"  >> plot2.txt
        time=`head -2 file.txt |  grep -o -P '(?<=m).*(?=s)'`
        echo "${time}"
        echo "${time}" >> plot2.txt


done

rm -f file.txt merge qsort
gnuplot -e "set ylabel \"Time (sec)\"; set xlabel \
 \"Array size\"; plot \"plot1.txt\" \
 using 1:2 with lines lt rgb \"red\" lw 3 t \"Parallel sort\", \
 \"plot2.txt\" using 1:2 with lines lt rgb \"blue\"  lw 3 t \"Qsort\"; pause -1"
