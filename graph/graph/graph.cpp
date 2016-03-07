#include "graph.h"

Edge::Edge( Node* st, Node* ed, double dis, double speed ) : start_node( st ), end_node( ed ), Distance_( dis ), Speed_( speed )
{
	if( st==NULL || ed==NULL || dis==0 || speed==0 ){
		std::cout << "warning: parameter used to init egde is not safe (point to null node or dis/speed==0)!" << std::endl;
		return;
	}
}
Edge::~Edge()
{

}

Node::Node( std::string node_name ) : name_( node_name )
{	
	if( node_name=="" ){
		std::cout << "warning: the name used to init node is empty!" << std::endl;
	}
	edges.clear();
}
Node::~Node()
{

}

graph::~graph(){
	//release Nodes_ and Edges_ memory
	while( Nodes_.begin()!=Nodes_.end() ){
		delete *Nodes_.begin();
		Nodes_.erase( Nodes_.begin() );
	}
	while( Edges_.begin()!=Edges_.end() ){
		delete *Edges_.begin();
		Edges_.erase( Edges_.begin() );
	}
};

void graph::AddNode( Node* node_ )
{
	//check name
	 if( node_->GetNodeName()=="" ){
		 std::cout << "error: the name of node is empty!" << std::endl;
		 delete node_;
		 return;
	 }
	//whether this node has added to the graph
	vString::iterator iter = std::find( node_list.begin(), node_list.end(), node_->GetNodeName() );
	if( iter!=node_list.end() ){
		std::cout << "error: the name of node has existed in the graph!" << std::endl;
		delete node_;
		return;
	}
	else{//add node to graph
		node_list.push_back( node_->GetNodeName() );
		Nodes_.push_back( node_ );
		return;
	}
}
void graph::DeleteNode( Node* node_ )
{
	//check point of edge_
	if( node_==NULL ){
		std::cout << "error: the node's point is NULL!" << std::endl;
		return;
	}

	//delete all edges linked to the node
	while( node_->GetEdges().begin()!=node_->GetEdges().end() ){
		DeleteEdge( *node_->GetEdges().begin() );
	}

	//delete node from graph -- node_list and Nodes_
	Nodes_.erase( std::find( Nodes_.begin(), Nodes_.end(), node_ ) );
	node_list.erase( node_list.begin() + ( std::find( Nodes_.begin(), Nodes_.end(), node_ ) - Nodes_.begin() ) );

	//release node memory
	delete node_;

	return;
}

void graph::AddEdge( Edge* edge_ )
{
	//whether Graph has all nodes of edge
	vpNode::iterator iter_st, iter_ed;
	iter_st = std::find( Nodes_.begin(), Nodes_.end(), edge_->GetStartNode() );
	iter_ed = std::find( Nodes_.begin(), Nodes_.end(), edge_->GetEndNode() );
	if( iter_st==Nodes_.end() || iter_ed==Nodes_.end() ){
		std::cout << "error: Graph do not has all nodes of this edge!" << std::endl;
		delete edge_;
		return;	
	}
	else{
		//Add edge to graph
		Edges_.push_back( edge_ );
		
		//link edge to nodes
		edge_->GetStartNode()->GetEdges().push_back( edge_ );
 		edge_->GetEndNode()->GetEdges().push_back( edge_ );

		return;
	}
}

void graph::DeleteEdge( Edge* edge_ )
{
	//check point of edge_
	if( edge_==NULL ){
		std::cout << "error: the edge's point is NULL!" << std::endl;
		return;
	}

	//delete from nodes
	vpEdge::iterator iter_st, iter_ed;
	iter_st = std::find( edge_->GetStartNode()->GetEdges().begin(), edge_->GetStartNode()->GetEdges().end(), edge_ );
	edge_->GetStartNode()->GetEdges().erase( iter_st );

	iter_ed = std::find( edge_->GetEndNode()->GetEdges().begin(), edge_->GetEndNode()->GetEdges().end(), edge_ );
	edge_->GetEndNode()->GetEdges().erase( iter_ed );

	//delete from graph
	Edges_.erase( std::find( Edges_.begin(), Edges_.end(), edge_ ) );

	//release edge memory
	delete edge_;

}

