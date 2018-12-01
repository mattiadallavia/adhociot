p(x, w, t) = w*x**2 + t

w_c = 1
t_c = 1

w_t = 1
t_t = 1

fit p(x,w_c,t_c) 'conn.dat' \
    using ((($3 >= 0.45) && ($3 <= 0.55)) ? $1 : NaN):2:($3 - 0.50) via w_c,t_c

fit p(x,w_t,t_t) 'conn.dat' \
    using ((($3 >= 0.85) && ($3 <= 0.95)) ? $1 : NaN):2:($3 - 0.9) via w_t,t_t

print sprintf('p(g, 0.9) = %.2f*x^2 + %.2f', w_t, t_t)
print sprintf('p(g, 0.5) = %.2f*x^2 + %.2f', w_c, t_c)

set multiplot layout 1, 2

set grid
unset key
set xrange [0.5:7]
set yrange [10:100]

plot 'conn.dat' using ((($3 >= 0.45) && ($3 <= 0.55)) ? $1 : NaN):2:3, \
	 'conn.dat' using ((($3 >= 0.85) && ($3 <= 0.95)) ? $1 : NaN):2:3, \
	 p(x,w_t,t_t), \
	 p(x,w_c,t_c)

set xrange [0:7]
set yrange [-10:10]

plot '+' using 1:((p(x,w_c,t_c)/$1**2)) with lines, \
	 w_c, \
	 '+' using 1:((p(x,w_t,t_t)/$1**2)) with lines, \
	 w_t

unset multiplot
reset