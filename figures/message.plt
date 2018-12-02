# set terminal pdf size 8cm,1.5cm font 'Times,16'

unset key
unset tics
unset border
set xrange [-1:1]
set yrange [0:1]
set style textbox opaque noborder margins 2,2
set style arrow 1 head filled size screen 0.03,15,45

set label 1 '' at -1,0 point pt 3
set label 2 '' at 0,0 point pt 3
set label 3 '' at 1,0 point pt 3

set label 4 '{/:Italic p + 1}' at -1,1 center
set label 5 '{/:Italic p}' at     0,1 center
set label 6 '{/:Italic p - 1}' at 1,1 center

set arrow 1 from -0.1,0 to -0.9,0 arrowstyle 1
set arrow 2 from 0.1,0 to 0.9,0 arrowstyle 1

set label 7 '{/:Italic esp. / risc.}' at -0.45,0 center boxed front
set label 8 '{/:Italic consegna}' at 0.45,0 center boxed front

plot NaN