d(g, n) = n/g**2
c(g, n, w, t, s, q) = s*tanh(w*d(g, n) + t) + q

w_f = 1
t_f = -1
s_f = 0.5
q_f = 0.5

fit c(x,y,w_f,t_f,s_f,q_f) 'stat.dat' using 1:2:3 via w_f,t_f,s_f,q_f

print sprintf('t(g, n) = %.2f*tanh(%.2f*d(g, n) + %.2f) + %.2f', s_f,w_f,t_f,q_f)

unset key
set xyplane 0
set cbrange [-0.2:0.2]
set palette defined (-1 'red', 0 'green', 1 'red')

splot c(x,y,w_f,t_f,s_f,q_f) lc 'black', \
	  'stat.dat' using 1:2:3:($3 - c($1,$2,w_f,t_f,s_f,q_f)) palette, \
	  'stat.dat' using 1:2:($3 - c($1,$2,w_f,t_f,s_f,q_f)) with lines palette