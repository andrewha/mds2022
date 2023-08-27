/**
 * @file rand_graph_lib.c
 * @author Andrei Batyrov (arbatyrov@edu.hse.ru)
 * @brief Enumerate and calculate the probability of connectedness of random graphs constructed with the Erdős-Rényi and Gilbert models
 * @version 0.1
 * @date 2023-08-26
 *
 * @copyright Copyright (c) 2023
 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

/**
 * @brief Macro for `bernoulli_rv()` non-success return value.
 */
#define NON_SUCCESS false

/**
 * @brief Macro for `bernoulli_rv()` success return value.
 */
#define SUCCESS true

/**
 * @struct Graph_Params
 * @brief Graph parameters structure, declared as a new type.
 *
 * @var GP::n
 * Number of labeled vertices.
 *
 * @var GP::m_max
 * Maximal possible number of edges in the complete graph with \f$ n \f$ labeled vertices:
 *
 * \f$ \displaystyle m_{max} = \binom{n}{2} = \frac{n  (n - 1)}{2} \f$ (complete graph).

 * @var GP::m_min
 * Minimal number of edges in a connected graph (tree):

 * \f$ m_{min} = n - 1 \f$ (tree).
 *
 * @var GP::m_crit
 * Number of edges as connectedness threshold:
 *
 * \f$ \displaystyle m_{crit} = \binom{n - 1}{2} = \frac{(n - 1)(n - 2)}{2} \f$.
 *
 * @var GP::n_graphs
 * Maximal possible number of graphs with \f$ n \f$ labeled vertices:
 *
 * \f$ n_{graphs} = 2^{m_{max}} \f$.
 *
 * @var GP::n_trees
 * Maximal possible number of trees with \f$ n \f$ labeled vertices, p.292, Erdős \cite 1 :
 *
 * \f$ n_{trees} = n^{n - 2} \f$.
 *
 * @var GP::p_edge
 * Edge probability \f$ 0 \leq p \leq 1 \f$
 */
typedef struct Graph_Params
{
    unsigned short n, m_min, m_max, m_crit;
    unsigned long long n_graphs, n_trees;
    double p_edge;
} GP;

/**
 * @brief Find binomial coefficient \f$ C(n, k) \f$.
 *
 * @param n Integer number \f$ n \f$
 * @param k Integer number \f$ k \f$
 * @return Binomial coefficient
 *
 * Implementation notes:
 * 1. Optimized algorithm without explicit calculation of factorial.
 * 2. Integer overflow unsafe, since the result size is not checked during calculation and before return.
 * 3. Time complexity: \f$ \mathcal{O}(r) \f$, where \f$ r = \min(k, n - k) \f$.
 */
unsigned long long binom(unsigned short n, unsigned short k)
{
    unsigned short i;
    unsigned long long coeff = 1;

    /* Special cases */
    if (k == 0 | k == n)
    {
        return coeff;
    }
    if (k == 1 | k == n - 1)
    {
        return n;
    }
    if (k == 2 | k == n - 2)
    {
        return n * (n - 1) / 2;
    }

    /* General case */
    if (k > n - k)
    {
        k = n - k;
    }
    for (i = 0; i < k; i++)
    {
        coeff *= (n - i);
        coeff /= (i + 1);
    }
    return coeff;
}

/**
 * @brief Populate and return graph parameters as a structure
 *
 * @param n GP::n
 * @return Graph parameters as per structure Graph_Params
 *
 * Implementation notes:
 * 1. Integer overflow unsafe, since the rhs's size is not checked before assignment.
 * 2. Time complexity: \f$ \max[\mathcal{O}( \f$ `binom()` \f$ ), \mathcal{O}( \f$ `pow()` \f$ )] \f$.
 */
GP calc_graph_params(unsigned short n, double p_edge)
{
    GP gp;
    gp.m_max = binom(n, 2);
    gp.m_min = n - 1;
    gp.m_crit = binom(n - 1, 2);
    gp.n_graphs = pow(2, gp.m_max);
    gp.n_trees = pow(n, n - 2);
    gp.p_edge = p_edge;
    return gp;
}

