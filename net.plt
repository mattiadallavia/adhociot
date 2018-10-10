load 'net.dat'

set size square
unset key
unset border
set xrange [-radius:radius]
set yrange [-radius:radius]
set xtics axis radius/5, radius/5, radius scale 0 left offset 0.2, 1
unset ytics

do for [i = (radius/5):radius:(radius/5)] {
	set object i circle size i dashtype 3 fillcolor rgb '#444444'
}

plot '$arcs' with lines lc rgb 'black', \
	 '$weights' with points ps 3.5 pt 7 lc rgb 'white', \
	 '$weights' with labels tc rgb 'black', \
	 '$vertices' with points ps 3.5 pt 7 lc rgb 'black', \
	 '$vertices' with labels tc rgb 'white'