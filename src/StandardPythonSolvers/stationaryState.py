import numpy as np
import numpy.linalg as la
ans = int(input())
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
print(1/rows)
try:
    x = la.solve(A.T.dot(A), A.T.dot(zeros))
    #x = np.linalg.solve(A, zeros)
    y = np.matmul(np.transpose(P),x)
    #print(x.size)
    #for i in x:
    #    print(i)
    print(x[ans])

except np.linalg.LinAlgError as err:
    print("Error: " + str(err))