/**
 * @brief Return Bernoulli distributed random variable.
 *
 * @param p Probability of a single outcome
 * @return True (1) or false (0)
 *
 * Implementation notes:
 * 1. Simulates Bernoulli process and return either 1 or 0. If \f$ p = 0.5 \f$, return values will simulate tosses of a fair coin (H = 1, T = 0).
 * 2. Uniformly distributed random variable with `p = 1 / RAND_MAX` is obtained with `rand()` which is then transformed into Bernoulli distributed random variable.
 * 3. Assumes that `RAND_MAX = 32767`, thus the result of `rand()` is stored in the unsigned short variable.
 * 4. Time complexity: \f$ \mathcal{O}( \f$ `rand()` \f$ ) \f$.
 *
 * Usage notes:
 * 1. Init random seed by calling `srand(time(NULL))`, if needed, before calling `bernoulli_rv()`.
 */
bool bernoulli_rv(double p)
{
    bool outcome = NON_SUCCESS;
    unsigned short quantile_p;
    unsigned short uniform_rv;

    if (p < 0.0)
    {
        p = 0.0;
    }
    if (p > 1.0)
    {
        p = 1.0;
    }
    quantile_p = p * RAND_MAX;
    uniform_rv = rand();
    if (uniform_rv <= quantile_p)
    {
        outcome = SUCCESS;
    }
    return outcome;
}

/**
 * @brief Print vertex combinations (all possible edge pairs).
 *
 * @param E Pointer to memory allocation with 2d array \f$ (m_{max} \times 2) \f$ -- edge pairs
 * @param m_max GP::m_max
 * 
 * Implementation notes:
 * 1. Time complexity: \f$ \mathcal{O}(m_{max}) \f$.
 */
void print_vertex_combinations(unsigned short **E, unsigned short m_max)
{
    unsigned short i;
    printf("Vertex combinations:\n");
    for (i = 0; i < m_max; i++)
    {
        printf("(%hu, %hu)\n", E[i][0], E[i][1]);
    }
}

/**
 * @brief Print graph adjacencies for each vertex (row).
 *
 * @param G Pointer to memory allocation with 2d array \f$ (n \times n) \f$ -- graph in the form of adjacencies arrays
 * @param n GP::n
 * 
 * Implementation notes:
 * 1. Time complexity: \f$ \mathcal{O}(n^2) \f$.
 */
void print_graph(unsigned short **G, unsigned short n)
{
    unsigned short i, j;
    printf("Graph adjacencies:\n");
    for (i = 0; i < n; i++)
    {
        printf("%u: [", i);
        for (j = 0; j < n; j++)
        {
            printf(" %u ", G[j][i]);
        }
        printf("]\n");
    }
}

/**
 * @brief Construct a random \f$ \mathcal{G}(n, p) \f$ graph with \f$ n \f$ vertices and \f$ p m_{max} \f$ edges on average.
 *
 * @param n GP::n
 * @param p_edge GP::p_edge
 * @param m_max GP::m_max
 * @return Pointer to memory allocation with 2d array \f$ (n \times n) \f$ -- graph in the form of adjacencies arrays
 * 
 * Implementation notes:
 * 1. Time complexity: \f$ \mathcal{O}(m_{max} \f$ `bernoulli_rv()` \f$ ) \f$.
 * 
 * Usage notes:
 * 1. `destroy_rand_gnp()` must be called immediately after the created graph is no longer used to free memory.
 */
unsigned short **construct_rand_gnp(unsigned short n, double p_edge, unsigned short m_max)
{
    /* Construct a random G(n, p) graph */
    unsigned short **E;
    unsigned short **G;
    unsigned short i, j, k, u, v;

    /* (1) Allocate memory to store edge pairs container array and initialize it with zeros. */
    E = calloc(m_max, sizeof(unsigned short *));
    /* (2) Find all possible combinations of edge pairs, and then populate E with them. */
    i = 0;
    for (j = 0; j < n; j++)
    {
        for (k = 0; k < n; k++)
        {
            if (j < k) /* It's a vertex combination */
            {
                E[i] = calloc(2, sizeof(unsigned short));
                E[i][0] = j;
                E[i][1] = k;
                i++;
            }
        }
    }
    // print_vertex_combinations(E, m_max);

    /* (3) Allocate memory to store vertices container array and initialize it with zeros. */
    G = calloc(n, sizeof(unsigned short *));
    /* (4) Allocate memory to store adjacencies arrays and initialize them with zeros. */
    for (i = 0; i < n; i++)
    {
        G[i] = calloc(n, sizeof(unsigned short));
    }
    for (i = 0; i < m_max; i++)
    {
        if (bernoulli_rv(p_edge))
        { /* If 1 => add edge */
            u = E[i][0];
            v = E[i][1];
            G[u][v] = u;
            G[v][u] = v;
        }
    }
    // print_graph(G, n);
    /* Free edge pairs arrays */
    for (i = 0; i < m_max; i++)
    {
        free(E[i]); /* Warning: E[i] is now a dangling pointer, do not reuse */
    }
    /* Free edge pairs container array */
    free(E); /* Warning: E is now a dangling pointer, do not reuse */
    return G;
}

