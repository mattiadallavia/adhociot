# set terminal pdf size 3in,3in font 'Times,16'

unset key
set xlabel '{/:Italic g}' offset 0,0.8
set ylabel '{/:Italic n}' offset 7
set ztics 0.2
set view 60, 350, 1.2, 0.8

splot 'stat.dat' with lines lc 'black'