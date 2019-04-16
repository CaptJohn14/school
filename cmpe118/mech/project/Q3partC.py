from array import *
import math

################################################################################
# Declare some stuff
# change these values to whatever you need
################################################################################
fin = 50000  #incoming frequency
n = 2       #order of the filter

G = 1.56            #gain from the filters

#frequency cut off respectively for Low-Pass, High-Pass, and AC couple
Fclo = 28.420 * 1000
Fchi = 21.220 * 1000
Fcac = 0.003386

################################################################################
# Getting magnitudes inbetween filter stages
################################################################################
Glo = (G)                /((1+((fin/Fclo)**(2*n)))**(1/2))
Ghi = (G*((fin/Fchi)**n))/((1+((fin/Fchi)**(2*n)))**(1/2))

Gac = (fin/Fcac)/((1+((fin/Fcac)**2))**(1/2))

################################################################################
# Print out the results
################################################################################
print("Incomming frequency:  ",fin,"Hz")
print("Magnitude after AC coup:   ",Gac)
print("Magnitude after low pass:  ",Glo)
print("Magnitude after high pass: ",Ghi)
print("Magnitude gain:            ", Gac*Glo*Ghi)
