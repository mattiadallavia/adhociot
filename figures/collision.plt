# set terminal pdf size 3.5in,2.8in font 'Times,16' dl 1.5

unset key
unset tics
unset border

set size ratio 0.8
set xrange [-2:3]
set yrange [-2:2]
set style textbox opaque noborder margins 1.5,1.5

set object 1 circle at 1,0 size 2 arc [120:240] fs pattern 7 noborder fc 'black'
set object 2 circle at -1,0 size 2 arc [300:60] fs pattern 7 noborder fc 'black'

set object 3 circle at 1,0 size 2 dt 2 fc 'black'
set object 4 circle at -1,0 size 2 dt 2 fc 'black'
set object 5 circle at 1,0 size char 1 fs solid fc 'white'
set object 6 circle at -1,0 size char 1 fs solid fc 'white'
set object 7 circle at 0,0 size char 1 fs solid fc 'white'

set label 1 '' at 1,0 point pt 3
set label 2 '' at -1,0 point pt 3
set label 3 '' at 0,0 point pt 3
set label 4 '' at 2,0 point pt 3

set label 5 '{/:Italic i}' at -1,-0.4 center boxed
set label 6 '{/:Italic k}' at 0,-0.4 center boxed
set label 7 '{/:Italic j}' at 1,-0.4 center boxed
set label 8 '{/:Italic q}' at 2,-0.4 center

plot NaN