void graph::AddNodeByName( std::string node_name )
{
	Node* node_new = new Node( node_name );
	AddNode( node_new );
}

void graph::DeleteNodeByName( std::string node_name )
{
	vString::iterator iter = std::find( node_list.begin(), node_list.end(), node_name );
	Node* node_delete = *( Nodes_.begin() + ( iter - node_list.begin()) );
	DeleteNode( node_delete );
}

void graph::AddEdgeByNames( std::string node_name_st, std::string node_name_ed, double dis, double speed )
{
	//if nodes are exist in graph
	vString::iterator iter_st, iter_ed;
	iter_st = std::find( node_list.begin(), node_list.end(), node_name_st );
	iter_ed = std::find( node_list.begin(), node_list.end(), node_name_ed );

	if( iter_st==node_list.end() || iter_ed==node_list.end() ){
		std::cout << "error: not all names belong to nodes of graph" << std::endl;
		return;
	}

	//create and add edge
	Edge* edge_temp = new Edge( Nodes_[iter_st - node_list.begin()], Nodes_[iter_ed - node_list.begin()], dis, speed );
	AddEdge( edge_temp );

}


void graph::DeleteEdgeByNames( std::string node_name_st, std::string node_name_ed )
{
	DeleteEdge( GetEdgeByNames( node_name_st, node_name_ed ) );
}

Node* graph::GetNodeByName( std::string node_name )
{
	vString::iterator iter = std::find( node_list.begin(), node_list.end(), node_name );
	if( iter==node_list.end() ){
		std::cout << "error: the name of node isn't existed in the graph!" << std::endl;
		return NULL;
	}
	return Nodes_[iter - node_list.begin()];
}


Edge* graph::GetEdgeByNames( std::string node_name_st, std::string node_name_ed )
{
	//if nodes are exist in graph
	vString::iterator iter_st, iter_ed;
	iter_st = std::find( node_list.begin(), node_list.end(), node_name_st );
	iter_ed = std::find( node_list.begin(), node_list.end(), node_name_ed );

	if( iter_st==node_list.end() || iter_ed==node_list.end() ){
		std::cout << "error: not all names belong to nodes of graph" << std::endl;
		return NULL;
	}

	Node *node_st = GetNodeByName( node_name_st );
	Node *node_ed = GetNodeByName( node_name_ed );

	vpEdge::iterator iter=node_st->GetEdges().begin();
	for( ; iter!=node_st->GetEdges().end(); ++iter){
		if( std::find( node_ed->GetEdges().begin(), node_ed->GetEdges().end(), *iter )!=node_ed->GetEdges().end() ){
			break;
		}
	}

	if( iter==node_st->GetEdges().end() ){
//		std::cout << "the edge is not in graph!" << std::endl;
		return NULL;
	}

	return *iter;
}


void graph::PrintNodes()
{
	vpNode& nodes = GetNodes();
	std::cout << "All nodes of graph:" << std::endl;
	for( vpNode::iterator iter=nodes.begin(); iter!=nodes.end(); ++iter){
		vpEdge edges_node = (*iter)->GetEdges();
		std::cout << "<Node_" << iter-nodes.begin() << ">" << std::endl;
		std::cout << "Name:	" << (*iter)->GetNodeName() << "\r\nNum of edges:	" << edges_node.size() << "\r\n" << std::endl;
	}
	std::cout << "--------------\r\n" << std::endl;
}


void graph::PrintEdges()
{
	vpEdge& edges = GetEdges();
	std::cout << "All edges of graph:" << std::endl;
	for( vpEdge::iterator iter=edges.begin(); iter!=edges.end(); ++iter){
		std::cout << "<edge_" << iter-edges.begin() << ">" << std::endl;
		std::cout	<< " node_st: " 
					<< (*iter)->GetStartNode()->GetNodeName()
					<< "\r\n node_ed: "
					<< (*iter)->GetEndNode()->GetNodeName()
					<< "\r\n distance: "
					<< (*iter)->GetDis()
					<< "\r\n speed: "
					<< (*iter)->GetSpeed()
					<< "\r\n time: "
					<< (*iter)->GetTime()
					<< "\r\n" <<std::endl;
	}
	std::cout << "-------------\r\n" << std::endl;
}


