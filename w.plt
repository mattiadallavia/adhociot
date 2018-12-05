# ./routst 1 1 1 10 10 1 1000 5000 --s 1 --c --w '0 30 1' --b '1 1 1' > w.dat
# ./st 4 --a 6 --m --st < w.dat > w_t.aggr.dat
# ./st 4 --a 7 --m --st < w.dat > w_tx.aggr.dat
# ./st 4 --a 9 --m --st < w.dat > w_coll.aggr.dat

set xlabel 'w'
set key top left

set multiplot layout 1,3

# t
set ylabel 't'

plot 'w_t.aggr.dat' with lines lc 'black' notitle, \
     'w_t.aggr.dat' using 1:($2+$3) with lines lc 'black' dt 2 title '± {/Symbol s}', \
     'w_t.aggr.dat' using 1:($2-$3) with lines lc 'black' dt 2 notitle

# tx
set ylabel 'tx'
plot 'w_tx.aggr.dat' with lines lc 'black' notitle, \
     'w_tx.aggr.dat' using 1:($2+$3) with lines lc 'black' dt 2 title '± {/Symbol s}', \
     'w_tx.aggr.dat' using 1:($2-$3) with lines lc 'black' dt 2 notitle

# coll
set ylabel 'coll'
plot 'w_coll.aggr.dat' with lines lc 'black' notitle, \
     'w_coll.aggr.dat' using 1:($2+$3) with lines lc 'black' dt 2 title '± {/Symbol s}', \
     'w_coll.aggr.dat' using 1:($2-$3) with lines lc 'black' dt 2 notitle

unset multiplot