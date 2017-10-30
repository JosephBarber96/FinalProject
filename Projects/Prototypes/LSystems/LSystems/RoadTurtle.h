#pragma once
#include <vector>
#include <stack>
class Transform;
class Vec2;
class Road;
class RoadTurtle
{
public:
	RoadTurtle();
	~RoadTurtle();

	void SetPos(Vec2* newPos);
	void SetAngle(float deg);

	void SetStartingTransform(Vec2* startPos, float startAngle);

	void ExtendRoad(float length);
	void Rotate(float deg);
	void Branch();
	void Push();
	void Pop();

	void AddRoad(Road* road);
	void AddRoad(Vec2* roadStart, Vec2* roadEnd);

	std::vector<Road*> getRoads() { return roads; }
	void SetNewList(std::vector<Road*> newList) { roads = newList; }

	int roadStackSize() { return roadStack.size(); }
	int transformStackSize() { return transformStack.size(); }
private:
	Transform* transform;
	Road* currentRoad;
	std::vector<Road*> roads;

	std::stack<Road*> roadStack;
	std::stack<Transform*> transformStack;
};

