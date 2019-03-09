# set terminal pdf size 8cm,6cm font 'Times,14'

# awk '$1 && $2 {print $2/($1^2)"\t"$3/$2"\t"$4/$2}' conn.aggr.dat | sort -g > conn.q.dat
# awk '$1 <= 10 {b=int($1+0.5); s[b]+=$2; n[b]++} END {for(i=0; i<=10; i++) print i"\t"s[i]/n[i]}' conn.q.dat > conn.bin.dat

set grid
unset key
set ylabel 'c(q)'
set xlabel 'q'
set cblabel 'n'
set xtics 1
set ytics 0.1

plot 'conn.bin.dat' w lines lc 'black'