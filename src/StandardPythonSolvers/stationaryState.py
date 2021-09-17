import numpy as np
import numpy.linalg as la
rows = int(input())
columns = int(input())
matrix = []

for i in range(rows):
    r = list(map(float, input().split()))
    matrix.append(r)

P = np.array(matrix)
A = np.subtract(P,np.identity(rows))
last = np.ones(rows)
Pp = np.transpose(A)
A = np.append(Pp, [last], axis = 0)
A = np.r_[ Pp, [last]] 
zeros = np.zeros(rows)
zeros = np.append(zeros, [1])

try:
    x = la.solve(A.T.dot(A), A.T.dot(zeros))
    #x = np.linalg.solve(A, zeros)
    y = np.matmul(np.transpose(P),x)
    print(x.size)
    for i in x:
        print(i)

except np.linalg.LinAlgError as err:
    print("Error: " + str(err))
