# set terminal pdf size 5cm,4cm font 'Times,14'

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

$conn << EOD
-2 2.5
-1 2
0 1.5
2 1.5 # s
1.5 0
3 -3
13.5 -16.5 # 4
2 -4
-1 -1
-7 -2
-21 -14 # 2
-8 -1
-1 1
-8 3
-15 8 # 5
-7 4
-4 3.5
-3 3
-2 2.5
EOD

unset tics
unset key
unset border
set xrange [-8.5:3.5]
set yrange [-5.5:4.5]
set style textbox opaque noborder margin 1,1

set label 1 '{/Times-Bold S}' at 0,0 boxed center front
set label 2 "sottografo\nconnesso" at -3.5,-3.6 boxed center front

plot '$arcs' with lines lc 'black', \
     '$arcs_extra' with lines dt 2 lc 'black', \
     '$vertices' pt 7 ps 2 lc 'white', \
     '$vertices' pt 3 lc 'black', \
     '$vertices' with labels offset 0,-0.8 center, \
     '$conn' with lines smooth bezier dt 4 lc 'black'