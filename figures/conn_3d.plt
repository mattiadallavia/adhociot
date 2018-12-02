# set terminal pdf size 7cm,7cm font 'Times,16'

unset key
set xlabel '{/:Italic d}' offset 0,0.8
set ylabel '{/:Italic n}' offset 7
set ytics 200
set ztics 0.2
set view 60, 350, 1.2, 0.8

splot 'conn.dat' with lines lc 'black'