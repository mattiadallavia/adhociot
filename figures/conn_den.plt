# set terminal pdf size 12cm,8cm font 'Times,16'

q(d, n) = n / d**2
c(q) = 0.497921 * tanh(0.985667 * q - 4.557268) + 0.502079

set grid
unset key
set ylabel '{/:Italic c(q)}'
set xlabel '{/:Italic q}'
set cblabel '{/:Italic n}'
set xrange [0:10]
set yrange [0:1]
set xtics 1
set ytics 0.1

plot 'conn_aggr.dat' using (q($1, $2)):($3/$2):2 palette pt 2 ps 0.8, \
     c(x) lc 'black'

reset