# set terminal pdf size 3.5in,3.5in font 'Times,16' dl 1.5

unset tics
unset key
unset border

set size square
set xrange [-1:1]
set yrange [-1:1]

set object 1 circle at 0,0 size 1 fc rgb 'black'
set object 2 circle at 0,0 size 0.3 dt 2 fc rgb 'black'
set object 3 circle at -0.25,0 size 0.3 dt 2 fc rgb 'black'
set object 4 circle at -0.35,-0.2 size 0.3 dt 2 fc rgb 'black'
set object 5 circle at 0.6,0.2 size 0.3 dt 2 fc rgb 'black'

set label 1 '' at 0,0 point pt 3
set label 2 '' at -0.25,0 point pt 3
set label 3 '' at -0.35,-0.2 point pt 3
set label 4 '' at 0.6,0.2 point pt 3

set arrow 1 from 0,0 to sqrt(2)/2,(-sqrt(2)/2)
set arrow 2 from 0,0 to 0.3*sqrt(2)/2,0.3*sqrt(2)/2

set object 6 circle at 0.6*sqrt(2)/2,(-0.6*sqrt(2)/2) front size char 1 fs solid fc rgb 'white'
set label 5 '{/:Italic r}' at 0.6*sqrt(2)/2,(-0.6*sqrt(2)/2) center front

set object 7 circle at 0.15*sqrt(2)/2,0.15*sqrt(2)/2 front size char 1 fs solid fc rgb 'white'
set label 6 '{/:Italic p}' at 0.15*sqrt(2)/2,0.15*sqrt(2)/2 center front

plot NaN