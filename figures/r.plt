# ./routst 1 1 1 10 10 1 1000 1000 --w "0 0 1" --b "1 1 1" --r "0 10 1" --s 1 --c > r.dat
# ./st 6 --a 7 --m --st < r.dat > r_tf.aggr.dat
# ./st 6 --a 9 --m --st < r.dat > r_retx.aggr.dat
# set terminal pdf size 12cm,5cm font 'Times,14'

unset key
set ylabel offset 1.5,0
set xlabel 'r' offset 0,0.5

set multiplot layout 1,2

# tf
set lmargin 10
set yrange [0:6000]
set ylabel 'tf (±3{/Symbol s})'

plot 'r_tf.aggr.dat' with lines lc 'black', \
     'r_tf.aggr.dat' using 1:($2+3*$3) with lines lc 'black' dt 3, \
     'r_tf.aggr.dat' using 1:($2-3*$3) with lines lc 'black' dt 3

# retx
set lmargin 7
set yrange [0:45]
set ylabel 'retx  (±3{/Symbol s})'

plot 'r_retx.aggr.dat' with lines lc 'black', \
     'r_retx.aggr.dat' using 1:($2+3*$3) with lines lc 'black' dt 3, \
     'r_retx.aggr.dat' using 1:($2-3*$3) with lines lc 'black' dt 3

unset multiplot