load 'net.dat'

set size square
unset key
unset border
set xrange [-radius*1.1:radius*1.1]
set yrange [-radius*1.1:radius*1.1]
set xtics radius/5
set ytics radius/5

set object 1 circle size radius fc rgb 'black' dashtype 3

plot '$arcs' using 1:2 with lines lc rgb 'black', \
	 '$weights' using 2:3 with points ps 3.5 pt 7 lc rgb 'white', \
	 '$weights' using 2:3:1 with labels tc rgb 'black', \
	 '$vertices' using 2:3 with points ps 3.5 pt 7 lc rgb 'black', \
	 '$vertices' using 2:3:1 with labels tc rgb 'white'