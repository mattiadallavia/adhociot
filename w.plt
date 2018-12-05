# ./routst 1 1 1 10 10 1 1000 5000 --s 1 --c --w '0 200 10' --b '1 1 1' > w.dat
# ./st 4 --a 6 --m --st < w.dat > w_t.aggr.dat
# ./st 4 --a 7 --m --st < w.dat > w_tx.aggr.dat
# ./st 4 --a 9 --m --st < w.dat > w_coll.aggr.dat

unset key
set xlabel 'w'

set multiplot layout 1,3

# t
set ylabel 't (±{/Symbol s})'

plot 'w_t.aggr.dat' with lines lc 'black', \
     'w_t.aggr.dat' using 1:($2+$3) with lines lc 'black' dt 2, \
     'w_t.aggr.dat' using 1:($2-$3) with lines lc 'black' dt 2

# tx
set ylabel 'tx (±{/Symbol s})'
plot 'w_tx.aggr.dat' with lines lc 'black' notitle, \
     'w_tx.aggr.dat' using 1:($2+$3) with lines lc 'black' dt 2, \
     'w_tx.aggr.dat' using 1:($2-$3) with lines lc 'black' dt 2

# coll
set ylabel 'coll (±{/Symbol s})'
plot 'w_coll.aggr.dat' with lines lc 'black' notitle, \
     'w_coll.aggr.dat' using 1:($2+$3) with lines lc 'black' dt 2, \
     'w_coll.aggr.dat' using 1:($2-$3) with lines lc 'black' dt 2

unset multiplot