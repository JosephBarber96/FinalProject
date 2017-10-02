#include "Edge.h"
#include "Node.h"
#include "V2.h"

Edge::Edge() {}

Edge::Edge(Node* newStart, Node* newEnd)
	:
	start(newStart),
	end(newEnd)
{
	length = EdgeLength();
}

Edge::~Edge() {}

float Edge::EdgeLength()
{
	return (V2::DistanceBetween(*start->position, *end->position));
}

bool Edge::EdgeContainsNode(Node* node)
{
	return (*start == *node || *end == *node);
}

bool Edge::operator== (const Edge& edge)
{
	return
		(
			(*start == *edge.start && *end == *edge.end)
			||
			(*start == *edge.end && *end == *edge.start)
		);
}

bool Edge::operator< (const Edge edge) const
{
	return length < edge.length;

	//return (EdgeLength() < edge.EdgeLength());
}

bool Edge::operator> (const Edge edge) const
{
	return length > edge.length;

	//return (EdgeLength() > edge.EdgeLength());
}