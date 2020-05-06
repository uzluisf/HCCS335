# Graph algorithms (cont.) 

## Network flow problems

Given a directed graph $G = (V, E)$ with edge capacities $c_{v,w}$ ( capacities
could represent the amount of water that could ﬂow through a pipe, for example):

* There are two vertices, the **source** $s$ and the **sink** $t$. 
* At any edge, $(v, w)$, at *most* $c_{v,w}$ units of “ﬂow” may pass.
* At any vertex $v$ (except $s$ and $t$), the total ﬂow coming in must equal the
  total ﬂow going out.
* The **maximum-flow problem** is to determine the maximum amount of flow that
  can pass from $s$ to $t$.

![A graph $W$ (left) and its maximum flow](images/graph-with-maximu-flow.png "")

In the graph $W$ depicted above, maximum flow is $5$, as indicated by the graph
on the right. As required by the problem statement:

* No edge carries more flow than its capacity.
* Vertex $a$ has three units of flow coming in, which it distributes to $c$ and
  $d$, which leaves $0$ for $b$.
* Vertex $d$ takes three units of flow from $a$ and $b$ and combines this,
  sending the result to $t$. It's worthing
  noting a vertex can combine and distribute flow in any manner that it likes,
  as long as edge capacities are not violated and as long as flow conservation
  is maintained (what goes in must come out).

How did we determined the graph $w$'s maximum flow is $5$?

We see that $s$ has edges of capacities $4$ and $2$ leaving it, and $t$
has edges of capacities $3$ and $3$ entering it. So perhaps the maximum ﬂow
could be $6$ instead of $5$, however this isn't the case. 

As shown in the following figure, we can prove that the maximum ﬂow is $5$.

![A cut in graph $W$ partitions the vertices with $s$ and $t$ in different groups. The total edge cost across the cut is 5, proving that a flow of 5 is maximum.](images/graph-with-maximum-flow-cut.png "")

We cut the graph into two parts; one part contains $s$ and some
other vertices; the other part contains $t$. Since flow must cross through the cut,
the total capacity of all edges $(u, v)$ where $u$ is in $s$’s partition and $v$
is in $t$’s partition is a bound on the maximum ﬂow. These edges are $(a, c)$
and $(d, t)$, with total capacity $5$, so the maximum ﬂow cannot exceed $5$.

### A simple maximum-flow algorithm

The problem proceeds in stages:

* Start with our graph, $G$.
* Construct a flow graph $G_f$:
  * This graph tells the flow that has been attained at any stage in the algorithm.
  * Initial state: zero flow at each edge. 
  * By the end of the algorithm, we hope $G_f$ contains a maximum ﬂow.
* Construct a **residual graph** $G_r$:
  * It tells the amount of extra flow that can be pushed at each edge. We can
   calculate this by subtracting the current flow from the capacity for each edge.
  * Initial state: $G_r$ is the same as the input graph $G$.
* We find a path in $G_r$ from $s$ to $t$ (**augmenting path**). The minimum
  edge on this path is the amount of flow that can be added to every edge on
  the path. We do this by adjusting $G_f$ and recomputing $G_r$. When we ﬁnd
  no path from $s$ to $t$ in $G_r$, we terminate.
  * How much flow can we push through an augmenting path?
  * Flow that is as much as the minimum edge on the path.

This algorithm will be run on the graph $W$.

**Initial state:**

![Initial stages of the graph, ﬂow graph, and residual graph](images/max-flow-algorithm-step01.png "")

There are many paths from $s$ to $t$ in the residual graph. Suppose we select
$s$, $b$, $d$, $t$. Then we can send two units of flow through every edge on
this path. 

**Augmenting path s, b, d, t:**

![$G$, $G_f$, $G_r$ after two units of flow added along s, b, d, t](images/max-flow-algorithm-step02.png "")

Next, we might select the path $s$, $a$, $c$, $t$, which also allows two units of flow.

**Augmenting path s, a, c, t:**

![$G$, $G_f$, $G_r$ after two units of flow added along s, a, c, t](images/max-flow-algorithm-step03.png "")

The only path left to select is $s$, $a$, $d$, $t$, which allows one unit of ﬂow.

**Augmenting path s, a, d, t:**

![$G$, $G_f$, $G_r$ after one unit of flow added along s, a, d, t — algorithm terminates](images/max-flow-algorithm-step04.png "")

