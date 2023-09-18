/**
 * @file rand_graph_lib.c
 * @author Andrei Batyrov (arbatyrov@edu.hse.ru)
 * @brief Library for enumerating and calculating the probability of connectedness of random undirected graphs constructed with the Erdős-Rényi and Gilbert models
 * @version 0.1
 * @date 2023-08-28
 *
 * @copyright Copyright (c) 2023
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "stats_lib.h"

/**
 * @struct Graph_Params
 * @brief Graph parameters structure, declared as a new type.
 *
 * @var Graph_Params::n
 * Number of labeled vertices.
 *
 * @var Graph_Params::m_max
 * Maximal possible number of edges in the complete graph with \f$ n \f$ labeled vertices:
 *
 * \f$ \displaystyle m_{max} = \binom{n}{2} = \frac{n  (n - 1)}{2} \f$ (complete graph).

 * @var Graph_Params::m_min
 * Minimal number of edges in a connected graph (tree):

 * \f$ m_{min} = n - 1 \f$ (tree).
 *
 * @var Graph_Params::m_crit
 * Number of edges as connectedness threshold:
 *
 * \f$ \displaystyle m_{crit} = \binom{n - 1}{2} = \frac{(n - 1)(n - 2)}{2} \f$.
 *
 * @var Graph_Params::n_graphs
 * Maximal possible number of graphs with \f$ n \f$ labeled vertices:
 *
 * \f$ n_{graphs} = 2^{m_{max}} \f$.
 *
 * @var Graph_Params::n_trees
 * Maximal possible number of trees with \f$ n \f$ labeled vertices, p.292, Erdős \cite 1 :
 *
 * \f$ n_{trees} = n^{n - 2} \f$.
 *
 * @var Graph_Params::p_edge
 * Edge probability \f$ 0 \leq p \leq 1 \f$
 *
 * @var Graph_Params::E
 * Pointer to memory allocation with an array of \f$ m_{max} \f$ vertex pair combinations, stored as \f$ 256 u + v \f$:
 *
 * Example for a graph on 3 vertices \f$ (m_{max} = 3) \f$: `[256 * 0 + 1 = 1, 256 * 0 + 2 = 2, 256 * 1 + 2 = 258]`.
 *
 * @var Graph_Params::G
 * Pointer to memory allocation with 2d array \f$ (n \times n) \f$ -- graph in the form of adjacencies arrays.

 */
typedef struct
{
    unsigned short n, m_min, m_max, m_crit;
    unsigned long long n_graphs, n_trees;
    double p_edge;
    unsigned short *E, **G;
} Graph_Params;

/**
 * @brief Print out all possible vertex pairs.
 *
 * @param gp Graph_Params
 *
 * Implementation notes:
 * 1. Vertex pair \f$ E_i \f$ is stored in one unsigned short allocation,
 * thus extracting high part for \f$ u \f$ and low part for \f$ v \f$.
 * 2. Time complexity: \f$ \mathcal{O}(m_{max}) \f$.
 */
void show_vertex_pairs(Graph_Params gp)
{
    unsigned short i, u, v;
    printf("Vertex pairs (%hu):\n", gp.m_max);
    for (i = 0; i < gp.m_max; i++)
    {
        // printf("(%hu, %hu)\n", gp.E[i][0], gp.E[i][1]);
        u = (gp.E[i] >> 8) & 0xff;
        v = gp.E[i] & 0xff;
        printf("(%hu, %hu)\n", u, v);
    }
}

/**
 * @brief Populate and return graph parameters as a structure.
 *
 * @param n Graph_Params::n
 * @param p_edge Graph_Params::p_edge
 * @return Graph parameters as per the structure Graph_Params
 *
 * Implementation notes:
 * 1. Integer overflow unsafe, since the rhs's size is not checked before assignment.
 * 2. Null pointer unsafe, since `calloc()` might fail, but this is not checked to reduce running time.
 * 3. Time complexity: \f$ \max[\mathcal{O}( \f$ `binom()` \f$ ), \mathcal{O}( \f$ `pow()` \f$ )] \f$.
 */
