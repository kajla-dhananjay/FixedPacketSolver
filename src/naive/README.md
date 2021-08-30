# Directory Structure

---

This project has the following files:

- `graph_gen.cpp` : Generates an unweighted, undirected graph probabilistically.
  - Input : Number of edges; Probability of edge existence b/w any 2 nodes.
  - Output : Weighted, undirected graph
  - Status : Complete
- `GTL.cpp` : Converts given graph into its corresponding laplacian matrix.
  - Input : Weighted, undirected graph
  - Output : Laplacian Matrix
  - Status : Complete
- `LTG.cpp`: Converts given laplacian matrix into its corresponding graph.
  - Input : Laplacian Matrix
  - Output : Weighted, undirected graph
  - Status : Complete
- `NaivePLW.cpp` : Implementation of the algorithm given by our paper.
  - Input : Hitting index; unweighted, undirected Graph; Edge weight function, **b** of L**x** = **b**, $\epsilon$: error param
  - Output :  Column vector **x**
  - Status : Incomplete
- `HittingTable.cpp` : Computes the hitting table for the given Laplacian matrix.
  - Input : Transition Matrix P
  - Output : Hitting Table Index
  - Status : WorkInProgress
- `standardSolver.py` : Standard solver for a system of linear equations to check for it's results.
  - Input : Matrix L; Column vector **b**
  - Output : Column vector **x** such that L**x** = **b** 
  - Status : Complete
- `TransitionMatrix.cpp` : Generates Transition matrix for given L, b
  - Input : Column matrix L; Column vector **b**
  - Output : Transition Matrix P
  - Status : Complete

# Input format for PLW-SOLVE 

---

The Input for PLW-SOLVE is formatted as:

```
<G> // G is the undirected, weighted graph corresponding to L
<b> // b is the column vector in Lx = b
<epsilon> // epsilon is the error parameter for the algorithm's termination
```

or as:

```
0 // Denotes Debug/Analysis mode
<G> // G is the undirected, weighted graph corresponding to L
<b> // b is the column vector in Lx = b
<epsilon> // epsilon is the error parameter for the algorithm's termination
<x> // x is the final solution to Lx = b, computed through standard solvers
```

Note the below protocol for the exact formats.

---

# Data Format for PLW-SOLVE Entities

---

## HashMaps

---

### Hitting Table Index

---

```
<number of nodes>
<start node> <end node> <indexed node> <occurences> // 1st Entry
<start node> <end node> <indexed node> <occurences> // 2nd Entry
...
...
...
<start node> <end node> <indexed node> <occurences> // <number of nodes> ^ 3 th Entry
```

---

## Graphs

---

### Unweighted & Undirected 

---

```
<number of nodes>
<number of edges>
<node_1> <node_2> //edge number 1
<node_1> <node_2> //edge number 2
...
...
...
<node_1> <node_2> //edge number <number of edges>
```

---

### Weighted & Undirected

---

```
<number of nodes>
<number of edges>
<node_1> <node_2> <weight> //edge number 1
<node_1> <node_2> <weight> //edge number 2
...
...
...
<node_1> <node_2> <weight> //edge number <number of edges>
```

---

---

## Matrices

---

```
<number of rows>
<number of columns>
<E> <E> <E> <E> ... <E> //space seperated row 1 of the matrix
<E> <E> <E> <E> ... <E> //space seperated row 2 of the matrix
...
...
...
<E> <E> <E> <E> ... <E> //space seperated row <number of rows> of the matrix
```

---

---

## Vectors and Functions

---

### Weight Vector

---

```
<number of edges> //number of edges
<node_1> <node_2> <weight> //edge number 1
<node_1> <node_2> <weight> //edge number 2
...
...
...
<node_1> <node_2> <weight> //edge number <number of edges>
```

---

### Column Vector for system of linear equations (**b**)

---

``` 
<number of entries in vector> //number of vector entries
<element> // First element
<element> // Second element
...
...
...
<element> // <number of entries in vector>th element
```

---

---

## Singleton Values

---

Error Parameter (**$\epsilon$**) 

---

```
<epsilon> //Value of error parameter
```

---

Number of samples($N$)

---

``` 
<Number of samples> //Total number of samples
```

---

---