The algorithm terminates at this point, because $t$ is unreachable from $s$. The
resulting flow of $5$ happens to be the maximum. To see what the problem is,
suppose that with our initial graph, we chose the path $s$, $a$, $d$, $t$. This path
allows three units of flow and thus seems to be a good choice. The result of this
choice, however, leaves only one path from $s$ to t in the residual graph; it
allows one more unit of flow, and thus, our algorithm has failed to find an
optimal solution. This is an example of a greedy algorithm that does not work.
The following figures  shows why the algorithm fails.

![$G$, $G_f$, $G_r$ if initial action is to add three units of flow along $s$, $a$, $d$, $t$ — algorithm terminates after one more step with suboptimal solution](images/max-flow-algorithm-fails.png "")

### Improving the simple maximum-flow algorithm

In order to make this algorithm work, we need to allow the algorithm to change
its mind. To do this, for every edge $(v, w)$ with flow $f_{v,w}$ in the ﬂow
graph, we will add an edge in the residual graph $(w, v)$ of capacity $f_{v,w}$.
In effect, we are allowing the algorithm to **undo** its decisions by sending
flow back in the opposite direction.

We'll start from our original graph from the previous section and augment
progressively following this new strategy.

**Augmenting path s, a, d, t:**

![Graphs after three units of ﬂow added along $s$, $a$, $d$, $t$ using correct algorithm](images/max-flow-algorithm-new-step01.png "")

Notice that in the residual graph, there are edges in both directions between
$a$ and $d$. Either one more unit of flow can be pushed from $a$ to $d$, or
up to three units can be pushed back—we can undo ﬂow.

**Augmenting path s, b, d, a, c, t:**

By pushing two units of flow from $d$ to $a$, the algorithm takes two units of
flow away from the edge $(a, d)$ and is essentially changing its mind. This
results in the following graphs:

![Graphs after two units of flow added along $s$, $b$, $d$, $a$, $c$, $t$ using correct algorithm](images/max-flow-algorithm-new-step02.png "")

There is no augmenting path in this graph, so the algorithm terminates. 

### Analysis

* If capacities are rational numbers, algorithm converges.
* If capacities are integers (positive) and maximum flow is $f$, then the
  algorithm would require at most $f$ steps.
* At each step, an augmenting path needs to be found: $O(|E|)$ with unweighted
  shortest path.
* Total cost (worst case): $O(f \cdot |E|)$
  * Each augmenting path increases flow by 1.
  * Not good and it should be improved.

### Bad example

The running time from the previous analysis is bad. The following is a classical
example of why this is a bad running time:

![The classic bad case for augmenting](images/network-flow-bad-case-for-augmenting.png "")

The maximum ﬂow is seen by inspection to be 2,000,000 by sending 1,000,000 down
each side. Random augmentations could continually augment along a path that
includes the edge connected by a and b. If this were to occur repeatedly,
2,000,000 augmentations would be required, when we could get by with only 2.

**Improvement 1:**

Choose the augmentating path that allows the largest increase
in flow, which is done by solving a weighted shortest-path problem, and 
a single-line modification to Dijkstra's algorithm will do the trick.

* If $cap_{\text{max}}$ is the maximum edge capacity, then 
$O(|E|log\; cap_{\text{max}})$ is the augmentation needed.

* The augmentation time now is $O(|E|log\; |V|)$.

* The total cost is $O(|E|^{2}log\; |V|\; cap_{\text{max}})$.

**Improvement 2:**

Here, we choose the path with least number of edges. With this rule,
each augmenting step computes the *shortest unweighted path* from $s$ to $t$
in the residual graph.

* The augmentation time is now $O(|E|)$.

* The total cost is thus: $O(|E|^{2} |V|)$.


## Minimum spanning trees (MSTs)

This problem involves finding a minimum spanning tree in an undirected graph.
Informally, a **minimum spanning tree (MST)** of an undirected graph $G$
is a tree formed from graph edges that connects all the vertices of $G$ at lowest
total cost. A minimum spanning tree exists if and only if $G$ is connected.

![A graph $G$ and its minimum spanning tree](images/tree-with-mst-example.png "")

The minimum spanning tree is 

* *tree* because it is acyclic,
* it is *spanning* because it covers every vertex,
* and it is *minimum* for the obvious reason.

