#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../parser_graph.h"

int main(){
    ParserGraph *graph = createParserGraph();

    tokenizeLine(graph, "scalar sqrt(5)");puts("");
    // parseLine(graph, "z = A*B*y");puts("");
    // parseLine(graph, "B = { 1 0 0 1 }");puts("");
    // parseLine(graph, "for(i in 1:n:1) {");puts("");
    // parseLine(graph, "print(x)");puts("");
    // parseLine(graph, "printsep()");puts("");
    // parseLine(graph, "sca = scalar123[1:2*31.5]");puts("");
    // parseLine(graph, "sca = scalar[1:2*31.5]");puts("");
    // parseLine(graph, "0sca = sca [1:2*31.5]");puts("");
    return 0;
}