# ./routst 1 1 1 0 30 1 1000 100 --s 1 --c > routst.dat
# ./st 2 --a 6 --m --st < routst.dat > routst_aggr.dat

set xlabel 'n'
set ylabel 't'
set key top left

plot 'routst_aggr.dat' with lines lc 'black' notitle, \
     'routst_aggr.dat' using 1:($2+$3) with lines lc 'black' dt 2 title '+-sigma', \
     'routst_aggr.dat' using 1:($2-$3) with lines lc 'black' dt 2 notitle