If we need to wire a house with a minimum of cable (assuming no other
electrical constraints), then a minimum spanning tree problem needs to be
solved.

### Prim's algorithm

One way to compute a minimum spanning tree is to grow the tree in successive
stages. In each stage, one node is picked as the root, and we add an edge, and
thus an associated vertex, to the tree.

Prim’s algorithm is essentially identical to Dijkstra’s algorithm for short- est
paths: 

* Sets of *known* (T) and *unknown* (F) vertices.
* $d_{v}$, weight of shortest edge connecting $v$ with known vertex.
* $p_v$, last vertex to cause a change in $d_v$.
* The update rule states that for after $v$ is selected, we update $d_w$ for all
unknown vertices adjacent to $v$ as $d_{w} = \text{min}(d_{w}, c_{w, v})$.
* The cost (same as Dijkstra's): 
  * $O(|V|^{2})$ (without heaps) for dense graphs.
  * $O(|E|log\; |V|)$ (using binary heaps) for sparse graphs.


The following figure is an example using the Prim's algorithm and an overview of
it through the multiple stages:

![Prim’s algorithm after each stage](images/prim-algorithm-stages-example.png "")

![Initial conﬁguration of table used in Prim’s algorithm](images/prim-algorithm-table-example.png "")

After we've set up the initial configuration of the table, we start
by declaring vertices as *known* and making changes accordingly. The first 
vertice to be labeled as *known* is $v_1$:

![The table after $v_1$ is declared known](images/prim-algorithm-v1-example.png "")

The next vertex selected is $v_4$. Every vertex is adjacent to $v_4$. $v_1$ is
not examined, because it is *known*. $v_2$ is unchanged, because it has $d_v = 2$
and the edge cost from $v_4$ to $v_2$ is $3$; all the rest are updated.

![The table after $v_4$ is declared known](images/prim-algorithm-v4-example.png "")

The next vertex chosen is $v_2$ (arbitrarily breaking a tie). This does not
affect any distances. Then $v_3$ is chosen, which affects the distance in $v_6$:

![The table after $v_2$ and then $v_3$ are declared known](images/prim-algorithm-v2v3-example.png "")

Then vertex $v_7$ is selected, which forces $v_6$ and $v_5$ to be adjusted.
$v_6$ and then $v_5$ are selected, completing the algorithm.

![The table after $v_7$ is declared known](images/prim-algorithm-v7-example.png "")

The final table is as follows:

![The table after $v_6$ and $v_5$ are selected (Prim’s algorithm terminates)](images/prim-algorithm-v6v5-example.png "")

The edges in the spanning tree can be
read from the table: $(v_2 , v_1)$, $(v_3 , v_4)$, $(v_4 , v_1)$, $(v_5 , v_7)$,
$(v_6 , v_7)$, $(v_7 , v_4)$. The total cost is 16.

### Kruskal's algorithm

With this greedy algorithm, we continually select the edges in order of smallest
weight and accept an edge if it does not cause a cycle:

* Maintain a forest (i.e., collection of trees).
  * Initially, each node is a tree.
* By adding one edge, two trees are merged.
* Always pick the minimum edge.
  * Note that you shouldn't connect via ad edge two nodes of the same.

![Kruskal’s algorithm after each stage](images/kruskal-algorithm-stages-example.png "")


![Action of Kruskal’s algorithm on $G$](images/kruskal-algorithm-actions.png "")

**Implementation:**

* Via union-find. Each set of vertices is a tree.
* Initially each vertex is in own set.
* Keep edges into a priority queue (for sparse trees).
* The cost is $O(|E|log|E|)$, however in practice it's much slower.

```cpp
void Graph::kruskal() {
    int acceptedEdges = 0;
    DisjointSet ds(_numVertices);

    PriorityQueue<Edge> pq;
    pq.buildQueue(getAllEdges());
    Edge e;
    Vertex u, v;

    while (acceptedEdges < _numVertices - 1) {
        pq.deleteMin(e); // Edge e = (u, v)
        SetType uSet = ds.find(u);
        SetType vSet = ds.find(v);

        if (uSet != vSet) {
            acceptedEdges++;
            ds.unionSets(uSet, vSet);
        }
    }
}
```

## Applications

* Transportation networks
* Electricity networks
* Interner
* Ecology