Graph_Params set_rand_gnp_params(unsigned short n, double p_edge)
{
    unsigned short i, j, k;
    Graph_Params gp;
    gp.n = n;
    gp.m_max = binom(n, 2);
    gp.m_min = n - 1;
    gp.m_crit = binom(n - 1, 2);
    gp.n_graphs = pow(2, gp.m_max);
    gp.n_trees = pow(n, n - 2);
    gp.p_edge = p_edge;

    /* Allocate memory to store vertex pairs container array and initialize it with zeros. */
    gp.E = calloc(gp.m_max, sizeof(unsigned short));
    /* Find all possible combinations of vertex pairs, and then populate E with them. */
    i = 0;
    for (j = 0; j < n; j++)
    {
        for (k = 0; k < n; k++)
        {
            if (j < k) /* It's a vertex pair (combination) */
            {
                // gp.E[i] = calloc(2, sizeof(unsigned short));
                // gp.E[i][0] = j;
                // gp.E[i][1] = k;
                gp.E[i++] = 256 * j + k; /* Store vertex pair in one unsigned short allocation */
                // i++;
            }
        }
    }
    // show_vertex_pairs(gp);

    /* Allocate memory to store vertices container array and initialize it with zeros. */
    gp.G = calloc(gp.n, sizeof(unsigned short *));
    /* Allocate memory to store adjacencies arrays and initialize them with zeros. */
    for (i = 0; i < gp.n; i++)
    {
        gp.G[i] = calloc(gp.n, sizeof(unsigned short));
    }
    return gp;
}

/**
 * @brief Print out graph adjacencies for each vertex (row by row).
 *
 * @param gp Graph_Params
 *
 * Implementation notes:
 * 1. Time complexity: \f$ \mathcal{O}(n^2) \f$.
 */
void show_rand_gnp(Graph_Params gp)
{
    unsigned short i, j;
    printf("Graph adjacencies for each vertex:\n");
    for (i = 0; i < gp.n; i++)
    {
        printf("%u: [", i);
        for (j = 0; j < gp.n; j++)
        {
            printf(" %u ", gp.G[j][i]);
        }
        printf("]\n");
    }
}

/**
 * @brief Construct a random \f$ \mathcal{G}(n, p) \f$ graph with \f$ n \f$ vertices and \f$ p m_{max} \f$ edges (on average).
 *
 * @param gp Graph_Params
 *
 * Implementation notes:
 * 1. Vertex pair \f$ E_i \f$ is stored in one unsigned short allocation,
 * thus extracting high part for \f$ u \f$ and low part for \f$ v \f$.
 * 2. Time complexity: \f$ \mathcal{O}(m_{max} \f$ `bernoulli_rv()` \f$ ) \f$.
 *
 * Usage notes:
 * 1. `destroy_rand_gnp()` must be called immediately after the created graph is no longer used to clean graphs's adjacencies arrays.
 */
void construct_rand_gnp(Graph_Params gp)
{
    unsigned short i, u, v;

    for (i = 0; i < gp.m_max; i++)
    {
        if (bernoulli_rv(gp.p_edge))
        { /* If true (1) => add edge */
            /* Vertex pair E[i] is stored in one unsigned short allocation, thus extracting high part for `u` and low part for `v` */
            u = (gp.E[i] >> 8) & 0xff;
            v = gp.E[i] & 0xff;
            gp.G[u][v] = u;
            gp.G[v][u] = v;
        }
    }
    // show_rand_gnp(gp);
}

/**
 * @brief Destroy a random graph -- clean graph's adjacencies arrays.
 *
 * @param gp Graph_Params
 *
 * Implementation notes:
 * 1. Time complexity: \f$ \mathcal{O}(n \f$ `memset()` \f$ ) \f$.
 *
 * Usage notes:
 * 1. `destroy_rand_gnp()` must be called immediately after the created graph is no longer used to clean graph's adjacencies arrays.
 */
void destroy_rand_gnp(Graph_Params gp)
{
    unsigned short i;
    /* Clean adjacencies arrays */
    for (i = 0; i < gp.n; i++)
    {
        memset(gp.G[i], 0, gp.n * sizeof(unsigned short));
    }
}

