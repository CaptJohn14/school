from array import *
import math

################################################################################
# Declare some stuff
# change these values to whatever you need
################################################################################
n = 1   #order/2

G = array('f', [1.586])          #gain, depends on order for chevy
Fmul_lo = array('f', [1.000])    #Fmul values for low pass
Fmul_hi = array('f', [1.000])

Fcut_lo = float(29*1000)             #Cutoff frequency for low pass
Fcut_hi = float(21*1000)

C = float(0.01*(10**-6))                #Capcitor values for C1 & C2

R4 = 10000
################################################################################
# get R3 from R4 and G
################################################################################
R3 =  [0]*n                             #making array 3 element

for i in range (0, n):                  #values for R3 for BOTH low and high pass
    R3[i] = (G[i]-1) * R4

################################################################################
# get values for R
################################################################################
R_lo  = [0]*n
R_hi  = [0]*n #

for i in range(0, n):
    R_lo[i] = 1/(2*math.pi*C*Fmul_lo[i]*Fcut_lo)
    R_hi[i] = 1/(2*math.pi*C*Fmul_hi[i]*Fcut_hi)

################################################################################
# Print out the results
################################################################################
print("********************")
print("Cutoffs: ")
print("********************")
print(Fcut_hi/1000, "k to ", Fcut_lo/1000,"k")
print("")

print("********************")
print("LOW AND HIGH: ")
print("********************")
print("R3: ")
for x in R3:
    print("    ",x/1000,"k")
print("R4: ",R4/1000,"k")
print("C1 & C2: ",C*1000000,"uF")
print("")

print("********************")
print("LOW Pass: ")
print("********************")
print("R1 & R2:")
for x in R_lo:
    print("    ",x/1000,"k")
print("")

print("********************")
print("HIGH pass: ")
print("********************")
print("R1 & R2:")
for x in R_hi:
    print("    ",x/1000,"k")
