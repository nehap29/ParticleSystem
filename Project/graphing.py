from mpl_toolkits import mplot3d

import numpy as np
import matplotlib.pyplot as plt



xvals = []
yvals = []
zvals = []

with open("data.txt", "r") as a_file:
  for line in a_file:
    stripped_line = line.strip()
    letter_list = stripped_line.split(",")
    xvals.append(float(letter_list[0]))
    yvals.append(float(letter_list[0]))
    zvals.append(floats(letter_list[0]))


fig = plt.figure()
ax = plt.axes(projection='3d')

ax = plt.axes(projection='3d')

ax.plot3D(xvals, yvals, zvals, 'gray')


ax.scatter3D(xvals, yvals, zvals, c=zvals, cmap='Greens');