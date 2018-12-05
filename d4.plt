# ./routst 4 4 1 100 500 20 1000 100 --s 1 --c > d4.dat
# ./st 2 --a 6 --m --st < d4.dat > d4_t.aggr.dat
# ./st 2 --a 7 --m --st < d4.dat > d4_tx.aggr.dat
# ./st 2 --a 9 --m --st < d4.dat > d4_coll.aggr.dat

# d=4
# n=100 q=n/D=100/16=6.25 c>0.95

unset key
set xlabel 'n'

set multiplot layout 1,3

# t
set ylabel 't (±{/Symbol s})'

plot 'd4_t.aggr.dat' with lines lc 'black', \
     'd4_t.aggr.dat' using 1:($2+$3) with lines lc 'black' dt 2, \
     'd4_t.aggr.dat' using 1:($2-$3) with lines lc 'black' dt 2

# tx
set ylabel 'tx  (±{/Symbol s})'
plot 'd4_tx.aggr.dat' with lines lc 'black', \
     'd4_tx.aggr.dat' using 1:($2+$3) with lines lc 'black' dt 2, \
     'd4_tx.aggr.dat' using 1:($2-$3) with lines lc 'black' dt 2

# coll
set ylabel 'coll  (±{/Symbol s})'
plot 'd4_coll.aggr.dat' with lines lc 'black', \
     'd4_coll.aggr.dat' using 1:($2+$3) with lines lc 'black' dt 2, \
     'd4_coll.aggr.dat' using 1:($2-$3) with lines lc 'black' dt 2 

unset multiplot