/**
 * @brief Destroy a random graph -- free memory allocations
 *
 * @param G Pointer to memory allocation with 2d array \f$ (n \times n) \f$ -- graph in the form of adjacencies arrays
 * @param n GP::n
 * 
 * Implementation notes:
 * 1. Time complexity: \f$ \mathcal{O}(n) \f$.
 *
 * Usage notes:
 * 1. `destroy_rand_gnp()` must be called immediately after the created graph is no longer used to free memory.
 */
void destroy_rand_gnp(unsigned short **G, unsigned short n)
{
    unsigned short i;
    /* Free adjacencies arrays */
    for (i = 0; i < n; i++)
    {
        free(G[i]); /* Warning: G[i] is now a dangling pointer, do not reuse */
    }
    /* Free vertices container array */
    free(G); /* Warning: G is now a dangling pointer, do not reuse */
}

/**
 * @brief Visit every vertex once.
 *
 * @param G Pointer to memory allocation with 2d array \f$ (n \times n) \f$ -- graph in the form of adjacencies arrays
 * @param v Current vertex
 * @param visited Array of visited vertices
 * @param n GP::n
 * 
 * Implementation notes:
 * 1. Recursive Depth-first search (DFS) algorithm.
 * 2. Time complexity: \f$ \mathcal{O}(|V| + |E|) \f$.
 */
void dfs(unsigned short **G, unsigned short v, unsigned short *visited, unsigned short n)
{
    unsigned short u, i;
    visited[v] = 1; /* v is now visited*/
    for (i = 0; i < n; i++)
    {
        u = G[i][v];         /* u is a neighbor of v */
        if (visited[u] == 0) /* Visit all neighbors of v */
        {
            dfs(G, u, visited, n);
        }
    }
}

/**
 * @brief Check if a graph is connected by counting the number of connected components in it
 *
 * @param G Pointer to memory allocation with 2d array \f$ (n \times n) \f$ -- graph in the form of adjacencies arrays
 * @param n GP::n
 * @return True (1) if G has exactly one connected component, i.e. it is connected, false (0) otherwise
 * 
 * Implementation notes:
 * 1. The algorithm first counts the number of connected components by employing `dfs()` and then
 * returns true (1), if there is only one connected component in the graph, false (0) otherwise.
 * 2. Time complexity: \f$ \mathcal{O}(|V| + |E|) \f$.
 */
bool is_connected(unsigned short **G, unsigned short n)
{
    /* Allocate memory to store visited array and initialize it with zeros */
    bool is_conn = false;
    unsigned short *visited = calloc(n, sizeof(unsigned short));
    unsigned short cc_count = 0;
    unsigned short v;
    /* Count connected components */
    for (v = 0; v < n; v++) /* Visit all vertices */
    {
        if (visited[v] == 0)
        {
            cc_count++; /* Found a connected component */
            dfs(G, v, visited, n);   /* Visit all neighbors */
        }
    }
    /* Free visited array */
    free(visited); /* Warning: visited is now a dangling pointer, do not reuse */
    if (cc_count == 1)
    {
        is_conn = true;
    }
    return is_conn;
}

/**
 * @brief Find the total number of labeled graphs (connected and disconnected) with \f$ n \f$ nodes -- sequence \f$ A006125 \f$ \cite 3 .
 *
 * @param n Graph order -- GP::n
 * @return Number of labeled graphs
 *
 * Implementation notes:
 * 1. The number of labeled graphs (connected and disconnected) with \f$ n \f$ nodes is GP::n_graphs.
 * 2. Integer overflow unsafe, since the result's size is not checked before return.
 * 3. Time complexity: \f$ \max[\mathcal{O}( \f$ `binom()` \f$ ), \mathcal{O}( \f$ `pow()` \f$ )] \f$.
 *
 * Results for \f$ n \in [0, 11] \f$:
 *  n | A006125(n)
 * ---|-----------------------
 *  0 | 1
 *  1 | 1
 *  2 | 2
 *  3 | 8
 *  4 | 64
 *  5 | 1 024
 *  6 | 32 768
 *  7 | 2 097 152
 *  8 | 268 435 456
 *  9 | 68 719 476 736
 * 10 | 35 184 372 088 832
 * 11 | 36 028 797 018 963 968
 */
