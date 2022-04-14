import matplotlib.pyplot as plt
import matplotlib
import numpy as np
l1 = [0.002, 0.016, 0.049, 3.283, 21.597]
z = [x for x in l1]
e = [5, 40, 140, 1174, 2888]
matplotlib.rcParams.update({'font.size': 8})
plt.plot(e, z)
#plt.xticks(np.arange(3,7,1))
#plt.yticks(np.arange(int(min(z)), max(z)+1, 10.0))
plt.ylabel('time')
plt.xlabel('Number of nodes')
plt.title("time v/s nodes (eps = 1/n^2)")
plt.savefig("time.png")
