n(g) = 16/pi * g**2
l(x, w, t) = w*x**2 + t

w_c = 1
t_c = 1

w_t = 1
t_t = 1

fit l(x,w_c,t_c) 'conn.dat' \
    using ((($3 >= 0.45) && ($3 <= 0.55)) ? $1 : NaN):2:($3 - 0.5) via w_c,t_c

fit l(x,w_t,t_t) 'conn.dat' \
    using ((($3 >= 0.85) && ($3 <= 0.95)) ? $1 : NaN):2:($3 - 0.9) via w_t,t_t

print sprintf('l(g, 0.9) = %.2f*x^2 + %.2f', w_t, t_t)
print sprintf('l(g, 0.5) = %.2f*x^2 + %.2f', w_c, t_c)

set grid
set key top left box opaque
set xlabel 'dim'
set ylabel 'n'
set xrange [0:20]
set yrange [0:1000]
set palette defined (0 '#4BC0C8', 0.5 '#C779D0', 1 '#FEAC5E')
unset colorbox

plot l(x,w_c,t_c) with filledcurves x2 palette cb 0.5 notitle fillstyle pattern 6, \
	 l(x,w_t,t_t) with filledcurves x2 palette cb 0.9 notitle fillstyle pattern 6, \
	 'conn.dat' using ((($3 >= 0.45) && ($3 <= 0.55)) ? $1 : NaN):2:3 palette pointtype 3 notitle, \
	 'conn.dat' using ((($3 >= 0.85) && ($3 <= 0.95)) ? $1 : NaN):2:3 palette pointtype 3 notitle, \
	 l(x,w_t,t_t) palette cb 0.9 title 'l(g, 0.9)', \
	 l(x,w_c,t_c) palette cb 0.5 title 'l(g, 0.5)', \
	 n(x) title '16/{/Symbol p} g^2'

reset