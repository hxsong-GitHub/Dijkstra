// graph.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>//sort的使用

#include "graph.h"

#define TIME_MAX INT_MAX

typedef std::vector<size_t> Route;

std::vector<size_t>* Dijkstra( graph &graph_, std::string node_from_, std::string node_to_ );
void InputGraphFromTXT( graph& graph_, std::string graph_path );
void findTop( int *vt, int *minNum, int*iter, int length, int topNum )
{
	for( size_t i=0; i!=topNum; ++i ){  

		for( size_t j=0; j!=length; ++j ){

			int updateNum=1;
			if( minNum[i]>=vt[j] ){

				for( size_t k=0; k!=i; ++k ){ //
					if( j==iter[k] ){
						updateNum=0;
					}
				}
				if( updateNum ){
					minNum[i]	= vt[j];
					iter[i]		= j;
				}
			}
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	
	int sortVector[] = { 2, 21, 6, 8, 9, 5, 11, 7, 2, 10 };
	int minNum[]={INT_MAX,INT_MAX,INT_MAX,INT_MAX}, iter[]={INT_MAX,INT_MAX,INT_MAX,INT_MAX};

	findTop(sortVector, minNum, iter, 10, 4);

	int aaa=4;







	/*
	graph MyGraph;
	std::string graph_data_path, command;

	std::cout << "Input Graph data file path or\r\nEnter [0] to load the default file." << std::endl;
	std::cin >> command;
	if( command=="0" ){
		graph_data_path = "graph_data.txt";
	}
	else{
		graph_data_path = command;
	}
	InputGraphFromTXT( MyGraph, graph_data_path );	
	std::cout	<< "\r\n   Enter command"  << std::endl;

	while (true)
	{
		std::cout	<< "[0] -- print nodes of graph.\r\n" 
					<< "[1] -- print edges of graph.\r\n" 
					<< "[2] -- add node to graph.\r\n"
					<< "[3] -- add edge to graph.\r\n"
					<< "[4] -- delete node from graph.\r\n"
					<< "[5] -- delete edge from graph.\r\n"
					<< "[6] -- find route in graph.\r\n"
					<< "[7] -- exit app.\r\n"
			<< std::endl;
		int int_command;
		std::cin >> int_command;
		switch (int_command)
		{
		case 0:{
			MyGraph.PrintNodes();
			break;
			   }
		case 1:{
			MyGraph.PrintEdges();
			break;
			   }
		case 2:{
			std::string node_name;
			std::cout << "Input name of node you want to add" << std::endl;
			std::cin >> node_name;
			MyGraph.AddNodeByName( node_name );
			break;
			   }
		case 3:{
			std::string node_st, node_ed;
			double dis, speed;
			std::cout << "Input name of start node" << std::endl;
			std::cin >> node_st;
			std::cout << "Input name of end node" << std::endl;
			std::cin >> node_ed;
			std::cout << "Input distance of edge" << std::endl;
			std::cin >> dis;
			std::cout << "Input speed of edge" << std::endl;
			std::cin >> speed;
			MyGraph.AddEdgeByNames( node_st, node_ed, dis, speed );
			break;
			   }
		case 4:{
			std::string node_name;
			std::cout << "Input name of node you want to delete" << std::endl;		
			std::cin >> node_name;
			MyGraph.DeleteNodeByName( node_name );
			break;
			   }
		case 5:{
			std::string node_st, node_ed;
			std::cout << "Input name of start node of the edge you want to delete" << std::endl;
			std::cin >> node_st;
			std::cout << "Input name of end node of the edge you want to delete" << std::endl;
			std::cin >> node_ed;
			MyGraph.DeleteEdgeByNames( node_st, node_ed );
			break;
			   }
		case 6:{
			std::string node_from, node_to;
			std::cout << "Input name of node you want to start" << std::endl;
			std::cin >> node_from;
			std::cout << "Input name of node you want to arrived" << std::endl;
			std::cin >> node_to;
			Route* fast = Dijkstra( MyGraph, node_from, node_to );
			if( fast!=NULL ){
				//print route 
				std::cout << "route:" << std::endl;
				for( Route::iterator iter=fast->begin(); iter!=( fast->end()-1 ); ++iter){
					vpNode &nodes = MyGraph.GetNodes(); 
					std::cout	<< nodes[*iter]->GetNodeName() << std::endl;
				}

				//total time
				std::cout << "total time:" << 60*static_cast<double>(*(fast->end()-1))/10000 << "  minute." << std::endl;
			}
			break;
			   }
		case 7:{
			exit(1);
			break;
			   }
		default:
			break;
		}
	}
	*/

	return 0;
}


//return a size_t vector, last is the total time,others are node of route
Route* Dijkstra( graph &graph_, std::string node_from_, std::string node_to_)
{
	Node *node_from, *node_to;
	node_from	= graph_.GetNodeByName( node_from_ );
	node_to		= graph_.GetNodeByName( node_to_ );
	 
	//check input
	if( node_from==NULL ){
		std::cout << "error: node " << node_from_ << "is not in graph" << std::endl;
		return NULL;
	}
	if( node_to==NULL ){
		std::cout << "error: node " << node_to_ << "is not in graph" << std::endl;
		return NULL;
	}

	//init parameter
	vpNode& nodes = graph_.GetNodes();
	vpEdge& edges =  graph_.GetEdges();
	vpNode::iterator iter_from, iter_to;
	iter_from	= std::find( nodes.begin(), nodes.end(), node_from);
	iter_to		= std::find( nodes.begin(), nodes.end(), node_to);

	//init route
	std::vector<Route*> fast_route;//fast route from node_from to each node
	for( vpNode::iterator iter=nodes.begin(); iter!=nodes.end(); ++iter){
		Route* new_route = new Route;
		fast_route.push_back( new_route);
	}
	fast_route[iter_from - nodes.begin()]->push_back( iter_from - nodes.begin() );

	//init s_, u_ pool and time_total, set s_ -- get the fast route, u_ -- haven't get the fast route
	std::vector<size_t> s_, u_;
	std::vector<double> time_total;//fast time to reach node
	for( vpNode::iterator iter = nodes.begin(); iter!=nodes.end(); ++iter){
		time_total.push_back( TIME_MAX );
		u_.push_back( iter - nodes.begin() );
	}
	s_.push_back( iter_from - nodes.begin() );
	u_.erase( std::find( u_.begin(), u_.end(), *s_.begin() ) );
	time_total[*s_.begin()] = 0;
	

	//iterator to find route
	while( std::find( s_.begin(), s_.end(), iter_to - nodes.begin())==s_.end() ){		
		
		double time_min( TIME_MAX );
		std::vector<size_t>::iterator iter_min = u_.end();

		//update time_total in u_ and find min time in u_
		for( std::vector<size_t>::iterator iter = u_.begin(); iter!=u_.end(); ++iter){
			Edge* edge_iter = graph_.GetEdgeByNames( nodes[*(s_.end() - 1)]->GetNodeName(), nodes[*iter]->GetNodeName() );
			double time_temp;

			//if *iter in u_ don't link to *(s_.end() - 1), set the reach time MAX
			if( edge_iter==NULL ){
				time_temp = TIME_MAX;
			}
			else{
				time_temp = edge_iter->GetTime() + time_total[*(s_.end() - 1)];
			}			

			//if last one add to s_ can reach a shorter time, update route
			if( time_total[*iter] > time_temp){
				time_total[*iter] = time_temp;
				fast_route[*iter]->clear();
				fast_route[*iter]->insert( fast_route[*iter]->begin(), fast_route[*(s_.end()-1)]->begin(), fast_route[*(s_.end()-1)]->end());
				fast_route[*iter]->push_back( *iter );
			}

			//the min time for this loop
			if( time_total[*iter] < time_min ){
				time_min = time_total[*iter];
				iter_min = iter;
			}		
		}

		//add the min u_ to s_ ,
		s_.push_back( *iter_min );
		u_.erase( iter_min );

	}

	//	return route;
	Route* route_ret = new Route;
	route_ret->insert( route_ret->begin(), fast_route[iter_to - nodes.begin()]->begin(), fast_route[iter_to - nodes.begin()]->end() );
	route_ret->push_back( (time_total[iter_to - nodes.begin()])*10000 );

	//release route memory
	while( fast_route.begin()!= fast_route.end() ){
		delete *fast_route.begin();
		fast_route.erase( fast_route.begin() );
	}

	return route_ret;
}

void InputGraphFromTXT( graph& graph_, std::string graph_path){
	vString node_list;
	vpNode nodes_input;
	vpEdge edges_input;

	std::fstream graph_data;
	std::string sTemp;

	graph_data.open(graph_path);
	if(!graph_data){
		std::cout << "error: Fail to open graph data file!" << std::endl;
		return;
	}

	//X & Y
	graph_data >> sTemp;
	if(sTemp!="<ID>"){
		std::cout << "error: graph data -- <ID> input error!" << std::endl;
		return;
	}
	int x_input, y_input;
	graph_data >> sTemp;
	if(sTemp!="<X>"){
		std::cout << "error: graph data -- X input error!" << std::endl;
		return;
	}
	graph_data >> x_input;
	graph_data >> sTemp;
	if(sTemp!="<Y>"){
		std::cout << "error: graph data -- Y input error!" << std::endl;
		return;
	}
	graph_data >> y_input;
	graph_data >> sTemp;
	if(sTemp!="</ID>"){
		std::cout << "error: graph data -- </ID> input error!" << std::endl;
		return;
	}

	//num of node
	int num_of_node;
	graph_data >> sTemp;
	if(sTemp!="<Num_of_Node>"){
		std::cout << "error: graph data -- <Num_of_Node> input error!" << std::endl;
		return;
	}
	graph_data >> num_of_node;

	//read and create nodes
	graph_data >> sTemp;
	if(sTemp!="<Nodes>"){
		std::cout << "error: graph data -- <Nodes> input error!" << std::endl;
		return;
	}
	for( size_t i=0; i!=num_of_node; ++i ){
		graph_data >> sTemp;
		Node* node_temp = new Node(sTemp);
		nodes_input.push_back(node_temp);
		node_list.push_back(sTemp);
	}
	graph_data >> sTemp;
	if(sTemp!="</Nodes>"){
		std::cout << "error: graph data -- </Nodes> input error!" << std::endl;
		//if error, release memory
		while ( nodes_input.begin()!=nodes_input.end() ){
			delete *( nodes_input.begin() );
			nodes_input.erase( nodes_input.begin() );
		}
		return;
	}

	//num of edges
	int num_of_edges;
	graph_data >> sTemp;
	if(sTemp!="<Num_of_Edges>"){
		std::cout << "error: graph data -- <Num_of_Edges> input error!" << std::endl;
		//if error, release memory
		while ( nodes_input.begin()!=nodes_input.end() ){
			delete *( nodes_input.begin() );
			nodes_input.erase( nodes_input.begin() );
		}
		return;
	}
	graph_data >> num_of_edges;

	//read edges
	graph_data >> sTemp;
	if(sTemp!="<Edges>"){
		std::cout << "error: graph data -- <Edges> input error!" << std::endl;
		//if error, release memory
		while ( nodes_input.begin()!=nodes_input.end() ){
			delete *( nodes_input.begin() );
			nodes_input.erase( nodes_input.begin() );
		}
		return;
	}
	for(size_t i=0; i!=6; ++i){
		graph_data >> sTemp;
	}

	for( size_t i=0; i!=num_of_edges&&!graph_data.eof(); ++i ){
		std::string node_st, node_ed;
		int dis, basic_speed, x_or_y, plus;
		graph_data >> node_st >> node_ed >> dis >> basic_speed >> x_or_y >> plus;
		if( node_st=="" || node_ed==""){
			std::cout << "error: edge parameter is not complete!" << std::endl;
			//if error, release memory
			while ( nodes_input.begin()!=nodes_input.end() ){
				delete *( nodes_input.begin() );
				nodes_input.erase( nodes_input.begin() );
			}
			while ( edges_input.begin()!=edges_input.end() ){
				delete *( edges_input.begin() );
				edges_input.erase( edges_input.begin() );
			}		
			return;
		}
		vString::iterator iter_st, iter_ed;
		iter_st = std::find( node_list.begin(), node_list.end(), node_st );
		iter_ed = std::find( node_list.begin(), node_list.end(), node_ed );
		if( iter_st!=node_list.end() && iter_ed!=node_list.end() && iter_st!=iter_ed){
			Edge* edge_temp = new Edge( nodes_input[iter_st-node_list.begin()],
				nodes_input[iter_ed-node_list.begin()],
				dis, basic_speed + plus*(x_or_y?y_input:x_input) );
			edges_input.push_back(edge_temp);
		}
		else{
			std::cout << "error: edges!" << std::endl;
			//if error, release memory
			while ( nodes_input.begin()!=nodes_input.end() ){
				delete *( nodes_input.begin() );
				nodes_input.erase( nodes_input.begin() );
			}
			while ( edges_input.begin()!=edges_input.end() ){
				delete *( edges_input.begin() );
				edges_input.erase( edges_input.begin() );
			}
			return;
		}
	}
	graph_data >> sTemp;
	if(sTemp!="</Edges>"){
		std::cout << "error: graph data -- </Edges> input error!" << std::endl;
		//if error, release memory
		while ( nodes_input.begin()!=nodes_input.end() ){
			delete *( nodes_input.begin() );
			nodes_input.erase( nodes_input.begin() );
		}
		while ( edges_input.begin()!=edges_input.end() ){
			delete *( edges_input.begin() );
			edges_input.erase( edges_input.begin() );
		}
		return;
	}

	//write data to Graph
	for( vpNode::iterator iter=nodes_input.begin(); iter!=nodes_input.end(); ++iter ){
		graph_.AddNode( *iter );
	}
	for( vpEdge::iterator iter=edges_input.begin(); iter!=edges_input.end(); ++iter ){
		graph_.AddEdge( *iter );
	}
	graph_data.close();
}


