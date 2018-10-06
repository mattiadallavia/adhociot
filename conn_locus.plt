l(x, w, t) = w*x**2 + t

w_c = 1
t_c = 1

w_t = 1
t_t = 1

fit l(x,w_c,t_c) 'stat.dat' \
    using ((($3 >= 0.45) && ($3 <= 0.55)) ? $1 : NaN):2:($3 - 0.5) via w_c,t_c

fit l(x,w_t,t_t) 'stat.dat' \
    using ((($3 >= 0.85) && ($3 <= 0.95)) ? $1 : NaN):2:($3 - 0.9) via w_t,t_t

print sprintf('l(g, 0.9) = %.2f*x^2 + %.2f', w_t, t_t)
print sprintf('l(g, 0.5) = %.2f*x^2 + %.2f', w_c, t_c)

set grid
set key top left box opaque
set ylabel 'nodes'
set xlabel 'gain'
set xrange [0.5:5]
set yrange [10:100]
set x2label 'gain in area'
set x2tics ('1' 1, '4' 2, '9' 3, '16' 4, '25' 5, '36' 6, '49' 7)
set palette defined (0 '#4BC0C8', 0.5 '#C779D0', 1 '#FEAC5E')
unset colorbox

plot l(x,w_c,t_c) with filledcurves y=1000 palette cb 0.5 notitle fillstyle pattern 6, \
	 l(x,w_t,t_t) with filledcurves y=1000 palette cb 0.9 notitle fillstyle pattern 6, \
	 '+' using ((($1 >= 0) && ($1 <= 1) ? $1 : NaN)):(0) with filledcurve y=1000 fillcolor 'black' notitle fillstyle pattern 6, \
	 'stat.dat' using ((($3 >= 0.45) && ($3 <= 0.55)) ? $1 : NaN):2:3 palette pointtype 3 notitle, \
	 'stat.dat' using ((($3 >= 0.85) && ($3 <= 0.95)) ? $1 : NaN):2:3 palette pointtype 3 notitle, \
	 '<echo 1 100' with impulses linecolor 'black' title 'conn. directly', \
	 l(x,w_t,t_t) palette cb 0.9 title 'l(g, 0.9)', \
	 l(x,w_c,t_c) palette cb 0.5 title 'l(g, 0.5)'

reset