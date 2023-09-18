/**
 * @file rand_graph_lib.h
 * @author Andrei Batyrov (arbatyrov@edu.hse.ru)
 * @brief Random graph library declarations
 * @version 0.1
 * @date 2023-08-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <stdbool.h>

typedef struct
{
    unsigned short n, m_min, m_max, m_crit;
    unsigned long long n_graphs, n_trees;
    double p_edge;
    unsigned short *E, **G;
} Graph_Params;

Graph_Params set_rand_gnp_params(unsigned short n, double p_edge);

void show_vertex_pairs(Graph_Params gp);

void show_rand_gnp(Graph_Params gp);

void construct_rand_gnp(Graph_Params gp);

void destroy_rand_gnp(Graph_Params gp);

void dfs(Graph_Params gp, unsigned short v, bool *visited);

unsigned short count_connected_components(Graph_Params gp);

bool is_connected(Graph_Params gp);

unsigned long long A006125_total(unsigned short n);

unsigned long long A001187_conn(unsigned short n);

unsigned long long A054592_disconn(unsigned short n);

double prob_conn(unsigned short n, double p_edge);
