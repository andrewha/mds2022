/**
 * @file rand_graph_lib.c
 * @author Andrei Batyrov (arbatyrov@edu.hse.ru)
 * @brief Enumerate and calculate the probability of connectedness of random graphs constructed with the Erdős–Rényi and Gilbert models
 * @version 0.1
 * @date 2023-08-23
 *
 * @copyright Copyright (c) 2023
 */

#include <math.h>

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
 * Maximal possible number of trees with \f$ n \f$ labeled vertices \cite 1 :
 *
 * \f$ n_{trees} = n^{n - 2} \f$.
 */
typedef struct Graph_Params
{
    unsigned int n, m_min;
    long double m_max, m_crit, n_graphs, n_trees;
} GP;

/**
 * @brief Populate and return graph parameters as a structure
 *
 * @param n Number of labeled vertices
 * @return Graph parameters as per structure Graph_Params
 */
GP calc_graph_params(unsigned int n)
{
    GP gp;
    gp.m_max = n * (n - 1) / 2;
    gp.m_min = n - 1;
    gp.m_crit = (n - 1) * (n - 2) / 2;
    gp.n_graphs = pow(2, gp.m_max);
    gp.n_trees = pow(n, (n - 2));
    return gp;
}

/**
 * @brief Find the total number of labeled graphs (connected and disconnected) with \f$ n \f$ nodes -- sequence \f$ A006125 \f$ \cite 3 .
 *
 * @param n Graph order -- number of vertices
 * @return Number of labeled graphs
 *
 * Implementation notes:
 * 1. the number of labeled graphs (connected and disconnected) with \f$ n \f$ nodes is GP::n_graphs.
 * 2. Observe, that \f$ \displaystyle \binom{n}{2} = \frac{n (n - 1)}{2} \f$, thus no factorial is used here.
 * 3. Time complexity: \f$ \displaystyle \mathcal{O}(\text{pow}) \f$.
 */
long double  A006125_total(unsigned int n)
{
    return pow(2, (n * (n - 1) / 2));
}

/**
 * @brief Find the number of connected labeled graphs with \f$ n \f$ nodes constructed with the Erdős–Rényi model \f$ \mathcal{G}(n, M) \f$ -- sequence \f$ A001187 \f$ \cite 2 .
 *
 * @param n Graph order -- number of vertices
 * @return Number of connected labeled graphs
 *
 * Implementation notes:
 * 1. In this model each graph is chosen randomly with equal probability of \f$ 1 / n_{graphs} \f$, where \f$ n_{graphs} \f$ is GP::n_graphs.
 * 2. The number \f$ C_{n} \f$ of labeled connected graphs of order \f$ n \f$ is given by
 * the recursive formula (1.2.1), p. 7 \cite 5 ;
 * \f$ p \f$ was substituted with \f$ n \f$ to avoid confusion with notation of probability.
 * 
 * \f$ \displaystyle C_{n} = 2^{\binom{n}{2}} - \frac{1}{n} \sum_{k=1}^{n-1} k \binom{n}{k} 2^{\binom{n-k}{2}} C_{k} \f$.
 * 
 * 3. Observe, that \f$ n! = \Gamma(n + 1) \f$, thus the standard C \c tgammal() function is used. 
 * 4. Observe, that \f$ \displaystyle \binom{n}{2} = \frac{n (n - 1)}{2} \f$, thus no factorial is used here.
 * 5. Time complexity: \f$ \displaystyle \mathcal{O}(2^{n \mathcal{O}(\text{tgamma + pow}) - 1}) \f$.
 */
long double A001187_conn(unsigned int n)
{
    unsigned int k;
    long double disconn_count = 0.0;

    if (n == 1)
    {
        return 1.0;
    }
    for (k = 1; k < n; k++)
    {
        disconn_count += k * (tgammal(n + 1) / tgammal(k + 1) / tgammal(n - k + 1)) * pow(2, ((n - k) * (n - k - 1) / 2)) * A001187_conn(k);
    }
    return pow(2, (n * (n - 1) / 2)) - disconn_count / n;
}

/**
 * @brief Find the number of disconnected labeled graphs with \f$ n \f$ nodes -- sequence \f$ A054592 \f$ \cite 4 .
 *
 * @param n Graph order -- number of vertices
 * @return Number of disconnected labeled graphs
 *
 * Implementation notes:
 * 1. Number of labeled graphs (connected and disconnected) with \f$ n \f$ nodes is sequence \f$ A006125 \f$ \cite 3 .
 * Number of connected labeled graphs with \f$ n \f$ nodes is sequence \f$ A001187 \f$ \cite 2 .
 * Thus, the number of disconnected labeled graphs with \f$ n \f$ nodes is simply
 * \f$ A054592(n) = A006125(n) - A001187(n) \f$.
 * 2. Time complexity: \f$ \max(\mathcal{O}( \f$ A006125_total \f$), \mathcal{O}( \f$ A001187_conn \f$ )) \f$.
 */
long double A054592_disconn(unsigned int n)
{
    return A006125_total(n) - A001187_conn(n);
}

/**
 * @brief Find the probability of connectedness of a random labeled graph constructed with the Gilbert model \f$ \mathcal{G}(n, p) \f$.
 *
 * @param n Graph order -- number of vertices
 * @param p Edge probability \f$ 0 \leq p \leq 1 \f$
 * @return \f$ P_{N} = P(\mathcal{G}(n, p) \text{ is connected}) \f$
 *
 * Implementation notes:
 * 1. In this model every possible edge occurs independently with probability \f$ p \f$.
 * The probability of obtaining any one particular random graph with \f$ m \f$ edges is 
 * \f$ p^{m}(1-p)^{N-m} \f$, where \f$ N \f$ is GP::m_max.
 * 2. The probability of connectedness of a random labeled graph is given by the recursive formula (3) on p. 2 \cite 6 ;
 * \f$ q \f$ was substituted by \f$ 1 - p \f$ to avoid introducing unnecessary new variable.
 * 
 * \f$ \displaystyle 1 - P_{N} = \sum_{k=1}^{N-1} \binom{N - 1}{k - 1} P_{k} q^{k(N-k)} \f$.
 * 
 * 3. Observe, that \f$ n! = \Gamma(n + 1) \f$, thus the standard C \c tgammal() function is used.
 * 4. Time complexity: \f$ \displaystyle \mathcal{O}(2^{n \mathcal{O}(\text{tgamma + pow}) - 1}) \f$.
 */
float prob_conn(unsigned int n, float p)
{
    // float q = 1.0 - p;
    float prob_disconn = 0.0;
    unsigned int k;
    for (k = 1; k < n; k++)
    {
        prob_disconn += tgammal(n) / tgammal(k) / tgammal(n - k + 1) * pow((1 - p), (k * (n - k))) * prob_conn(k, p);
    }
    return 1.0 - prob_disconn;
}
