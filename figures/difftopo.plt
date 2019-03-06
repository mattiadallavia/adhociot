$m1 << EOD
0	.6
.2	.65
.8	.55
1 	.6
EOD

$m2 << EOD
.4	0
.6	.2
1	.3
EOD

$n1a << EOD
.1	.4
.35	.25
.4	.5
.6	.2 # sink
.7	.4
.85	.1
EOD

$a1a << EOD
.6	.2 # sink
.85	.1

.6	.2 # sink
.35	.25

.6	.2 # sink
.7	.4

.35	.25
.1	.4

.35	.25
.4	.5
EOD

$n1b << EOD
.1	.4
.35	.25
.4	.5
.7	.4
.85	.1
.2	.65 # sink
.8	.55 # sink
.1	.9
.4	.8
.7	.75
EOD

$a1b << EOD
.2	.65 # sink
.1	.9

.2	.65 # sink
.4	.8

.8	.55 # sink
.7	.75
EOD

$n2 << EOD
.1	.4
.35	.25
.4	.5
.7	.4
.85	.1
.2	.65 # sink
.8	.55 # sink
.1	.9
.4	.8
.7	.75
EOD

$a2 << EOD
.2	.65 # sink
.1	.9

.2	.65 # sink
.4	.8

.2	.65 # sink
.1	.4

.2	.65 # sink
.4	.5

.4	.5
.35	.25

.8	.55 # sink
.7	.75

.8	.55 # sink
.7	.4

.7	.4
.85	.1
EOD

unset key
unset tics
set xrange [0:1]
set yrange [0:1]
set title offset 0,-0.8

set multiplot layout 1,3

set lmargin 1
set rmargin 1

# day 1a
set title 'day 1'
plot '$m2' w lines smooth csplines dt 4 lc 'black', \
     '$a1a' with lines lc 'black', \
     '$n1a' pt 7 ps 3 lc 'white', \
     '$n1a' pt 3 lc 'black'

# day 1b
set title 'day 1'
plot '$m1' w lines smooth csplines dt 4 lc 'black', \
     '$a1b' with lines lc 'black', \
     '$n1b' pt 7 ps 3 lc 'white', \
     '$n1b' pt 3 lc 'black'

# day 2
set title 'day 2'
plot '$m1' w lines smooth csplines dt 4 lc 'black', \
     '$a2' with lines lc 'black', \
     '$n2' pt 7 ps 3 lc 'white', \
     '$n2' pt 3 lc 'black'

unset title
unset multiplot