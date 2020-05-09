# Lecture 20: Graph algorithms (cont.)

## Depth First Search (DFS)

Depth-ﬁrst search is a generalization of preorder traversal.

Starting at some vertex, $v$, we process $v$ and then recursively traverse all
vertices adjacent to $v$. If this process is performed on a tree, then all tree
vertices are systematically visited in a total of $O(|E|)$ time, since
$|E| = (|V|)$. If we perform this process on an arbitrary graph, we need to be
careful to avoid cycles.

A recursive implementation of DFS on a graph could be as follows. The following
algorithm is just a template for the general style of performing a depth-first
search:

```cpp
void Graph::dfs( Vertex v ) {
    v.visited = true;
    for each Vertex w adjacent to v {
        if (!w.visited) {
            dfs(w);
        }
    }
}
```