unsigned long long A006125_total(unsigned short n)
{
    if (n == 0 | n == 1)
    {
        return 1;
    }
    return pow(2, binom(n, 2));
}

/**
 * @brief Find the number of connected labeled graphs with \f$ n \f$ nodes constructed with the Erdős–Rényi model \f$ \mathcal{G}(n, M) \f$ -- sequence \f$ A001187 \f$ \cite 2 .
 *
 * @param n Graph order -- GP::n
 * @return Number of connected labeled graphs
 *
 * Implementation notes:
 * 1. In this model each graph is chosen randomly with equal probability of \f$ 1 / n_{graphs} \f$, where \f$ n_{graphs} \f$ is GP::n_graphs.
 * 2. The number \f$ C_{n} \f$ of labeled connected graphs of order \f$ n \f$ is given by
 * the recursive formula (1.2.1), p. 7, Harary \cite 5 ;
 * \f$ p \f$ was substituted with \f$ n \f$ to avoid confusion with notation of probability.
 *
 * \f$ \displaystyle C_{n} = 2^{\binom{n}{2}} - \frac{1}{n} \sum_{k=1}^{n-1} k \binom{n}{k} 2^{\binom{n-k}{2}} C_{k} \f$.
 *
 * 3. Integer overflow unsafe, since the result's size is not checked during calculation and before return.
 * 4. Time complexity: \f$ \mathcal{O}(2 \f$ <sup> \f$ (n \max[\mathcal{O}( \f$ `binom()` \f$ ), \mathcal{O}( \f$ `pow()` \f$ )] - 1) \f$ </sup> \f$ ) \f$.
 *
 * Results for \f$ n \in [0, 11] \f$:
 *  n | A001187(n)
 * ---|-----------------------
 *  0 | 1
 *  1 | 1
 *  2 | 1
 *  3 | 4
 *  4 | 38
 *  5 | 728
 *  6 | 26 704
 *  7 | 1 866 256
 *  8 | 251 548 592
 *  9 | 66 296 291 072
 * 10 | 34 496 488 594 816
 * 11 | 35 641 657 548 953 344
 */
unsigned long long A001187_conn(unsigned short n)
{
    unsigned short k;
    unsigned long long disconn_count = 0;

    if (n == 0 | n == 1 | n == 2)
    {
        return 1;
    }
    for (k = 1; k < n; k++)
    {
        disconn_count += k * binom(n, k) * pow(2, binom(n - k, 2)) * A001187_conn(k);
    }
    return pow(2, binom(n, 2)) - disconn_count / n;
}

/**
 * @brief Find the number of disconnected labeled graphs with \f$ n \f$ nodes -- sequence \f$ A054592 \f$ \cite 4 .
 *
 * @param n Graph order -- GP::n
 * @return Number of disconnected labeled graphs
 *
 * Implementation notes:
 * 1. Number of labeled graphs (connected and disconnected) with \f$ n \f$ nodes is sequence \f$ A006125 \f$ \cite 3 .
 * Number of connected labeled graphs with \f$ n \f$ nodes is sequence \f$ A001187 \f$ \cite 2 .
 * Thus, the number of disconnected labeled graphs with \f$ n \f$ nodes is simply
 * \f$ A054592(n) = A006125(n) - A001187(n) \f$.
 * 2. Integer overflow unsafe, since the result's size is not checked during calculation and before return.
 * 3. Time complexity: \f$ \max(\mathcal{O}( \f$ `A006125_total()` \f$), \mathcal{O}( \f$ `A001187_conn()` \f$ )) \f$.
 *
 * Results for \f$ n \in [0, 11] \f$:
 *  n | A054592(n)
 * ---|-----------------------
 *  0 | 0
 *  1 | 0
 *  2 | 1
 *  3 | 4
 *  4 | 26
 *  5 | 296
 *  6 | 6 064
 *  7 | 230 896
 *  8 | 16 886 864
 *  9 | 2 423 185 664
 * 10 | 687 883 494 016
 * 11 | 387 139 470 010 624
 */
unsigned long long A054592_disconn(unsigned short n)
{
    if (n == 0 | n == 1)
    {
        return 0;
    }
    return A006125_total(n) - A001187_conn(n);
}

