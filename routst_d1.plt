# ./routst 1 1 1 0 400 20 1000 100 --s 1 --c > routst_d1.dat
# ./st 2 --a 6 --m --st < routst_d1.dat > routst_d1_t.aggr.dat
# ./st 2 --a 7 --m < routst_d1.dat > routst_d1_tx.aggr.dat

set xlabel 'n'
set key top left

set multiplot layout 1,2

# t
set ylabel 't'

plot 'routst_d1_t.aggr.dat' with lines lc 'black' notitle, \
     'routst_d1_t.aggr.dat' using 1:($2+$3) with lines lc 'black' dt 2 title '± {/Symbol s}', \
     'routst_d1_t.aggr.dat' using 1:($2-$3) with lines lc 'black' dt 2 notitle

# tx
set ylabel 'tx'
plot 'routst_d1_tx.aggr.dat' with lines lc 'black' notitle

unset multiplot