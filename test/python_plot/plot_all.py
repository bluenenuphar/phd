
"""
Plot the evolution of every variables
"""

import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import sys
from mpl_toolkits.mplot3d import Axes3D

plt.close('all')
fname = "data/data_plane.txt"
data = np.loadtxt(fname,dtype=float)

x = data[:,0]
y = data[:,1]
z = data[:,2]
V = data[:,3]
gamma = data[:,4]
khi = data[:,5]
alpha = data[:,6]
beta = data[:,7]
sigma = data[:,8]
t = data[:,9]

ax1 = plt.subplot(2,2,1)
#plt.plot(t,x,label='x')
#plt.plot(t,y,label='y')
plt.plot(t,z,label='z',color='#6699ff')
plt.legend(loc='upper left')

ax2 = plt.subplot(2,2,2)
plt.plot(t,V,label='V',color='#6699ff')
plt.legend(loc='upper left')

ax3 = plt.subplot(2,2,3)
plt.plot(t,gamma,label='gamma',color='#6699ff')
plt.plot(t,khi,label='khi',color='#ff6600')
plt.legend(loc='upper left')

ax3 = plt.subplot(2,2,4)
plt.plot(t,alpha,label='alpha',color='#6699ff')
plt.plot(t,beta,label='beta',color='#ff6600')
plt.plot(t,sigma,label='sigma',color='#00cc66')
plt.legend(loc='upper left')

plt.show()

