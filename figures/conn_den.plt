# set terminal pdf size 10cm,8cm font 'Times,14'

q(d, n) = n / d**2
c(q) = 0.497921 * tanh(0.985667 * q - 4.557268) + 0.502079

set grid
unset key
set ylabel 'c(q)'
set xlabel 'q'
set cblabel 'n'
set xrange [0:10]
set yrange [0:1]
set xtics 1
set ytics 0.1

plot 'conn.aggr.dat' using (q($1, $2)):($3/$2):2 palette pt 2 ps 0.8, \
     c(x) lc 'black'

reset