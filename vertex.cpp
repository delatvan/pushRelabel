/*
delatvan@gmail.com
*/

#include "vertex.hpp"
#include "edge.hpp"

#include <fstream> 
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

vertex::vertex()
{
	iD = -1;
	rank = 0;
	height = 0;
	active=0;
	excess = 0;
	next = NULL;
	prev = NULL;
}

vertex::vertex(int iD_, int height_)
{
	iD = iD_;
	rank =0;
	active = 0;
	excess = 0;
	height = height_;
}

vertex::~vertex()
{}

void vertex::print(){

	cout<<"iD = "<<iD<<", height = "<<height<<" ,excess = "<<excess<<endl;
	cout<<"delta plus edges:"<<endl;
	for (int i = 0; i < delta_plus.size(); ++i)
	{
		cout<<delta_plus[i]<<" "<<endl;
	}
	cout<<"delta minus edges:"<<endl;
	for (int j = 0; j < delta_min.size(); ++j)
	{
		cout<<delta_min[j]<<" "<<endl;
	}
	cout<<endl;
}