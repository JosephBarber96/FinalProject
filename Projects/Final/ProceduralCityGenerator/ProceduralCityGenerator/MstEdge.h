#pragma once

class MstNode;
class MstEdge
{
public:
	MstEdge();
	MstEdge(MstNode* _start, MstNode* _end);
	~MstEdge();

	MstNode* start;
	MstNode* end;

	bool partOfTree;

	float Length() const { return length; }
	bool EdgeContainsNode(MstNode node);

	bool operator== (const MstEdge edge);
	bool operator< (const MstEdge edge);
	bool operator> (const MstEdge edge);

private:
	float length;
};