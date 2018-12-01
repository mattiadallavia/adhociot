# set terminal pdf size 5in,3.5in font 'Times,16'

q(d, n) = n / d**2
c(q, w, t, s, u) = s * tanh(w * q + t) + u

w_f = 1
t_f = -1
s_f = 0.5
u_f = 0.5

fit [1:20] [50:1000] c(q(x,y),w_f,t_f,s_f,u_f) 'conn.dat' using 1:2:3 via w_f,t_f,s_f,u_f

print sprintf('c(q) = %.2f * tanh(%.2f * q + %.2f) + %.2f', s_f,w_f,t_f,u_f)

set grid
unset key
set ylabel '{/:Italic c(q)}'
set xlabel '{/:Italic q = n/D}'
set cblabel '{/:Italic n}'
set xrange [0:10]
set yrange [0:1]
set xtics 1
set ytics 0.1

plot 'conn.dat' using (q($1, $2)):3:2 palette pt 2 ps 0.8, \
     c(x,w_f,t_f,s_f,u_f) lc 'black'

reset