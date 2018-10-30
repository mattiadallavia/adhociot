# set terminal pdf size 3.5in,3.5in font 'Times,16' dl 1.5

$nodes << EOD
-4 1 1
-7 -1 2
4 8 3
2 -3 4
EOD

unset tics
unset key
unset border
set size square
set xrange [-10:10]
set yrange [-10:10]
set style textbox opaque noborder margin 2,2

set object 1 circle at 0,0 size 10 fc rgb 'black'

set label 1 '{/:Bold S}' at 0,0 boxed center front
set object 2 circle at 0,0 size 5 dt 2 fc 'black'

set arrow 1 from 0,0 to 10,0
set arrow 2 from 4,8 to 9,8

set label 2 '{/:Italic a}' at 7,0 boxed center front
set label 3 '{/:Italic p}' at 6.5,8 boxed center front

plot '$nodes' pt 7 ps 2 lc 'white', \
     '$nodes' pt 3 lc 'black', \
     '$nodes' using 1:2:(5) with circles dt 2 fc 'black'