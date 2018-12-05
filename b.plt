# ./routst 1 1 1 10 10 1 1000 1000 --s 1 --c --w '0 0 1' --b '1 10 1' > b.dat
# ./st 5 --a 6 --m --st < b.dat > b_t.aggr.dat
# ./st 5 --a 7 --m --st < b.dat > b_tx.aggr.dat
# ./st 5 --a 9 --m --st < b.dat > b_coll.aggr.dat

set xlabel 'b'
set key top left

set multiplot layout 1,3

# t
set ylabel 't'

plot 'b_t.aggr.dat' with lines lc 'black' notitle, \
     'b_t.aggr.dat' using 1:($2+$3) with lines lc 'black' dt 2 title '± {/Symbol s}', \
     'b_t.aggr.dat' using 1:($2-$3) with lines lc 'black' dt 2 notitle

# tx
set ylabel 'tx'
plot 'b_tx.aggr.dat' with lines lc 'black' notitle, \
     'b_tx.aggr.dat' using 1:($2+$3) with lines lc 'black' dt 2 title '± {/Symbol s}', \
     'b_tx.aggr.dat' using 1:($2-$3) with lines lc 'black' dt 2 notitle

# coll
set ylabel 'coll'
plot 'b_coll.aggr.dat' with lines lc 'black' notitle, \
     'b_coll.aggr.dat' using 1:($2+$3) with lines lc 'black' dt 2 title '± {/Symbol s}', \
     'b_coll.aggr.dat' using 1:($2-$3) with lines lc 'black' dt 2 notitle

unset multiplot