# Lecture 17: Graph algorithms

## Graphs

A **graph** $G = (V, E)$

* consists of a set of **vertices**, $V$, and a set of **edges**, $E$.
* Each edge is a pair $(v, w)$, where $v, w \in V$. Edges are also known as
  **arcs**.
* A vertex $w$ is **adjacent** to $v$ if and only if $(v, w) \in E$.
* Sometimes an **edge** has a third component known as either a **weight** or a
  **costs**.

![Example of a directed graph](images/graph-example.png "")

### Weighted graph

A **weighted graph** or a **network** is a graph in which a number (the weight)
is assigned to each edge. Such weights might represent for example costs,
lengths or capacities, depending on the problem at hand. Such graphs arise in
many contexts, for example in shortest path problems such as the traveling
salesman problem.

![A weighted graph with ten vertices and twelve edges.](images/weighted-network.png "")

### Directed vs undirected graphs

When a graph has an ordered pair of vertexes, it is called a **directed graph**.
The edges of the graph represent a specific direction from one vertex to another.

![Example of directed graph](images/directed-graph-example.png "")

A real world example of a directed graph could be the relantionship between
a money lender and a borrower; if any edge from a person A to a person B
corresponds to A owes money to B, then B doesn't need to owe money to A 
because owing money is not necessarily reciprocated.

When a graph has an unordered pair of vertexes, it is an undirected graph. In
other words, there is no specific direction to represent the edges. The vertexes
connect together by undirected arcs, which are edges without arrows. If there is
an edge between vertex A and vertex B, it is possible to traverse from B to A,
or A to B as there is no specific direction.

![Example of undirected graph](images/undirected-graph-example.png "")

A real example of a undirected graph could be a group of people at a party,
where there is an edge between two people if they shake hands. 
Since person A can shake hands with a person B only if B also shakes hands
with A, then there's no need for directions.

### Path, length, loop, 

A **path** in a graph is a sequence of vertices $w_1, w_2, \ldots, w_N$  such
that $(w_i, wi+1) \in E$ for $1 \leq i < N$.

The **length of a path** is the number of edges on the path, which is equal
to $N-1$. We allow a path from a vertex to itself, if this path contains no
edges, then the path length is $0$. If the graph contains an edge $(v, v)$ from a
vertex to itself, then the path $v, v$ is sometimes referred as a **loop**. 

A **simple path** is a path such that all vertices are distinct, except that the
first and last could be the same.

In a directed graph, a **cycle** is a path of length at least $1$ such that $w_1
= w_N$; this cycle is simple is the path is simple. For undirected graphs,
edges must be distinct. A directed graph is **acyclic** is it has no cycles;
this graph is sometimes referred to by its abbreviation, **DAG**.

## Representation of graphs

### Adjacency matrix

* Two-dimensional array. This is known  as an **adjacency matrix** representation.
* For each edge $(u, v)$, we set $A[u][v]$ to `true`; otherwise the entry in the
  array is `false`.
* If the edge has a weight associated with it, then we can set $A[u][v]$ equal
  to the weight and use either a very large or a very small weight as a
  sentinel to indicate nonexistent edges.
* This representation's merit lies on its extreme simplicity. However, its space
  requirement is $\Theta(|V|^{2})$, which can be quite prohibitive if the graph
  doesn't have many edges.
* An adjacency matrix is an appropriate representation if the graph is
  **dense**, meaning $|E| = \Theta(|V|^{2})$.

![An adjacency matrix representation of a graph](images/adjacency-matrix-representation.png "")

### Adjacency list

* For each vertex, we keep a list of all adjacent vertices.
* The space requirement is then $O(|E| + |V|)$, which is linear in the size of
the graph.
* This type of representation is good for **sparse** graphs.
* If the edges have weights, then this additional information is also stored in
  the adjacency lists.
* Adjacency lists are the standard way to represent graphs. Undirected graphs
  can be similarly represented; each edge (u, v) appears in two lists, so the
  space usage essentially doubles.

![An adjacency list representation of a graph](images/adjacency-list-representation.png "")

## Topological sort

A **topological sort** is an ordering of vertices in a directed acyclic graph, such
that if there is a path from $v_i$ to $v_j$, then $v_j$ appears after $v_i$
in the ordering.

