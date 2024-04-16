#include "graph.h"
#include <stdio.h>
#include <stdlib.h>

static int resize_edges_if_needed(graph_t *graph) {
    if (graph->num_edges >= graph->capacity) {
        size_t new_capacity = (graph->capacity == 0) ? 10 : graph->capacity * 2;
        edge_t *new_edges = (edge_t *)realloc(graph->edges, new_capacity * sizeof(edge_t));
        if (!new_edges) {
            perror("Failed to resize edges");
            return -1;
        }

        graph->edges = new_edges;
        graph->capacity = new_capacity;
    }
    return 0;
}

void load_bin(const char *fname, graph_t *graph) {
    FILE *file = fopen(fname, "rb");
    if (!file) {
        perror("Failed to open file for reading");
        return;
    }

    edge_t temp_edge;
    size_t read_size = 0;

    while (fread(&temp_edge, sizeof(edge_t), 1, file) == 1) {
        if (resize_edges_if_needed(graph) == -1) {
            break;
        }

        graph->edges[graph->num_edges++] = temp_edge;
        read_size += sizeof(edge_t);
    }

    if (ferror(file)) {
        perror("Error while reading file");
    }

    fclose(file);

    printf("Read %zu bytes from %s\n", read_size, fname);
}

void save_bin(const graph_t *const graph, const char *fname) {
    FILE *file = fopen(fname, "wb");
    if (!file) {
        perror("Failed to open file for writing");
        return;
    }

    size_t write_size = 0;

    fwrite(graph->edges, sizeof(edge_t), graph->num_edges, file);
    write_size = sizeof(edge_t) * graph->num_edges;

    if (ferror(file)) {
        perror("Error while writing file");
    }

    fclose(file);

    printf("Wrote %zu bytes to %s\n", write_size, fname);
}
