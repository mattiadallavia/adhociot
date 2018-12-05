# ./routst 1 1 1 0 400 20 1000 100 --s 1 --c > d1.dat
# ./st 2 --a 6 --m --st < d1.dat > d1_t.aggr.dat
# ./st 2 --a 7 --m < d1.dat > d1_tx.aggr.dat

set xlabel 'n'
set key top left

set multiplot layout 1,2

# t
set ylabel 't'

plot 'd1_t.aggr.dat' with lines lc 'black' notitle, \
     'd1_t.aggr.dat' using 1:($2+$3) with lines lc 'black' dt 2 title '± {/Symbol s}', \
     'd1_t.aggr.dat' using 1:($2-$3) with lines lc 'black' dt 2 notitle

# tx
set ylabel 'tx'
plot 'd1_tx.aggr.dat' with lines lc 'black' notitle

unset multiplot