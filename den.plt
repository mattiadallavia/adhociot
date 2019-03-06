normal(x) = a * 1/(sigma*sqrt(2.0*pi)) * exp(-(x-mu)**2/(2.0*sigma**2))
poisson(x) = (x<0) ? 0 : (a*exp(x*log(mu)-lgamma(x+1)-mu))

fit poisson(x) 'den.dat' using 2:1 via mu, a

unset key

plot 'den.dat' u 2:1, \
     poisson(x)