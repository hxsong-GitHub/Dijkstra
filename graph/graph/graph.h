//

#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <iostream>

class Edge;
class Node;
class graph;

typedef std::vector<std::string>	vString;
typedef std::vector<Node*>			vpNode;
typedef std::vector<Edge*>			vpEdge;

class Edge{
public:
	Edge( Node* st, Node* ed, double dis, double speed );
	~Edge();

	Node* GetStartNode(){ return start_node;}
	Node* GetEndNode(){ return end_node;}
	double GetDis(){ return Distance_ ;}
	double GetSpeed(){ return Speed_; }
	double GetTime(){ return Distance_/Speed_; };

private:
	Node *const start_node;
	Node *const end_node;

	const double Distance_;
	const double Speed_;
	
};

class Node{
public:
	Node( std::string node_name );
	~Node();

	std::string GetNodeName(){ return name_; }
	vpEdge& GetEdges(){ return edges; }

private:
	const std::string name_;
	vpEdge edges;
};


class graph
{
public:
	graph() : Nodes_( NULL ), Edges_( NULL ){};
	~graph();


	//basic operate
	void AddNode( Node* node_ );
	void DeleteNode( Node* node_ );
	void AddEdge( Edge* edge_ );
	void DeleteEdge( Edge* edge_ );

	//convenience operate
	void AddNodeByName( std::string node_name );
	void DeleteNodeByName( std::string node_name );
	void AddEdgeByNames( std::string node_name_st, std::string node_name_ed, double dis, double speed );
	void DeleteEdgeByNames( std::string node_name_st, std::string node_name_ed );
	
	//get info of graph
	Node*   GetNodeByName( std::string node_name );
	Edge*   GetEdgeByNames( std::string node_name_st, std::string node_name_ed );
	vpNode& GetNodes(){ return Nodes_; }
	vpEdge& GetEdges(){ return Edges_; }
	void PrintNodes();
	void PrintEdges();


private:
	vString node_list;//in case of two node have same names;
	vpNode Nodes_;
	vpEdge Edges_;

};


#endif // !GRAPH_H
