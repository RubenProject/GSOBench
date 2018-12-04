import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt
import numpy as np

plt.hold(True)
for i in range(0, 100):
    print(i)
    x=[]
    y=[]
    fp = open("vis_data.txt","r")
    for line in fp:
        currentline = line.split(",")
        #print(currentline[0])
        if int(currentline[0]) == i:
            #print("hoi")
            x.append(float(currentline[1]))
            y.append(float(currentline[2].rstrip('\n')))
    print(x)
    print(y)
    plt.plot(x[1],y[1],'bo')
    plt.plot(x,y,'r-')
    del x
    del y

plt.axis([-5, 5, -5, 5])
plt.savefig('plotding.pdf')
