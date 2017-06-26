/*
delatvan@gmail.com
*/

#include "graph.hpp"
#include <fstream> 
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "vertex.hpp"
#include "edge.hpp"

using namespace std;

//constructor (G,u,s,t)
// there are at least 2 vertex, s!=t
graph::graph(char const * filename)
{
	string buf;
	ifstream file(filename);

	if (file.is_open())
	{	
		
		getline(file,buf);
		stringstream s1(buf);
		s1>>number_vertices; //number_vertices>=2 per def
		vertices.resize(number_vertices);
		active_vertices.resize(2*number_vertices-1);
		admissible_edges.resize(number_vertices);
		s = &(vertices[0]);
		t = &(vertices[1]);
		number_edges = 0;
		while(getline(file,buf)){
			stringstream ss(buf);
			int v_iD, w_iD, capacity;
			ss>>v_iD >> w_iD >>capacity;
			if (vertices[v_iD].iD==-1)
			{
				vertex v(v_iD,0);
				vertices[v_iD]= v;
			}
			if (vertices[w_iD].iD==-1)
			{
				vertex w(w_iD,0);
				vertices[w_iD]= w;
			}
			edge e(number_edges,&(vertices[v_iD]), &(vertices[w_iD]), capacity);
			edges.push_back(e);
			vertices[v_iD].delta_plus.push_back(e.index);
			vertices[w_iD].delta_min.push_back(e.index);
			++number_edges;
		}
	}
	file.close();
}


graph::~graph()
{}


//it could be maybe improved
void graph::print()
{		cout<<"Edges"<<endl;
	for(int i=0; i<edges.size(); i++){
		edges[i].print();
	}
	
	cout<<"Vertices: "<<endl;
	for(int i=0; i<vertices.size(); i++){
		vertices[i].print();
	}

}

void graph::update_excess(vertex * v){
	int incoming = 0;
	int outgoing = 0;
	for (int i = 0; i < v->delta_min.size(); ++i)
	{
		incoming += edges[v->delta_min[i]].flow;
	}
	for (int i = 0; i < v->delta_plus.size(); ++i)
	{
		outgoing += edges[v->delta_plus[i]].flow;
	}
	v->excess = incoming - outgoing;
}

void graph::active_insert(vertex & head, vertex * x){
	x->next = head.next;
	if (head.next!=NULL)
	{
		head.next->prev = x;
	}
	head.next = x;
	x->prev = NULL;
	head.active = 1;
	x->active=1;
}

void graph::active_delete(vertex & head, vertex * x){
	if (x->prev!=NULL)
	{
		x->prev->next = x->next;
	}
	else{
		head.next = x->next;
	}
	if (x->next!=NULL)
	{
		x->next->prev = x->prev;
	}
	if (head.next==NULL)
	{
		head.active = 0;
	}
	x->active = 0;
}

//insert edge into admissible edges from Gf
void graph::admissible_insert(edge & head, edge * e, int v){
	e->next_Gf = head.next;
	if (head.next!=NULL)
	{	
		//if head.next edge is in Gf, head.next=(v,w)
		if ( head.next->x->iD==v)
		{
			head.next->prev_Gf = e;
		}
		//if it corresponds to delta_minus, thus in Gf_opp
		else{
			head.next->prev_Gf_opp = e;
		}
	}
	head.next = e;
	e->prev_Gf = NULL;
	e->active_res = 1;
}

//delete edge from admissible edges from Gf
void graph::admissible_delete(edge & head, edge * e, int v){
	if (e->prev_Gf!=NULL)
	{	
		//if e.prev_Gf is in Gf
		if (e->prev_Gf->x->iD==v)
		{
			e->prev_Gf->next_Gf = e->next_Gf;
		}
		else{
			e->prev_Gf->next_Gf_opp = e->next_Gf;
		}
	}
	else{
		head.next = e->next_Gf;
	}
	if (e->next_Gf!=NULL)
	{	
		//if e.next_Gf is in Gf
		if (e->next_Gf->x->iD==v)
		{
			e->next_Gf->prev_Gf = e->prev_Gf;
		}
		else{
			e->next_Gf->prev_Gf_opp = e->prev_Gf;
		}
	}
	e->active_res = 0;

}

//insert edge into admissible edges from Gf_opp
void graph::admissible_opp_insert(edge & head, edge * e, int v){
	e->next_Gf_opp = head.next;
	if (head.next!=NULL)
	{	
		//if head.next edge is in Gf, head.next=(v,w)
		if ( head.next->x->iD==v)
		{
			head.next->prev_Gf = e;
		}
		//if it corresponds to delta_minus, thus in Gf_opp
		else{
			head.next->prev_Gf_opp = e;
		}
	}
	head.next = e;
	e->prev_Gf_opp = NULL;
	e->active_res_opp = 1;

}


//delete edge from admissible edges from Gf_opp
void graph::admissible_opp_delete(edge & head, edge * e, int v){
	/*
	We're treating e as if it belongs to delta_minus of v.
	So e is a reverse edge for Gf.So e=(w,v). Thus only 
	appearing as an admissible edge for v in the case 
	were uf(e)_opp is greater than 0. So only if
	the fluss in greater than 0.
	*/
	if (e->prev_Gf_opp!=NULL)
	{	
		//if e.prev_Gf_opp is in Gf
		if (e->prev_Gf_opp->x->iD==v)
		{
			e->prev_Gf_opp->next_Gf = e->next_Gf_opp;
		}
		else{
			e->prev_Gf_opp->next_Gf_opp = e->next_Gf_opp;
		}
	}
	else{
		head.next = e->next_Gf_opp;
	}
	if (e->next_Gf_opp!=NULL)
	{	
		//if e.next_Gf_opp is in Gf
		if (e->next_Gf_opp->x->iD==v)
		{
			e->next_Gf_opp->prev_Gf = e->prev_Gf_opp;
		}
		else{
			e->next_Gf_opp->prev_Gf_opp = e->prev_Gf_opp;
		}
	}
	e->active_res_opp = 0;

}

int graph::edge_admissible(edge * e, int v){
	//e in delta+ of v
	if (e->x->iD==v)
	{
		if (e->res_capacity>0 && e->x->height == e->y->height+1)
		{
			return 1;
		}
	}
	//e in delta- of v, e=(w,v) thus e_opp=(v,w)
	else{
		if (e->res_capacity_opp>0 && e->y->height == e->x->height+1)
		{
			return 1;
		}
	}
	return 0;
}

void graph::admissible_update(int v_iD){
	for(int j = 0; j < vertices[v_iD].delta_plus.size(); ++j)
		{
			if (edge_admissible(&edges[vertices[v_iD].delta_plus[j]],v_iD) && edges[vertices[v_iD].delta_plus[j]].active_res==0)
			{
				admissible_insert(admissible_edges[v_iD],&edges[vertices[v_iD].delta_plus[j]],v_iD);
			}
		}
		for(int j = 0; j < vertices[v_iD].delta_min.size(); ++j)
		{
			if (edge_admissible(&edges[vertices[v_iD].delta_min[j]],v_iD) && edges[vertices[v_iD].delta_min[j]].active_res_opp==0)
			{
				admissible_opp_insert(admissible_edges[v_iD],&edges[vertices[v_iD].delta_min[j]],v_iD);
			}
		}
}

void graph::admissible_init(){
	for (int i = 0; i < number_vertices; ++i)
	{
		admissible_update(i);
	}
}