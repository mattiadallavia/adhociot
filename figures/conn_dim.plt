# set terminal pdf size 3in,3in font 'Times,16'

unset key
set xlabel '{/:Italic d}'
set contour
set view map
set grid
unset surface
unset ylabel
set xtics 2

set label 1 '{/:Italic n} = 100' at 5.5,0.70 rotate by -84
set label 2 '{/:Italic n} = 500' at 10.9,0.70 rotate by -81
set label 3 '{/:Italic n} = 900' at 14.6,0.70 rotate by -82

set cntrparam levels discrete 100, 500, 900
splot 'conn.dat' using 1:3:2 with lines