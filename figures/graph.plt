# set terminal pdf size 4.5in,3.15in font 'Times,16' dl 1.5

$vertices << EOD
-4 1 1
-7 -1 2
3 3 3
2 -3 4
-7 3 5
EOD

$arcs << EOD
0 0
-4 1

0 0
2 -3

-4 1
-7 -1

-4 1
-7 3
EOD

$arcs_extra << EOD
0 0
3 3
EOD

$weights << EOD
-2 0.5 0.8
1 -1.5 0.7
-5.5 0 0.6
-5.5 2 0.6
1.5 1.5 1.2
EOD

$conn << EOD
-2 2.5
-1 2
0 1.5
1 1 # s
1.5 0
3 -3
13 -13 # 4
2 -4
-1 -1
-7 -2
-20 -9 # 2
-8 -1
-1 1
-8 3
-13 7 # 5
-7 4
-4 3.5
-3 3
-2 2.5
EOD

unset tics
unset key
unset border
set size ratio 0.7
set xrange [-8.5:3.5]
set yrange [-4.5:4]
set style textbox opaque noborder margin 2,2

set label 1 '{/:Bold S}' at 0,0 boxed center front
set label 2 "{/:Italic sottografo}\n{/:Italic connesso}" at -3.5,-3.5 boxed center front

plot '$arcs' with lines lc 'black', \
     '$arcs_extra' with lines dt 2 lc 'black', \
     '$weights' with labels boxed center font ':Italic', \
     '$vertices' pt 7 ps 3 lc 'white', \
     '$vertices' pt 3 lc 'black', \
     '$vertices' with labels offset 0,-1 center font ':Italic', \
     '$conn' with lines smooth bezier dt 4 lc 'black'