# ./stat 1 1 1 0 30 1 1000 100 --seed 1 --coll > stat.dat
# ./st 2 --agg 4 --mean --stdev < stat.dat > stat_aggr.dat

set xlabel 'n'
set ylabel 't'
set key top left

plot 'stat_aggr.dat' with lines lc 'black' notitle, \
     'stat_aggr.dat' using 1:($2+$3) with lines lc 'black' dt 2 title '+-sigma', \
     'stat_aggr.dat' using 1:($2-$3) with lines lc 'black' dt 2 notitle