# file = open('gyrodata.txt', 'r')
# x =   open('GyroX.txt', 'w')
# y =   open('GyroY.txt', 'w')
# z =   open('GyroZ.txt', 'w')
#
# for line in file:
#     if(line[0]=='x'):
#         x.write(line[1:len(line)])
#     elif(line[0]=='y'):
#         y.write(line[1:len(line)])
#     elif(line[0]=='z'):
#         z.write(line[1:len(line)])
#
#
# x.close()
# y.close()
# z.close()
# file.close()

read = open('part5Z.txt', 'r')
wr =   open('uh.txt', 'w')
for line in read:
    x = (int(line)-71.55)/131
    wr.write(str(x))
    wr.write("\n")

read.close()
wr.close()
