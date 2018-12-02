# set terminal pdf size 8cm,8cm font 'Times,16'

n(d) = 16/pi * d**2

unset key
set grid
set xrange [0:14]
set yrange [0:1000]
set xtics 2
set ytics 200
set link x2 via x**2 inverse sqrt(x)
set for [x = 0:7] x2tics ((2*x)**2)
set xlabel '{/:Italic d}'
set x2label '{/:Italic D = d^2}'
set ylabel '{/:Italic n {/Symbol \263} 16/{/Symbol p} d^2}'

plot n(x) lc 'black', \
     n(x) with filledcurves x2 fillstyle pattern 6 lc 'black'