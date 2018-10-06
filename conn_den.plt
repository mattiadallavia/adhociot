d(g, n) = n/g**2
c(d, w, t, s, q) = s*tanh(w*d + t) + q

w_f = 1
t_f = -1
s_f = 0.5
q_f = 0.5

fit c(d(x,y),w_f,t_f,s_f,q_f) 'stat.dat' using 1:2:3 via w_f,t_f,s_f,q_f

print sprintf('c(d) = %.2f*tanh(%.2f*d + %.2f) + %.2f', s_f,w_f,t_f,q_f)

set grid
set key top left box opaque
set xlabel 'area gain density: d(g, n) = n/g^2'
set cblabel 'nodes'
set xrange [0:10]
set yrange [0:1]
set xtics 1
set ytics 0.1

plot c(x,w_f,t_f,s_f,q_f) title 'c(d)', \
	 'stat.dat' using (d($1, $2)):3:2 palette notitle

reset