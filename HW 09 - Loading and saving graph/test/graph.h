#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <stdlib.h>

typedef struct {
    int start_vertex;
    int end_vertex;
    int cost;
} edge_t;

typedef struct {
    edge_t *edges;
    size_t num_edges;
    size_t capacity;
} graph_t;

graph_t* allocate_graph();
void free_graph(graph_t **graph);

void load_txt(const char *fname, graph_t *graph);
void load_bin(const char *fname, graph_t *graph);

void save_txt(const graph_t * const graph, const char *fname);
void save_bin(const graph_t * const graph, const char *fname);

#endif // __GRAPH_H__
