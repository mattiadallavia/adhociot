# set terminal pdf size 5in,3.5in font 'Times,16'

n(g) = 16/pi * g**2

unset key
set grid
set xrange [0:14]
set yrange [0:1000]
set xtics 1
set ytics 100
set link x2 via x**2 inverse sqrt(x)
set for [x = 0:14] x2tics (x**2)
set xlabel '{/:Italic g}'
set x2label '{/:Italic G = g^2}'
set ylabel '{/:Italic n = 16/{/Symbol p} g^2}'

plot n(x) lc 'black', \
     n(x) with filledcurves y=1000 fillstyle pattern 6 lc 'black'