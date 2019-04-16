clear all
close all
clc

EllipseXYData

one = ones(150,1);
y2 = Ymeas .* Ymeas;
v = [2*Xmeas, -y2, 2.*Ymeas, one];

something = Xmeas .* Xmeas;

uh = v\something

x0 = uh(1)
y0 = uh(3)/(2*uh(2))

a2 = uh(4) + uh(2)*y0 + uh(1)*uh(1)

a = sqrt(a2)

b2 = a2/uh(2)

b = sqrt(b2)

x = (Xmeas - x0)/a
y = (Ymeas - y0)/b

[at, bt] = CalibrateEllipseData2D(x, y, 1 , 1)
[xc, yc] = CorrectEllipseData2D(x, y, at, bt)

scatter(xc, yc)
