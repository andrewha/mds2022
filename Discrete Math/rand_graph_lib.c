/**
 * @file rand_graph_lib.c
 * @author Andrei Batyrov (arbatyrov@edu.hse.ru)
 * @brief Enumerate and calculate the probability of connectedness of random graphs constructed with the Erdős-Rényi and Gilbert models
 * @version 0.1
 * @date 2023-08-25
 *
 * @copyright Copyright (c) 2023
 */
#include <math.h>

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
 */
typedef struct Graph_Params
{
    unsigned short n, m_min, m_max, m_crit;
    unsigned long long n_graphs, n_trees;
} GP;

/**
 * @brief Populate and return graph parameters as a structure
 *
 * @param n Number of labeled vertices
 * @return Graph parameters as per structure Graph_Params
 *
 * Implementation notes:
 * 1. Integer overflow unsafe, since the rhs's size is not checked before assignment.
 */
GP calc_graph_params(unsigned short n)
{
    GP gp;
    gp.m_max = binom(n, 2);
    gp.m_min = n - 1;
    gp.m_crit = binom(n - 1, 2);
    gp.n_graphs = pow(2, gp.m_max);
    gp.n_trees = pow(n, n - 2);
    return gp;
}

/**
 * @brief Find the total number of labeled graphs (connected and disconnected) with \f$ n \f$ nodes -- sequence \f$ A006125 \f$ \cite 3 .
 *
 * @param n Graph order -- number of vertices
 * @return Number of labeled graphs
 *
 * Implementation notes:
 * 1. The number of labeled graphs (connected and disconnected) with \f$ n \f$ nodes is GP::n_graphs.
 * 2. Integer overflow unsafe, since the result's size is not checked before return.
 * 3. Time complexity: \f$ \displaystyle \mathcal{O}(\text{pow}) \f$.
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
 * @param n Graph order -- number of vertices
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
 * 4. Time complexity: \f$ \displaystyle \mathcal{O}(2^{n \max(\mathcal{O}(\text{binom}), \mathcal{O}(\text{pow})) - 1}) \f$.
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
 * @param n Graph order -- number of vertices
 * @return Number of disconnected labeled graphs
 *
 * Implementation notes:
 * 1. Number of labeled graphs (connected and disconnected) with \f$ n \f$ nodes is sequence \f$ A006125 \f$ \cite 3 .
 * Number of connected labeled graphs with \f$ n \f$ nodes is sequence \f$ A001187 \f$ \cite 2 .
 * Thus, the number of disconnected labeled graphs with \f$ n \f$ nodes is simply
 * \f$ A054592(n) = A006125(n) - A001187(n) \f$.
 * 2. Integer overflow unsafe, since the result's size is not checked during calculation and before return.
 * 3. Time complexity: \f$ \max(\mathcal{O}( \f$ A006125_total \f$), \mathcal{O}( \f$ A001187_conn \f$ )) \f$.
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
 * @param n Graph order -- number of vertices
 * @param p Edge probability \f$ 0 \leq p \leq 1 \f$
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
 * 4. Time complexity: \f$ \displaystyle \mathcal{O}(2^{n \max(\mathcal{O}(\text{binom}), \mathcal{O}(\text{pow})) - 1}) \f$.
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
double prob_conn(unsigned short n, double p)
{
    // double q = 1.0 - p;
    double prob_disconn = 0.0;
    unsigned short k;
    for (k = 1; k < n; k++)
    {
        prob_disconn += binom(n - 1, k - 1) * pow(1 - p, k * (n - k)) * prob_conn(k, p);
    }
    return 1.0 - prob_disconn;
}
