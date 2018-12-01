# ./conn 0 1000 50 0 20 1 1000 100

set multiplot layout 2, 2
unset key

# 1
set xlabel 'dim'
set ylabel 'n'

splot 'conn.dat' with lines

# 2
set contour
set view map
set grid
unset surface

set cntrparam levels discrete 0.1, 0.5, 0.9
splot 'conn.dat' with lines

# 3
unset ylabel
set ytics 0.1

set cntrparam levels discrete 100, 500, 900
splot 'conn.dat' using 1:3:2 with lines

# 4
set xlabel 'n'

set cntrparam levels discrete 5, 10, 15
splot 'conn.dat' using 2:3:1 with lines

unset multiplot
reset