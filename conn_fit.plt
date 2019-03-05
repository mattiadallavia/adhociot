q(d, n) = n / d**2
c(x) = s * tanh(w * x + t) + u

w = 1
t = -1
s = 0.5
u = 0.5

fit [1:20] [50:1000] c(q(x, y)) 'conn.aggr.dat' using 1:2:($3/$2):(($4+0.001)/$2) via w,t,s,u

print sprintf('c(q) = %f * tanh(%f * q + %f) + %f', s,w,t,u)

do for [x = 0:12] {
	print sprintf('%2d    %f', x, c(x))
}

unset key
set xyplane 0
set cbrange [-0.2:0.2]

splot 'conn.aggr.dat' using 1:2:(($3/$2) - c(q($1,$2))) with pm3d, \
      c(q(x,y)) lc 'black', \
      'conn.aggr.dat' using 1:2:($3/$2):(($3/$2) - c(q($1,$2))) palette