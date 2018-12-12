# set terminal pdf size 5cm,5cm font 'Times,14'

unset tics
unset key
unset border
set xrange [-env:env]
set yrange [-env:env]
set style textbox opaque noborder margin 1,1

set object 1 circle size env

plot '$arcs' with lines lc 'black', \
     '$vertices' pt 7 ps 2 lc 'white', \
     '$vertices' pt 3 lc 'black', \
     '$vertices' with labels boxed offset 0,-0.8 center