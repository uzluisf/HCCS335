# Lecture 21: Introduction to NP-completeness

## Easy vs Hard

Some problems have solutions and can be solved in a finite amount of time/space. For
instance, the *gcd* algorithm, when applied on two numbers $M$ and $N$, takes
$O(log N)$ time. Since the numbers consist of $log M$ and $log N$ bits,
respectively, the gcd algorithm is really taking time that is linear in the
*amount* or size of input.

However, there are problems that while having solutions, they're still
impossible to solve. These problems are known as **undecidable problems**.
There are several interesting undecidable problems. Among them: 
scheduling, map coloring, bin packing, traveling salesman, puzzle solving
proteing folding, theorem proving, and the halting problem.

## The halting problem

* Assume we have a program `HALT(P, X)` that returns
  * `true` if program `P` halts on input `X`, and
  * `false` if `P` goes into an infinite loop on input `X`.
* Consider the program `LOOP(P)`,
  * `LOOP(P)` halts if `HALT(P, P)` is `false`.
  * `LOOP(P)` infinitely loops if `HALT(P, P)` is `true`.
* What happens when we run `LOOP(LOOP)`? `LOOP(LOOP)` halts if and only if
  `LOOP(LOOP)` doesn't halt. However, this is a logical contradiction. This
  means that our assumption that the program `HALT(P, X)` can exist was false.

The intuitive reason that this problem is undecidable is that such a program
might have a hard time checking itself. For this reason, these problems are
sometimes called *recursively undecidable*.

## The class NP


* This class stands for **nondeterministic polynomial-time**. A
  **deterministic** machine, at each point in time, is executing an instruction,
  and depending on the instruction it then goes to some next instruction, which
  is unique. On the other hand, a **nondeterministic** machine has a choice of
  next steps and thus, it's free to choose any that it wishes, and if one of
  those steps leads to a solution, it will always choose the correct one.

* Regardless how ridiculous it might be as a model, nondeterminism is a useful
  theoretical construct. Nonetheless, it's not as powerful as one might think.
  For instance, undecidable problems are still undecidable, even if
  nondeterminism is allowed.

* A simple way to check if a problem is in NP is to phrase the problem as a
  yes/no question. The problem is in NP if, in polynomial time, we can prove
  that any “yes” instance is correct. We do not have to worry about “no”
  instances, since the program always makes the right choice.

* An example of a NP-problem is the Hamiltonian cycle problem:
  * Is there a simple cycle in an undirected graph that will visit all vertices?
  * A nondeterministic program will start from a vertex, and will guess the
  correct next vertex on the simple cycle if such cycle exists => In polynomial
  time it will provide the result (YES answer).

## NP-complete problems

The **NP-complete problems** is a subset of NP and it contains the hardest
problems. An NP-complete problem has the property that any problem in NP
can be **polynomially reduced** to it.

### Reduction

* Suppose we've two problems $P_1$ and $P_2$.
* $P_1$ can be polynomially reduced to $P_2$ as follows:
  * Provide a mapping so that any instance of $P_1$ can be mapped to an instance
  of $P_2$ (in **polynomial** time).
  * Solve $P_2$.
  * Map the solutions back to $P_1$ (in **polynomial** time). Thus, if you known
  how to solve $P_2$, you can solve $P_1$ with an additional polynomial cost.

A simple practical for this would be a pocker calculator:

 * $P_1$, numbers are entered in decimal.
 * $P_2$, numbers are converted to binary, all calculations are performed in
 binary, and the final answer is converted back to decimal for display.

For $P_1$ to be polynomially reducible to $P_2$, all the work associated with
the transformations must be performed in polynomial time.

A problem $P$ is thus **NP-complete** if all other NP problems are polynomially
reducible to it. If a polynomial solution is found for $P$, then the whole class
NP becomes polynomial (in deterministic machines). 

## Traveling salesman problem 

The **traveling salesman problem** (TSP) can be stated as follows:

>Given a complete graph, $G = (V, E)$, with edge costs, and an integer $K$, is there
>a simple cycle that visits all vertices and has total cost $\leq K$?

Suppose that we already know that the Hamiltonian cycle problem is NP-complete.
Then we must prove that TSP is also NP-complete.

The traveling salesman problem is NP-complete. It is easy to see that a solution
can be checked in polynomial time, so it is certainly in NP. To show that it is
NP-complete, we polynomially reduce the Hamiltonian cycle problem to it. To do
this we construct a new graph, $G'$. $G'$ has the same vertices as $G$. For $G'$,
each edge $(v, w)$ has a weight of 1 if $(v, w) \in G$, and 2 otherwise.
We choose $K = |V|$.

