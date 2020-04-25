# Lecture 18: Graph algorithms (cont.)

## Weighted shortest path: Dijkstra

If the graph is weighted, the problem (apparently) becomes harder, but we can
still use the ideas from the unweighted case.

We keep all of the same information as before. Thus, each vertex is marked as
either *known* or *unknown*. A tentative distance $d_v$ is kept for each vertex,
as before. This distance turns out to be the shortest path length from $s$ to
$v$ using only known vertices as intermediates. As before, we record $p_v$,
which is the last vertex to cause a change to $d_v$.

The general method to solve the single-source shortest-path problem is known as
**Dijkstra’s algorithm**, which is a prime example of a greedy algorithm.
Greedy algorithms generally solve a problem in stages by doing what appears to
be the best thing at each stage.

### General description

Suppose we want to find a shortest path from a given nodesto other nodesin a
network (one-to-all shortest path problem).

* Dijkstra’s algorithm solves such a problem:
  * It finds the shortest path from a given node $s$ to all other nodes inthe network.
  * Node $s$ is called a starting node or an initial node.

* How is the algorithm achieving this?
  * Dijkstra’s algorithm starts by assigning some initial values for
    the distances from node $s$ and to every other node in the network.
  * It operates in steps, where at each step the algorithm improves the distance
    values.
  * At each step, the shortest distance from node $s$ to another node is determined.

### Formal description

The algorithm characterizes each node by its state; the state of a node consists
of two features: 

* **distance value**, a scalar representing an estimate of its distance from
  node $s$.
* **status label**, an attribute specifying whether the distance value of a node
  is equal to the shortest distance to node $s$ or not.
  * The status label of a node is **permanent** (or known) if its distance
  is equal to the shortest distance from node $s$.
  * Otherwise, the status label of a node is **temporary** (or unknown).

The algorithm maintains and step-by-step updates the states of the nodes. At
each step, one node is designate as **current**.

### Notation

We'll use the following notation for the rest of the text:

* $d_{\ell}$ denotes the distance value of a node $\ell$.
* $p$ or $t$ denotes the status label of a node, where $p$ stands for permanent
  and $t$ stands for temporary.
* $c_{ij}$ is the cost of traversing link $(i, j)$ as given by the problem.

The state of a node $\ell$ is the ordered pair of its distance value $d_{\ell}$
and its status label.

### Algorithm steps

**Step 1.** *Initialization*

* Set node $s$ to distance value $0$, and label it as **permanent**. Thus, the
  state of node $s$ is $(0, p)$.
* Assign to every node a distance value of $\infty$ and label them as
  **temporary**. Thus, the state of every other node $(\infty, t)$.
* Designate the node $s$ as the **current** node.

**Step 2.** *Distance Value Update and Current Node Designation Update*

Let $i$ be the index of the current node.

1. Find the set $J$ of **nodes with temporary labels** that can be reached from
   the current node $i$ by a link $(i, j)$. **Update the distance values of
   these nodes**.
   * For each $j \in J$, the distance value $d_j$ of node $j$ is updated as
   follows $\text{new } d_{j} = \text{ min }\{d_j, d_i + c_{ij}\}$, where
   $c_{ij}$ is the cost of link $(i, j)$, as given in the network problem.

2. Determine a node $j$ that has the smallest distance value $d_j$ among all
   nodes $j \in J$, find $j*$ such that $\text{min}_{j\in J}d_{j} = d_{j*}$.

3. Change the label of node $j*$ to **permanent** and designate this node as the
   **current node**.

**Step 3.** *Termination Criterion*

* If all nodes that can be reached from node $s$ have been permanently labeled,
  then stop - we are done.

* If we cannot reach any temporary labeled node from the current node, then all
  the temporary labels become permanent - we are done.

* Otherwise, go to Step 2.


### Example

Given the graph $M$, we want to find the shortest path from node $1$ to all
other nodes using Dijkstra's algorithm.

![Graph M](images/dijkstra-example01.png "")

**Step 1:**

* Node $1$ is designated as the current node.
* The state of node $1$ is $(0, p)$.
* Every other node has state $(\infty, t)$.

![Graph M after initialization](images/dijkstra-example02.png "")

**Step 2:**

* Nodes $2$, $3$, and $6$ can be reached from the current node $1$.
* Update distance values for these nodes:
  * $d_2 = min\{\infty, 0+7\} = 7$
  * $d_3 = min\{\infty, 0+9\} = 9$
  * $d_6 = min\{\infty, 0+14\} = 14$
* Now, among the nodes $2$, $3$, and $6$, node $2$ has the smallest distance
  value.
* The status label of node $2$ changes to permanent, so its state is $(7, p)$,
  while the status of $3$ and $6$ remains temporary.
* Node $2$ becomes the current node.

![Graph M: Node 2 changes to permanent and becomes the current node](images/dijkstra-example03.png "")

**Step 3:**

We are not done, not all nodes have been reached from node $1$, so weperform
another iteration (back to Step 2).

**Step 2 (again):**

* Nodes $3$ and $4$ can be reached from the current node $2$.
* Update distance values for these nodes:
  * $d_3 = min\{9, 7+10\} = 9$
  * $d_4 = min\{\infty, 7+15\} = 22$
* Now, between the nodes $3$ and $4$, node $3$ has the smallest distance value.
* The status label of node $3$ changes to permanent, while the status of $6$
  remains temporary.
* Node $3$ becomes the current node.

We're not done (Step 3 fails), so we perform another Step 2.

![Graph M: Node 3 changes to permanent and becomes the current node](images/dijkstra-example04.png "")

**Step 2 (again):**

* Nodes $6$ and $4$ can be reached from the current node $3$.
* Update distance values for these nodes:
  * $d_4 = min\{22, 9+11\} = 20$
  * $d_6 = min\{14, 9+2\} = 11$
* Now, between the nodes $6$ and $4$, node $6$ has the smallest distance value.
* The status label of node $6$ changes to permanent, while the status $4$
  remains temporary.
* Node $6$ becomes the current node.

We're not done (Step 3 fails), so we perform another Step 2.

![Graph M: Node 6 changes to permanent and becomes the current node](images/dijkstra-example05.png "")

**Step 2 (once again):**

* Node $5$ can be reached from the current node $6$.
* Update distance value for node $5$:
  * $d_5 = min\{\infty, 11+9\} = 20$
* Now, node $5$ is the only candidate, so its status changes to permanent.
* Node $5$ becomes the current node.

From node $5$ we cannot reach any other node. Hence, node $4$ gets permanently
labeled and we're done.

![Graph M: Both 5 and 4 changes to permanent](images/dijkstra-example06.png "")

### Importance of Dijkstra's algorithm

Many more problems than you come across can be cast as shortest path problems,
which make Dijkstra’s algorithm a powerful and general tool. For example:

* Dijkstra’s algorithm is applied to automatically find directions
  betweenphysical locations, such as driving directions on websites like
  Mapquestor Google Maps.
* In a networking or telecommunication applications, Dijkstra’s algorithm has
  been used for solving the min-delay path problem (which is the shortest path
  problem). For example in data network routing, the goal is to find the path
  for data packets to go through a switching network with minimal delay.
* It is also used for solving a variety of shortest path problems arising in
  plant and facility layout, robotics, transportation, and VLSI (Very Large
  Scale Integration) design.

## Graphs with negative edge costs

## Acyclic graphs

## Network flow problems
