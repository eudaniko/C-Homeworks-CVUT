#include "graph.h"
#include <stdio.h>

int main() {
    const char *input_txt = "graph_input.txt";   // Replace with your input text file name
    const char *binary_file = "graph_binary.bin";
    const char *output_txt = "graph_output.txt";

    // Allocate a new graph
    graph_t *graph = allocate_graph();
    if (!graph) {
        fprintf(stderr, "Failed to allocate graph.\n");
        return 1;
    }

    // Load graph from text file
    load_txt(input_txt, graph);
    printf("Graph loaded from text file.\n");

    // Save graph to binary file
    save_bin(graph, binary_file);
    printf("Graph saved to binary file.\n");

    // Free and reallocate the graph
    free_graph(&graph);
    graph = allocate_graph();

    // Load graph from binary file
    load_bin(binary_file, graph);
    printf("Graph loaded from binary file.\n");

    // Save graph to another text file
    save_txt(graph, output_txt);
    printf("Graph saved to text file.\n");

    // Free the graph
    free_graph(&graph);

    return 0;
}
