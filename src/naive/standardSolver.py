import numpy as np
rows = int(input())
columns = int(input())
matrix = []

for i in range(rows):
    r = list(map(int, input().split()))
    matrix.append(r)
col = int(input())

if(col != columns):
    print("Bad System of Linear Equations")
    exit(1)

b = []

for i in range(col):
    b.append(int(input()))

L = np.array(matrix)
bnp = np.array(b)

try:
    x = np.linalg.solve(L, b)
    print(x.size)
    for i in x:
        print(i)

except np.linalg.LinAlgError as err:
    print("Error: " + str(err))