![Hamiltonian cycle problem transformed to traveling salesman problem](images/hamiltonian-to-tsp-problem.png "")

In general, to prove that a new problem is NP-complete, it must be shown to be
in NP, and then an appropriate NP-complete must be transformed into it.

![Proof diagram for NP-completeness](images/proof-np-completeness.png "")

Since proving that a problem is NP-complete requires transforming it from
another NP-complete problem, there must be some NP-complete problem for which
this strategy will not work. Thus, **how was the first NP-complete problem
actually proven to be NP-complete?**

The first problem that was actually proven to be NP-complete was the
**satisfiability problem**, which takes as input a Boolean expression and asks
whether the expression has an assignment to the variables that gives a value of
`true`.

In 1971, Cook showed that satisfiability was NP-complete by directly proving
that all problems that are in NP could be transformed to satisfiability. To do
this, he used the one known fact about every problem in NP:

> Every problem in NP can be solved in polynomial time by a nondeterministic computer.

Cook showed how the actions of a **Turing machine** could be simulated
by an extremely complicated and long, but still polynomial, Boolean formula.
This Boolean formula would be true if and only if the program which was being
run by the Turing machine produced a “yes” answer for its input.

Once satisfiability was shown to be NP-complete, a host of new NP-complete
problems, including some of the most classic problems, were also shown to be
NP-complete.

## Problems that are not in NP?

There exist decidable problems that are not in the NP class. For example,
given a graph determine whether **it doesn't** have a Hamiltonian cycle.
It's not in the NP class because nobody knows how to show, in polynomial time,
that a graph does not have a Hamiltonian cycle. It seems that one must enumerate
all the cycles and check them one by one. Thus the non–Hamiltonian cycle problem
is not known to be in NP.

## Is P = NP?

Note P is a subset of NP (i.e., each problem in P is also in NP). Thus, is there
a problem $X$ that is $NP$ but not $P$?

## Greedy algorithms

We've seen a few greedy algorithms already: Dijkstra's, Prim's, and Kruskal's
algorithms.

Greedy algorithms work in phases. *In each phase, a decision is made that
appears to be good, without regard for future consequences*. Generally, this
means that some **local optimum** is chosen. However, we hope that the local
optimum is equal to the **global optimum**. If this is the case, then the
algorithm is correct; otherwise, the algorithm has produced a **suboptimal
solution**. 

### Simple scheduling problem

* Given jobs $j_1, j_2, \ldots, j_N$ with known completion times $t_1, t_2,
  \ldots, t_N$, respectively.
* We've a single processor, assume non preemptive scheduling so once started
  must run to completion.
* What's the best way to schedule these jobs in order to minimize the average
  completion time? 

**An example:**

Suppose we have four jobs and associated running times as shown in the following
figure:

![Jobs and times](images/jobs-times-scheduling.png "")

We've two possible schedules, as shown below:

![Schedules #1 and #2](images/jobs-times-schedules.png "")

Schedule #2 is arranged by shortest job first, which yields an optimal schedule:

Let the jobs in the schedule be $j_{i_{1}}, j_{i_{2}}, \ldots, j_{i_{N}}$.
The first job finishes in time $t_{i_1}$. The second job finishes after
$t_{i_1} + t_{i_2}$, and the third job after $t_{i_1} + t_{i_2} + t_{i_3}$. From
this, we can see the total costs, $C$, of the schedule is

\begin{align*}
C = \sum^{N}_{k=1}(N-k + 1)t_{i_k} \\
C = (N+1)\sum^{N}_{k=1}t_{i_k} - \sum^{N}_{k=1}k\cdot t_{i_k}\\
\end{align*}


**Multiprocessor case:**

We can extend this problem to the case of several processors. Again we have jobs
$j_1, j_2 , \ldots, j_N$, with associated running times $t_1, t_2, \ldots, t_N$,
and a number $P$ of processors. We will assume without loss of generality that the
jobs are ordered, shortest running time ﬁrst. 

Suppose P = 3, and the jobs shown in the following figure:

![Jobs and times](images/jobs-times-scheduling-multiprocessor.png "")

We've the following optimal solutions:

![An optimal solution for the multiprocessor case](images/optimal-schedules-multiprocessor01.png "")

![A second optimal solution for the multiprocessor case](images/optimal-schedules-multiprocessor02.png "")