* Topological sort is defined *only* for directed graphs.
* Only possible for **DAG**; if the graph has a cycle, since for two vertices $v$
  and $w$ on the cycle, $v$ precedes $w$ and $w$ precedes $v$.
* The ordering is not necessarily unique; any legal ordering will do.


### Finding a topological ordering

A simple algorithm to find a topological ordering is

1. Find any vertex with no incoming edges.
2. Then print this vertex, and remove it, along with its edges, from the graph.
3. Then, we apply this same strategy to the rest of the graph.

To formalize this, we deﬁne the **indegree** of a vertex $v$ as the number of
edges $(u, v)$. We compute the indegrees of all vertices in the graph. Assuming that
the indegree for each vertex is stored, and that the graph is read into an
adjacency list, we can then apply the following pseudocode algorithm to
generate a topological ordering:

```cpp
/*

* findNewVertexOfIndegreeZero scans the array of vertices looking for
a vertex with indegree 0 that has not already been assigned a topological
number. It returns NOT_A_VERTEX if no such vertex exists; this indicates that
the graph has a cycle.

*/
void Graph::topsort():
    for (int counter = 0; counter < NUM_VERTICES; counter++):
        Vertex v = findNewVertexOfIndegreeZero()
        if (v == NOT_A_VERTEX):
            throw CycleFoundException
        v.topNum = counter

        // decrement each vertex's indegree
        for each Vertex w adjacent to v:
            w.indegree--;
```

Because `findNewVertexOfIndegreeZero` is a simple sequential scan of the array
of vertices, each call to it takes $O(|V|)$ time. Since there are $|V|$ calls,
the running time of the algorithm is $O(|V|^{2})$. **Is it possible to do
better?**

There are a few things that can improve the algorithm:

* The sequential scan through the array of vertices gives a poor running
  time. We can remove this inefﬁciency by keeping all the (unassigned) vertices
  of indegree $0$ in a special box. Either a queue or a stack can be used to
  implement the box.
* Then all vertices of indegree $0$ are placed on an initially empty queue.
* While the queue is not empty, a vertex $v$ is removed, and all vertices adjacent
  to v have their indegrees decremented. A vertex is put on the queue as soon as
  its indegree falls to 0. 
* The topological ordering then is the order in which the vertices dequeue. 

A pseudocode implementation of the new and better algorithm could be as follows:

```
void Graph::topsort:
    Queue<Vertex> q
    int counter = 0

    q.makeEmpty()
    for each Vertex v
        if (v.indegree == 0)
            q.enqueue(v)

    until q.isEmpty():
        Vertex v = q.dequeue() # remove element
        v.topNum = counter     # assign next number

        for each Vertex w adjacent to v:
            if --w.indegree == 0:
                q.enqueue(w)

    if counter != NUM_VERTICES:
        throw CycleFoundException
```

The time to perform this algorithm is $O(|E| + |V|)$ if adjacency lists are used.
This is apparent when one realizes that the body of the `for` loop is executed at
most once per edge. Computing the indegrees can be done with the following code;
this same logic shows that the cost of this computation is $O(|E| + |V|)$, even
though there are nested loops.

The queue operations are done at most once per vertex, and the other
initialization steps, including the computation of indegrees, also take time
proportional to the size of the graph.

## Shortest path algorithms

The input for the shortest-path problems is a **weighted graph**:

Associated with each edge $(v_i, v_j)$ is a cost $c_{i, j}$ to traverse the
edge. The cost of a path $v_1v_2\ldots v_N$ is $\Sigma^{N-1}_{i=1}c_{i, i+1}$.
This is known as the **weighted path length**. The **unweighted path length**
is merely the number of edges on the path, namely, $N-1$

>**Single-Source Shortest-Path Problem:**
>
>Given as input a weighted graph, $G = (V, E)$, and a distinguished vertex, $s$, ﬁnd
>the shortest weighted path from $s$ to every other vertex in $G$.

![A directed graph G](images/directed-graph-G.png "")

In the directed graph $G$ (from the figure), the shortest weighted path
from $v_1$ to $v_6$ has a weight (or cost) of $6$ which goes from $v_1$ to $v_4$
to $v_7$ to $v_6$. The shortest unweighted path between these two
vertices is $2$ (number of edges $N = 3$, $N-1 = 3-1 = 2$). Notice graph $G$
has no edges of negative weight.

Negative edges can cause a problem known as a **negative-cost cycle**:

