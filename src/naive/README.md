# Directory Structure

---

This project has the following files:

- `formatter.cpp`: Converts raw data into required format.
  - Input : Raw data (List of edges)
  - Output : Formatted data (Input for NaivePLW)
  - Status : Complete
- `NaivePLW.cpp` : Implementation of the algorithm given by our paper.
  - Input : Hitting index; unweighted, undirected Graph; Edge weight function, **b** of L**x** = **b**, $\epsilon$: error param
  - Output :  Column vector **x**
  - Status : Working on Phase-2

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



