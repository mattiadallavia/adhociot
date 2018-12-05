# ./conn 0 20 1 0 1000 50 1000 100 --s 1 > conn.dat
# ./st 1 2 --a 3 --m --st < conn.dat > conn_aggr.dat

set multiplot layout 2, 2
unset key

# 1
set xlabel 'dim'
set ylabel 'n'

splot 'conn_aggr.dat' using 1:2:($3/$2) with lines

# 2
set contour
set view map
set grid
unset surface

set cntrparam levels discrete 0.1, 0.5, 0.9
splot 'conn_aggr.dat' using 1:2:($3/$2) with lines

# 3
unset ylabel
set ytics 0.1

set cntrparam levels discrete 100, 500, 900
splot 'conn_aggr.dat' using 1:($3/$2):2 with lines

# 4
set xlabel 'n'

set cntrparam levels discrete 5, 10, 15
splot 'conn_aggr.dat' using 2:($3/$2):1 with lines

unset multiplot
reset