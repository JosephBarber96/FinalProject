#include "MstEdge.h"
#include "MstNode.h"
#include "Vec2.h"

MstEdge::MstEdge() {}

MstEdge::MstEdge(MstNode* _start, MstNode* _end)
	:
	start(_start),
	end(_end)
{
	length = Vec2::DistanceBetween(*start->position, *end->position);
}

MstEdge::~MstEdge() {}

bool MstEdge::EdgeContainsNode(MstNode node)
{
	return (*start == node || *end == node);
}

bool MstEdge::operator== (const MstEdge edge)
{
	return
		(
			(*start == *edge.start && *end == *edge.end)
			||
			(*start == *edge.end && *end == *edge.start)
		);
}

bool MstEdge::operator< (const MstEdge edge)
{
	return length < edge.Length();
}

bool MstEdge::operator> (const MstEdge edge)
{
	return length > edge.Length();
}