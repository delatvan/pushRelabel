/*
delatvan@gmail.com
*/

#include <iostream>
#include "vertex.hpp"
#include "edge.hpp"
#include "graph.hpp"
#include <vector>
#include <algorithm>
#include <limits>
using namespace std;


void push(edge * e, vertex * v, graph &g){
	//e = (v,w)
	if (e->x->iD==v->iD)
	{
		//check res_capacity
		int gamma = min(e->res_capacity,v->excess);
		e->update_flow(gamma);
		g.update_excess(v);
		g.update_excess(e->y);
		if (v->excess>0)
		{
			g.active_insert(g.active_vertices[v->height],v);
		}
		if(e->y->iD!=0 && e->y->iD!=1 && e->y->excess>0 && e->y->active==0){
			g.active_insert(g.active_vertices[e->y->height],e->y);
		}
		if(g.edge_admissible(e,v->iD)){
			g.admissible_insert(g.admissible_edges[v->iD],e,v->iD);
		}
		g.admissible_update(e->y->iD);

	}
	//e=(w,v), contemplating as e_opp
	else{
		//check res_capacity_opp
		int gamma = min(e->res_capacity_opp,v->excess);
		e->update_flow(-gamma);
		g.update_excess(v);
		g.update_excess(e->x);
		if (v->excess>0)
		{
			g.active_insert(g.active_vertices[v->height],v);
		}
		if(e->x->iD!=0 && e->x->iD!=1 && e->x->excess>0 && e->x->active==0){
			g.active_insert(g.active_vertices[e->x->height],e->x);
		}
		if(g.edge_admissible(e,v->iD)){
			g.admissible_opp_insert(g.admissible_edges[v->iD],e,v->iD);
		}
		g.admissible_update(e->x->iD);


	}
}

int relabel(vertex * v, graph &g){
	int initial = v->height;
	int min = 2*g.number_vertices-1;
	for (int i = 0; i < v->delta_plus.size(); ++i)
	{
		if (g.edges[v->delta_plus[i]].res_capacity>0)
		{
			if (min > g.edges[v->delta_plus[i]].y->height+1 )
			{
				min = g.edges[v->delta_plus[i]].y->height+1;
			}
		}
	}
	for (int i = 0; i < v->delta_min.size(); ++i)
	{
		if (g.edges[v->delta_min[i]].res_capacity_opp>0)
		{
			if (min > g.edges[v->delta_min[i]].x->height+1 )
			{
				min = g.edges[v->delta_min[i]].x->height+1;
			}
		}
	}
	v->height = min;
	g.active_insert(g.active_vertices[v->height],v);
	g.admissible_update(v->iD);
	return v->height -initial;
}


int main(int argc, char* argv[]){
	
	graph g(argv[1]);
	for (int i = 0; i < g.s->delta_plus.size(); ++i)
	{	
		/*
		Each time there's an update flow there has to be an
		update excess for v & w
		*/
		g.edges[g.s->delta_plus[i]].update_flow(g.edges[g.s->delta_plus[i]].capacity);
		g.update_excess(g.s);
		g.update_excess(g.edges[g.s->delta_plus[i]].y);
		if(g.edges[g.s->delta_plus[i]].y->iD!=1){
			g.active_insert(g.active_vertices[0],g.edges[g.s->delta_plus[i]].y);
		}
	}
	g.s->height = g.number_vertices;
	g.admissible_init();
	int height_iterator = 0;
	vertex *v;
	edge *e;
	//TILL HERE OK

	//while there exists an active vertex
	while(height_iterator>-1){
		if (g.active_vertices[height_iterator].next!=NULL)
		{
			v = g.active_vertices[height_iterator].next;
			//delete v from active list
			g.active_delete(g.active_vertices[height_iterator],v);
			if(g.admissible_edges[v->iD].next!=NULL){
				e = g.admissible_edges[v->iD].next;
				//this means e=(v,w) -> e=(v,w) in Gf
				if (e->x->iD==v->iD)
				{
					g.admissible_delete(g.admissible_edges[v->iD],e,v->iD);
				}
				//this means e=(w,v) -> e_opp=(v,w) in Gf
				else{
					g.admissible_opp_delete(g.admissible_edges[v->iD],e,v->iD);
				}
				push(e, v, g);
			}
			//There are no admissible edges for v
			else{
				height_iterator+=relabel(v, g);
			}
		}
		else{
			height_iterator--;
		}
	}
	
	cout<<-g.s->excess<<endl;
	
	for (int i = 0; i < g.edges.size(); ++i)
	{	
		if(g.edges[i].flow != 0){
			cout<<g.edges[i].index<<" "<<g.edges[i].flow<<endl;
		}
	}

	return 0;

}