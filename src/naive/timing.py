import matplotlib.pyplot as plt
import matplotlib
import numpy as np
l1 = [8792, 1118, 455, 415]
z = [x/40 for x in l1]
e = [3, 4, 5, 6]
matplotlib.rcParams.update({'font.size': 8})
plt.plot(e, z)
plt.xticks(np.arange(3,7,1))
plt.yticks(np.arange(int(min(z)), max(z)+1, 10.0))
plt.ylabel('L1 norm of (Lx-b)')
plt.xlabel('epsilon(in 10^{-x})')
plt.title("L1 norm v/s epsilon")
plt.savefig("l1eps.png")
