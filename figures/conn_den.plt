# set terminal pdf size 12cm,8cm font 'Times,16'

q(d, n) = n / d**2
c(q) = 0.495136 * tanh(0.967988 * q - 4.381028) + 0.503169

set grid
unset key
set ylabel '{/:Italic c(q)}'
set xlabel '{/:Italic q}'
set cblabel '{/:Italic n}'
set xrange [0:10]
set yrange [0:1]
set xtics 1
set ytics 0.1

plot 'conn.dat' using (q($1, $2)):3:2 palette pt 2 ps 0.8, \
     c(x) lc 'black'

reset