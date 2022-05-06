import matplotlib.pyplot as plt
import matplotlib
import math
import numpy as np
l1 = [21, 201, 1311, 9937, 72316, 664366, 5471019]
n1 = [3091.66, 1997.93, 603.445, 289.449, 105.976, 51.0432, 40.7391]
z = [math.log10(x) for x in l1]
e = [2, 3, 4, 5, 6, 7, 9]
matplotlib.rcParams.update({'font.size': 8})
plt.plot(e, n1)
#plt.xticks(np.arange(3,7,1))
#plt.yticks(np.arange(int(min(z)), max(z)+1, 100000.0))
plt.ylabel('l1 norm')
plt.xlabel('epsilon')
plt.title("l1 norm v/s epsilon(Logarithmic) [n=141]")
plt.savefig("l1_ep.png")
