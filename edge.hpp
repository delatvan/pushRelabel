/*
delatvan@gmail.com
*/

// edge.hpp (Declaration of Class Edge)
#ifndef EDGE_HPP
#define EDGE_HPP

#include <iostream>
#include <vector>

using namespace std;

class vertex;

class edge{

public:

	//default constructor
	edge();

	//constructor for edges just with capacity
	edge(int index_, vertex * x_, vertex* y_, int capacity_);

	//constructor for edges with capacity and cost
	edge(int index_, vertex * x_, vertex* y_, int capacity_, int cost_);

	//constructor for edges just with cost
	edge(int index_, int cost_, vertex* x_, vertex* y_);

	~edge();

	//index of the edge
	int index;
	//cost of the edge
	int cost;
	//capacity of the edge u(e)
	int capacity;
	//capacity of edge in Gf (same direction) uf(e)
	int res_capacity;
	//capacity of edge in Gf (opposite direction) uf(e'), e'=(y,x)
	int res_capacity_opp;
	//flow through the edge
	int flow;
	//edge active as res_capacity for vertex x
	int active_res;
	//edge active as res_capacity_opp for vertex y
	int active_res_opp;

	vertex * x;
	vertex * y;
	/*
	if edge directed e=(x,y)
	x->y
	*/
	edge * next;
	edge * next_Gf;
	edge * next_Gf_opp;
	edge * prev_Gf;
	edge * prev_Gf_opp;
	
	//comparator
	struct cmp_capacity{
	bool operator()(const edge& e1, const edge& e2) const{
		return e1.capacity<e2.capacity;
	}
	};
	
	void print();

	void update_flow(int new_flow);

	static void sort_by_capacity(vector<edge> & edges);

private:


};

#endif