# http://www.gnuplotting.org/plotting-the-world-revisited

unset key
set size ratio -1
set xrange [6.4:14.1]
set yrange [44.2:47.4]

plot 'italy.dat' w lines lc 'black', \
     'stations_north.dat' u 3:2 w points pt 2 lc 'sea-green', \
     'trucks_north.dat' u 3:2 w lines lc 'blue'