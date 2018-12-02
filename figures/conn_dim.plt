# set terminal pdf size 7cm,7cm font 'Times,16'

unset key
set xlabel '{/:Italic d}'
set contour
set view map
set grid
unset surface
unset ylabel
set xtics 2

set label 1 '{/:Italic n} = 100' at 5.6,0.70 rotate by -84
set label 2 '{/:Italic n} = 500' at 11.1,0.70 rotate by -84
set label 3 '{/:Italic n} = 900' at 14.7,0.70 rotate by -82

set cntrparam levels discrete 100, 500, 900
splot 'conn.dat' using 1:3:2 with lines