/**
 * @brief Find the probability of connectedness of a random labeled graph constructed with the Gilbert model \f$ \mathcal{G}(n, p) \f$.
 *
 * @param n Graph order -- GP::n
 * @param p_edge Edge probability GP::p_edge
 * @return \f$ P_{n} = P(\mathcal{G}(n, p) \text{ is connected}) \f$
 *
 * Implementation notes:
 * 1. In this model every possible edge occurs independently with probability \f$ p \f$.
 * The probability of obtaining any one particular random graph with \f$ m \f$ edges is
 * \f$ p^{m}(1-p)^{N-m} \f$, where \f$ N \f$ is GP::m_max.
 * 2. The probability of connectedness of a random labeled graph is given by the recursive formula (3), p. 2, Gilbert \cite 6 ;
 * \f$ q \f$ was substituted by \f$ 1 - p \f$ to avoid introducing unnecessary new variable;
 * \f$ N \f$ was substituted by \f$ n \f$ to avoid confusion with \f$ N \f$ above.
 *
 * \f$ \displaystyle P_{n} = 1 - \sum_{k=1}^{n-1} \binom{n - 1}{k - 1} (1 - p)^{k(n-k)} P_{k} \f$.
 *
 * 3. Integer overflow unsafe, since the result's size is not checked during calculation and before return.
 * 4. Time complexity: \f$ \mathcal{O}(2 \f$ <sup> \f$ (n \max[\mathcal{O}( \f$ `binom()` \f$ ), \mathcal{O}( \f$ `pow()` \f$ )] - 1) \f$ </sup> \f$ ) \f$.
 *
 * Results for \f$ P_{n} \f$ for \f$ n \in [2, 11] \f$ and \f$ p \in [0.1, 0.9] \f$:
 * n / p |   0.1   |   0.2   |   0.3   |   0.4   |   0.5   |   0.6   |   0.7   |   0.8   |   0.9
 * ----: |---------|---------|---------|---------|---------|---------|---------|---------|--------
 *   2   | 0.10000 | 0.20000 | 0.30000 | 0.40000 | 0.50000 | 0.60000 | 0.70000 | 0.80000 | 0.90000
 *   3   | 0.02800 | 0.10400 | 0.21600 | 0.35200 | 0.50000 | 0.64800 | 0.78400 | 0.89600 | 0.97200
 *   4   | 0.01293 | 0.08250 | 0.21865 | 0.40038 | 0.59375 | 0.76550 | 0.89249 | 0.96666 | 0.99581
 *   5   | 0.00810 | 0.08195 | 0.25626 | 0.48965 | 0.71094 | 0.87026 | 0.95751 | 0.99166 | 0.99949
 *   6   | 0.00621 &dagger; | 0.09230 | 0.31690 | 0.59555 | 0.81494 &Dagger; | 0.93652 | 0.98497 | 0.99805 | 0.99994
 *   7   | 0.00551 | 0.11127 | 0.39385 | 0.69878 | 0.88990 | 0.97072 | 0.99484 | 0.99955 | 0.99999
 *   8   | 0.00541 | 0.13851 | 0.47987 | 0.78627 | 0.93709 | 0.98677 | 0.99824 | 0.99990 | 1.00000
 *   9   | 0.00574 | 0.17396 | 0.56714 | 0.85325 | 0.96474 | 0.99408 | 0.99941 | 0.99998 | 1.00000
 *  10   | 0.00644 | 0.21723 | 0.64897 | 0.90128 | 0.98045 | 0.99737 | 0.99980 | 0.99999 | 1.00000
 *  11   | 0.00752 | 0.26729 | 0.72107 | 0.93445 | 0.98925 | 0.99885 | 0.99994 | 1.00000 | 1.00000
 *
 * &dagger; Table 1, p. 2, Gilbert \cite 6 : 0.00624.
 *
 * &Dagger; Table 1, p. 2, Gilbert \cite 6 : 0.81569.
 */
double prob_conn(unsigned short n, double p_edge)
{
    // double q_edge = 1.0 - p_edge;
    double prob_disconn = 0.0;
    unsigned short k;
    for (k = 1; k < n; k++)
    {
        prob_disconn += binom(n - 1, k - 1) * pow(1 - p_edge, k * (n - k)) * prob_conn(k, p_edge);
    }
    return 1.0 - prob_disconn;
}
