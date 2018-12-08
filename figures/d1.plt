# ./routst 1 1 1 5 50 5 1000 2000 --w "30 30 1" --b "30 30 1" --r "6 6 1" --s 1 --c > d1.dat
# ./st 2 --a 7 --m --st < d1.dat > d1_tf.aggr.dat
# ./st 2 --a 8 --m --st < d1.dat > d1_tx.aggr.dat
# ./st 2 --a 9 --m --st < d1.dat > d1_retx.aggr.dat
# ./st 2 --a 11 --m --st < d1.dat > d1_coll.aggr.dat
# set terminal pdf size 12cm,9cm font 'Times,14'

r(x) = m*x+q
fit r(x) 'd1_tx.aggr.dat' via m,q
print sprintf('r(x) = %f * x + %f', m,q)

unset key
set ylabel offset 1.5,0

set multiplot layout 2,2

# tf
set lmargin 10
set bmargin 0.5
set xtics format ''
set yrange [0:600]
set ylabel 'tf (±3{/Symbol s})'

plot 'd1_tf.aggr.dat' with lines lc 'black', \
     'd1_tf.aggr.dat' using 1:($2+3*$3) with lines lc 'black' dt 3

# tx
set lmargin 7
set yrange [0:120]
set ylabel 'tx  (±3{/Symbol s})'

plot 'd1_tx.aggr.dat' with lines lc 'black', \
     'd1_tx.aggr.dat' using 1:($2+3*$3) with lines lc 'black' dt 3, \
     'd1_tx.aggr.dat' using 1:($2-3*$3) with lines lc 'black' dt 3

# coll
set lmargin 10
set bmargin 3
set x2tics format ''
set xtics format '%.0f' rotate
unset x2label
set xlabel 'n' offset 0,0.5
set yrange [0:200]
set ylabel 'coll.  (±3{/Symbol s})'

plot 'd1_coll.aggr.dat' with lines lc 'black', \
     'd1_coll.aggr.dat' using 1:($2+3*$3) with lines lc 'black' dt 3

# retx
set lmargin 7
set yrange [0:20]
set ylabel 'retx  (±3{/Symbol s})'

plot 'd1_retx.aggr.dat' with lines lc 'black', \
     'd1_retx.aggr.dat' using 1:($2+3*$3) with lines lc 'black' dt 3

unset xlabel
unset multiplot