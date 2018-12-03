q(d, n) = n / d**2
c(d, n, w, t, s, u) = s * tanh(w * q(d, n) + t) + u

w_f = 1
t_f = -1
s_f = 0.5
u_f = 0.5

fit [1:20] [50:1000] c(x,y,w_f,t_f,s_f,u_f) 'conn_aggr.dat' using 1:2:($3/$2):(($4+0.001)/$2) via w_f,t_f,s_f,u_f

print sprintf('c(q) = %f * tanh(%f * q + %f) + %f', s_f,w_f,t_f,u_f)

unset key
set xyplane 0
set cbrange [-0.2:0.2]

splot 'conn_aggr.dat' using 1:2:(($3/$2) - c($1,$2,w_f,t_f,s_f,u_f)) with pm3d, \
      c(x,y,w_f,t_f,s_f,u_f) lc 'black', \
      'conn_aggr.dat' using 1:2:($3/$2):(($3/$2) - c($1,$2,w_f,t_f,s_f,u_f)) palette