/*
delatvan@gmail.com
*/

#include "edge.hpp"
#include "vertex.hpp"

using namespace std;

edge::edge(){
	/*
	edge is a head for the accesible_edges vector
	*/
	index=-1;
	x=NULL;
	y=NULL;
	next = NULL;
	flow = 0;
	active_res = 0;
	active_res_opp = 0;
	this->update_flow(0);
}

edge::edge(int index_, vertex * x_, vertex* y_, int capacity_){
	index = index_;
	x = x_;
	y = y_;
	capacity = capacity_;
	flow = 0;
	active_res = 0;
	active_res_opp = 0;
	this->update_flow(0);
}

edge::edge(int index_, vertex * x_, vertex* y_, int capacity_, int cost_){
	index = index_;
	x = x_;
	y = y_;
	capacity = capacity_;
	cost = cost_;
	flow = 0;
	active_res = 0;
	active_res_opp = 0;
	this->update_flow(0);
}

edge::edge(int index_, int cost_, vertex * x_, vertex* y_){
	index = index_;
	cost = cost_;
	x = x_;
	y = y_;
	flow = 0;
	active_res = 0;
	active_res_opp = 0;
	this->update_flow(0);
}

edge::~edge()
{}

void edge::print(){
	cout<<"Index: "<<index<<endl;
	cout<<"x iD = "<<x->iD<<" "<<"y iD = "<<y->iD<<" "<<"capacity = "<<capacity<<endl;
	cout<<"f(e) = "<<flow<<" uf(e) = "<<res_capacity<<" uf_opp(e) = "<<res_capacity_opp<<endl;
}

void edge::update_flow(int new_flow){
	flow = flow + new_flow;
	res_capacity = capacity - flow;
	res_capacity_opp = flow;
}


//sort vector of edges by capacity
void edge::sort_by_capacity(vector<edge>&  edges)
{
	cmp_capacity comparison;
	sort(edges.begin(),edges.end(),comparison);
}