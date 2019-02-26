# http://www.gnuplotting.org/plotting-the-world-revisited

unset key
set size ratio -1

plot 'italy.dat' w lines lc 'black', \
     'stations.dat' u 3:2 w points pt 2, \
     '1021.dat' u 3:2 w lines lc 'blue'