# set terminal pdf size 3.5in,3.5in font 'Times,16' dl 1.5

unset tics
unset key
unset border
set style textbox opaque noborder margin 1,1
set size square
set xrange [-env:env]
set yrange [-env:env]

set object 1 circle size env

plot '$arcs' with lines lc 'black', \
     '$vertices' pt 7 ps 3 lc 'white', \
     '$vertices' pt 3 lc 'black', \
     '$vertices' with labels boxed offset 0,-1 center font ':Italic'

# '$weights' with labels boxed center font ':Italic'