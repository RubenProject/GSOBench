from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter
import numpy as np

import plotly
import plotly.plotly as py
import plotly.graph_objs as go

plotly.tools.set_credentials_file(username='mickisgay', api_key='FaiGQJE3E8ldYHgn5tC6')

fp = open('sphere.txt', 'r')
data = fp.readlines()

x = []
y = []
z = []

for line in data[1:]:
    t = line.split(',')
    x.append(t[0])
    y.append(t[1])
    z.append(t[2])

X = np.array(x)
Y = np.array(y)
Z = np.array(z)


trace = go.Heatmap(z=Z, x=X, y=Y)
data=[trace]
py.plot(data, filename='labelled-heatmap')
