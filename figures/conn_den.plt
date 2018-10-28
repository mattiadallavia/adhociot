# set terminal pdf size 5in,3.5in font 'Times,16'

d(g, n) = n/g**2
c(d, w, t, s, q) = s*tanh(w*d + t) + q

w_f = 1
t_f = -1
s_f = 0.5
q_f = 0.5

fit [1:20] [50:1000] c(d(x,y),w_f,t_f,s_f,q_f) 'stat.dat' using 1:2:3 via w_f,t_f,s_f,q_f

print sprintf('c(d) = %.2f*tanh(%.2f*d + %.2f) + %.2f', s_f,w_f,t_f,q_f)

set grid
unset key
set ylabel '{/:Italic c(d)}'
set xlabel '{/:Italic d = n/G}'
set cblabel '{/:Italic n}'
set xrange [0:10]
set yrange [0:1]
set xtics 1
set ytics 0.1

plot 'stat.dat' using (d($1, $2)):3:2 palette pt 2 ps 0.8, \
     c(x,w_f,t_f,s_f,q_f) title 'c(d)' lc 'black'

reset