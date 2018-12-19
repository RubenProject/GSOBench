import matplotlib.pyplot as plt
import numpy as np

plt.figure()
for i in range(0, 50):
    x=[]
    y=[]
    fp = open("vis_data.txt","r")
    for line in fp:
        currentline = line.split(",")
        if int(currentline[0]) == i:
            x.append(float(currentline[1]))
            y.append(float(currentline[2].rstrip('\n')))
    plt.plot(x[0],y[0], 'ko')
    plt.plot(x,y,'g-')
    del x
    del y

plt.axis([-5, 5, -5, 5])
plt.savefig('plot_test.png')
plt.show()

