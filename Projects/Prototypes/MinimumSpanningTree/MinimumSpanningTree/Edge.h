#pragma once
class V2;
class Edge
{
public:
	Edge();
	Edge(V2* newStart, V2* newEnd);
	~Edge();

	V2* start;
	V2* end;

	bool operator== (Edge edge);
};

