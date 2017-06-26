/*
delatvan@gmail.com
*/

// graph.hpp (Declaration of Class Graph)
#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <vector>
#include "vertex.hpp"
#include "edge.hpp"

using namespace std;


class graph{

public:

graph(char const* filename);
~graph();


// vertices in Graph G
vector<vertex> vertices; 
//edges in Graph G
vector<edge> edges;
//vector of active vertices and corresponding heights
vector<vertex> active_vertices;
//vector of admissible edges for each vertex
vector<edge> admissible_edges;

int number_vertices;
int number_edges;
vertex * s;
vertex * t;

void print();

//update excess for a given vertex
void update_excess(vertex * v);

//insert vertex into list of active vertices
void active_insert(vertex & head, vertex * x);

//delete vertex from list of active vertices
void active_delete(vertex & head, vertex * x);

//insert edge into admissible edges from Gf
void admissible_insert(edge & head, edge * e, int v );

//delete edge from admissible edges from Gf
void admissible_delete(edge & head, edge * e, int v);

//insert edge into admissible edges from Gf_opp
void admissible_opp_insert(edge & head, edge * e, int v);

//delete edge from admissible edges from Gf_opp
void admissible_opp_delete(edge & head, edge * e, int v);

//checks if e is admissible for v
int edge_admissible(edge * e, int v);

//updates admissible edges for vertex v
void admissible_update(int v_iD);

//initialize vector of admissible_edge
void admissible_init();


private:

};

#endif