#pragma once
class Node;
class Edge
{
public:
	Edge();
	Edge(Node* newStart, Node* newEnd);
	~Edge();

	Node* start;
	Node* end;
	
	float length;
	float EdgeLength();
	bool EdgeContainsNode(Node* node);

	bool operator== (const Edge& edge);
	bool operator< (const Edge edge) const;
	bool operator> (const Edge edge) const;

	//friend bool operator< (const Edge& lhs, const Edge& rhs)
	//{
	//	return (lhs.length < rhs.length);
	//}

	//friend bool operator> (const Edge& lhs, const Edge& rhs)
	//{
	//	return (lhs.length > rhs.length);
	//}
};