/**
 * @brief Visit every vertex once.
 *
 * @param gp Graph_Params
 * @param v Current vertex
 * @param visited Array of visited vertices
 *
 * Implementation notes:
 * 1. Recursive Depth-first search (DFS) algorithm.
 * 2. Time complexity: \f$ \mathcal{O}(|V| + |E|) \f$.
 */
void dfs(Graph_Params gp, unsigned short v, bool *visited)
{
    unsigned short u, i;
    visited[v] = true; /* `v` is now visited*/
    for (i = 0; i < gp.n; i++)
    {
        u = gp.G[i][v];  /* `u` is a neighbor of `v` */
        if (!visited[u]) /* Visit all neighbors of `v` */
        {
            dfs(gp, u, visited);
        }
    }
}

/**
 * @brief Count the number of connected components in a graph.
 *
 * @param gp Graph_Params
 * @return Number of connected components
 *
 * Implementation notes:
 * 1. The algorithm counts the number of connected components by employing `dfs()`.
 * 2. Time complexity: \f$ \mathcal{O}(|V| + |E|) \f$.
 */
unsigned short count_connected_components(Graph_Params gp)
{
    bool visited[gp.n];
    memset(visited, false, sizeof(visited)); /* Init `visited` array with false(s) */
    unsigned short cc_count = 0;
    unsigned short v;
    /* Count connected components */
    for (v = 0; v < gp.n; v++) /* Visit all vertices */
    {
        if (!visited[v])
        {
            cc_count++;          /* Found a connected component */
            dfs(gp, v, visited); /* Visit all neighbors */
        }
    }
    return cc_count;
}

/**
 * @brief Check if a graph is connected by counting the number of connected components in it.
 *
 * @param gp Graph_Params
 * @return `True (1)` if G has exactly one connected component, i.e. it is connected, `false (0)` otherwise
 *
 * Implementation notes:
 * 1. The algorithm first counts the number of connected components by employing `count_connected_components()` and then
 * returns `true (1)`, if there is only one connected component in the graph, `false (0)` otherwise.
 * 2. Time complexity: \f$ \mathcal{O}(|V| + |E|) \f$.
 */
bool is_connected(Graph_Params gp)
{
    bool is_conn = false;

    if (count_connected_components(gp) == 1)
    {
        is_conn = true;
    }
    return is_conn;
}

/**
 * @brief Find the total number of labeled graphs (connected and disconnected) with \f$ n \f$ nodes -- sequence \f$ A006125 \f$ \cite 3 .
 *
 * @param n Graph order -- Graph_Params::n
 * @return Number of labeled graphs
 *
 * Implementation notes:
 * 1. The number of labeled graphs (connected and disconnected) with \f$ n \f$ nodes is Graph_Params::n_graphs.
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
    if ((n == 0) | (n == 1))
    {
        return 1;
    }
    return pow(2, binom(n, 2));
}

/**
 * @brief Find the number of connected labeled graphs with \f$ n \f$ nodes constructed with the Erdős–Rényi model \f$ \mathcal{G}(n, M) \f$ -- sequence \f$ A001187 \f$ \cite 2 .
 *
 * @param n Graph order -- Graph_Params::n
 * @return Number of connected labeled graphs
 *
 * Implementation notes:
 * 1. In this model each graph is chosen randomly with equal probability of \f$ 1 / n_{graphs} \f$, where \f$ n_{graphs} \f$ is Graph_Params::n_graphs.
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

    if ((n == 0) | (n == 1) | (n == 2))
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
 * @param n Graph order -- Graph_Params::n
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
    if ((n == 0) | (n == 1))
    {
        return 0;
    }
    return A006125_total(n) - A001187_conn(n);
}

/**
 * @brief Find the probability of connectedness of a random labeled graph constructed with the Gilbert model \f$ \mathcal{G}(n, p) \f$.
 *
 * @param n Graph order -- Graph_Params::n
 * @param p_edge Edge probability Graph_Params::p_edge
 * @return \f$ P_{n} = P(\mathcal{G}(n, p) \text{ is connected}) \f$
 *
 * Implementation notes:
 * 1. In this model every possible edge occurs independently with probability \f$ p \f$.
 * The probability of obtaining any one particular random graph with \f$ m \f$ edges is
 * \f$ p^{m}(1-p)^{N-m} \f$, where \f$ N \f$ is Graph_Params::m_max.
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
