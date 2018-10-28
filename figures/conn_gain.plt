t(x, w, t, s, q, e) = s*tanh(w/x**e + t) + q
c(g) = g**(-2)

w_20 = -1
t_20 = 1
s_20 = -0.5
q_20 = 0.5
e_20 = 2

w_50 = -1
t_50 = 1
s_50 = -0.5
q_50 = 0.5
e_50 = 2

w_80 = -1
t_80 = 1
s_80 = -0.5
q_80 = 0.5
e_80 = 2

fit t(x,w_20,t_20,s_20,q_20,e_20) 'stat.dat' \
    using ((($2 >= 15) && ($2 <= 25)) ? $1 : NaN):3:(abs($2 - 20)+1) via w_20,t_20,s_20,q_20,e_20

fit t(x,w_50,t_50,s_50,q_50,e_50) 'stat.dat' \
    using ((($2 >= 45) && ($2 <= 55)) ? $1 : NaN):3:(abs($2 - 50)+1) via w_50,t_50,s_50,q_50,e_50

fit t(x,w_80,t_80,s_80,q_80,e_80) 'stat.dat' \
    using ((($2 >= 75) && ($2 <= 85)) ? $1 : NaN):3:(abs($2 - 80)+1) via w_80,t_80,s_80,q_80,e_80

print sprintf('t(g, 20) = %.2f*tanh(%.2f/x^%.2f + %.2f) + %.2f', s_20,w_20,e_20,t_20,q_20)
print sprintf('t(g, 50) = %.2f*tanh(%.2f/x^%.2f + %.2f) + %.2f', s_50,w_50,e_50,t_50,q_50)
print sprintf('t(g, 80) = %.2f*tanh(%.2f/x^%.2f + %.2f) + %.2f', s_80,w_80,e_80,t_80,q_80)

set grid
set key top right box opaque
set xlabel 'gain'
set xrange [0.5:7]
set yrange [0:1]
set ytics 0.1
set x2label 'gain in area'
set x2tics ('1' 1, '4' 2, '9' 3, '16' 4, '25' 5, '36' 6, '49' 7)
set palette defined (0 '#4BC0C8', 0.5 '#C779D0', 1 '#FEAC5E')

plot 'stat.dat' using 1:3:2 with points pointtype 3 palette notitle, \
	 c(x) title '1/x^2', \
	 t(x,w_20,t_20,s_20,q_20,e_20) palette cb 0.2 title 't(g, 20)', \
	 t(x,w_50,t_50,s_50,q_50,e_50) palette cb 0.5 title 't(g, 50)', \
	 t(x,w_80,t_80,s_80,q_80,e_80) palette cb 0.8 title 't(g, 80)'

reset