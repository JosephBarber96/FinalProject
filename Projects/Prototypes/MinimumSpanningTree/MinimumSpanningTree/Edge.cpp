#include "Edge.h"
#include "V2.h"

Edge::Edge() {}

Edge::Edge(V2* newStart, V2* newEnd)
	:
	start(newStart),
	end(newEnd)
{}

Edge::~Edge() {}

bool Edge::operator== (Edge edge)
{
	return
		(
			(*start == *edge.start && *end == *edge.end)
			||
			(*start == *edge.end && *end == *edge.start)
		);
}