library(gstat)
data(meuse)
new.locs <- data.frame(x = c(181170, 180310, 180205, 178673, 178770, 178270),
	y = c(333250, 332189, 331707, 330066, 330675, 331075))
krige(zinc ~ 1, ~ x + y, meuse, newdata = new.locs, 
		model = vgm(1.34e5, "Sph", 800, nug = 2.42e4), 
		block = c(40,40), nmax = 40)
