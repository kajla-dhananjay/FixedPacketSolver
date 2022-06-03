# Instructions to run


# Input format for PLW-SOLVE 


The Input for PLW-SOLVE is formatted as:

```
<G> // G is the undirected, weighted graph corresponding to L
<b> // b is the column vector in Lx = b
<epsilon> // epsilon is the error parameter for the algorithm's termination
```


Note the below protocol for the exact formats.

---

# Data Format for PLW-SOLVE Entities


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

