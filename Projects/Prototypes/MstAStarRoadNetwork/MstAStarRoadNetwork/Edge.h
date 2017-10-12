#pragma once
class MstNode;
class Edge
{
public:
	Edge();
	Edge(MstNode* newStart, MstNode* newEnd);
	~Edge();

	MstNode* start;
	MstNode* end;

	float length;
	bool partOfTree;
	float EdgeLength();
	bool EdgeContainsNode(MstNode node);

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

