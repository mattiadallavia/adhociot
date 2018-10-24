# set terminal pdf size 3.5in,3.5in font 'Times,16' dl 2

unset tics
unset key
unset border

set size square
set xrange [-1:1]
set yrange [-1:1]

set object 1 circle at 0,0 size 1 fc rgb 'black'
set object 2 circle at 0,0 size 0.4 dt 2 fc rgb 'black'
set object 3 circle at -0.35,0 size 0.4 dt 2  fc rgb 'black'

set label 1 '' at 0,0 point pt 3
set label 2 '' at -0.35,0 point pt 3

set arrow 1 from 0,0 to sqrt(2)/2,(-sqrt(2)/2)
set arrow 2 from 0,0 to 0.4*sqrt(2)/2,0.4*sqrt(2)/2

set object 4 circle at sqrt(2)/3,(-sqrt(2)/3) front size char 1 fs solid fc rgb 'white'
set label 3 '{/:Italic r}' at sqrt(2)/3,(-sqrt(2)/3) center front

set object 5 circle at 0.4*sqrt(2)/4,0.4*sqrt(2)/4 front size char 1 fs solid fc rgb 'white'
set label 4 '{/:Italic p}' at 0.4*sqrt(2)/4,0.4*sqrt(2)/4 center front

plot NaN