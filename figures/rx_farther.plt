# set terminal pdf size 3in,1.5in font 'Times,16' dl 1.5

unset key
unset tics
unset border

set style textbox opaque noborder margins 3,3

set xrange [-3.2:-0.8]
set yrange [-1:1]

set label 1 '' at -1,0 point pt 3
set label 2 '' at -3,0 point pt 3

set label 3 '{/:Italic p}' at -1,0.8 center
set label 4 '{/:Italic p + 1}' at -3,0.8 center
set label 5 '{/:Italic i}' at -1,-0.8 center
set label 6 '{/:Italic j}' at -3,-0.8 center

set object 1 circle at 0,0 size 2 dt 4 fc rgb 'black'

set arrow 1 from -2.8,0 to -1.2,0

set label 7 '{/:Italic m}' at -2,0 center boxed front

plot NaN