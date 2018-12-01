q(d, n) = n / d**2
c(d, n, w, t, s, u) = s * tanh(w * q(d, n) + t) + u

w_f = 1
t_f = -1
s_f = 0.5
u_f = 0.5

fit [1:20] [50:1000] c(x,y,w_f,t_f,s_f,u_f) 'conn.dat' using 1:2:3 via w_f,t_f,s_f,u_f

print sprintf('c(q) = %.2f * tanh(%.2f * q + %.2f) + %.2f', s_f,w_f,t_f,u_f)

unset key
set xyplane 0
set cbrange [-0.2:0.2]

splot 'conn.dat' using 1:2:($3 - c($1,$2,w_f,t_f,s_f,u_f)) with pm3d, \
      c(x,y,w_f,t_f,s_f,u_f) lc 'black', \
      'stat.dat' using 1:2:3:($3 - c($1,$2,w_f,t_f,s_f,u_f)) palette