* Always ever shortest path by following loops that are arbitrarily long. This
causes the shortest path between two points to be undefined.

* When a negative-cost cycle is present in the graph, the shortest paths are not
defined.

* Negative-cost edges aren't necessarily bad, as the cycles are, but their presence
seems to make the problem harder.

![A graph H with a negative-cost cycle](images/graph-negative-cost-cycle.png "")

In the graph $H$, the path $v_5$ to $v_4$ has cost $1$, but a shorter path
exists by following the loop $v_5$, $v_4$, $v_2$, $v_5$, $v_4$, which has cost
$-5$. However, the path isn't still the shortest path because we could still
in the loop arbitrarily long. Thus, the shortest path between these two points
is undefined.

We can model airplane or other mass transit routes by graphs and use a shortest-
path algorithm to compute the best route between two points. In this and many
practical applications, we might want to find the shortest path from one vertex,
$s$, to only one other vertex, $t$. Currently there are no algorithms in which
ﬁnding the path from $s$ to one vertex is any faster (by more than a constant
factor) than finding the path from $s$ to all vertices.

### Unweighted shortest paths


For this, we consider an unweighted graph $K$ which is an input parameter,
and we would like to find the shortest path from $s$ to all vertices. We 
are only interested in the number of edges contained on the path, so 
*there are no weights on the edges*.

![An unweighted directed graph K](images/unweighted-directed-graph-k.png "")

Suppose we choose $s$ to be $v_3$:

* Immediately, we can tell that the shortest  path from $s$ to $v_3$ is then a
  path of length $0$. We can mark this information on the vertex $v_3$ by
  placing a $0$ beside it.

* Now we can start looking for all vertices that are a distance 1 away from $s$.
  These can be found by looking at the vertices that are adjacent to $s$. If we
  do this, we see that $v_1$ and $v_6$ are one edge from $s$. Thus, $v_1$ and
  $v_6$ has a $1$ beside them.

* We can now find vertices whose shortest path from $s$ is exactly $2$, by
  finding all the vertices adjacent to $v_1$ and $v_6$ (the vertices at distance $1$),
  whose shortest paths are not already known. This search tells us that the
  shortest path to $v_2$ and $v_4$ is $2$.

* Finally we can find, by examining vertices adjacent to the recently evaluated
  $v_2$ and $v_4$, that $v_5$ and $v_7$ have a shortest path of three edges.
  All vertices have now been calculated, and we end up with graph $K'$, which is
  final result of the algorithm.

![Graph K' with final shortest paths](images/unweighted-directed-graph-k-prime-final.png "")

This strategy for searching a graph is known as **breadth-first search**. It
operates by processing vertices in layers: The vertices closest to the start are
evaluated ﬁrst, and the most distant vertices are evaluated last. This is much
the same as a level-order traversal for trees. Translated to pseudocode, the
basic algorithm looks as follows:

```cpp
void Graph::unweighted( Vertex s ):
    # For each vertex, we will keep track of three pieces of information.
    # First, we will keep its distance from s in the entry v. Initially
    # all vertices are unreachable except for s, whose path length is 0.
    for each Vertex v:
        v.dist = INFINITY
        v.know = false

    # set s's path length
    s.dist = 0

    for (int currDist = 0; currDist < NUM_VERTICES; currDist++):
        for each Vertex v:
            if !v.known && v.dist == currDist:
                # vertex processed so mark as done
                v.known = true;                 
                for each Vertex w adjacenct to v:
                    if w.dist == INFINITY:
                        w.dist = currDist + 1
                        w.path = v

```

The running time of the algorithm is $O(|V|^{2})$, because of the doubly nested
for loops. However, there are still some inefficiencies and the algorithm
could be improved further.

### Dijkstra's Algorithm

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

The pseudocode for the Dijkstra's algorithm is as follows:

```cpp
void Graph::dijkstra( Vertex s ):
    for each Vertex v:
        v.dist = INFINITY
        v.known = false

    s.dist = 0

    while there is an unknown distance vertex:
        Vertex v = smallest unknown distance vertex
        v.known = true

        for each Vertex w adjacent to v:
            unless w.known:
                DistType cvw = cost of edge from v to w
                if v.dist + cvw < w.dist:
                    # update w
                    decrease(w.dist to v.dist + cvw)
                    w.path = v
```


