unset key
set xlabel '{/:Italic g}'
set contour
set view map
set grid
unset surface
unset ylabel

set label 1 '{/:Italic n} = 20' at 2.58,0.68 rotate by -75
set label 2 '{/:Italic n} = 50' at 3.6,0.68 rotate by -75
set label 3 '{/:Italic n} = 80' at 4.42,0.68 rotate by -75

set cntrparam levels discrete 20, 50, 80
splot 'stat.dat' using 1:3:2 with lines