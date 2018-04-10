#pragma once
#include <vector>
#include <stack>
class Transform;
class Vec2;
class Road;
class PopulationPoint;
class RoadTurtle
{
public:
	RoadTurtle();
	~RoadTurtle();

	void SetPos(Vec2* newPos);
	void SetPos(PopulationPoint* newPos);
	void SetAngle(float deg);

	void SetStartingTransform(Vec2* startPos, float startAngle);

	void ExtendRoad(float length);
	void ExtendUntilConnect(float length);
	void Rotate(float deg);
	void Branch();
	void Push();
	void Pop();
	bool FindNewTarget(std::vector<PopulationPoint*>& popPoints);

	void SetCurrentTarget(PopulationPoint* newTarget);
	void AddRoad(Road* road);
	void AddRoad(Vec2* roadStart, Vec2* roadEnd);
	void SetNewList(std::vector<Road*> newList) { roads = newList; }

	int roadStackSize() const { return roadStack.size(); }
	int transformStackSize() const { return transformStack.size(); }
	PopulationPoint* getCurrentTarget() const { return currentTarget; }
	Transform* getTransform() const { return transform; }
	std::vector<Road*> getRoads() const { return roads; }
private:
	PopulationPoint* currentTarget;
	Transform* transform;
	Road* currentRoad;
	std::vector<Road*> roads;

	std::stack<Road*> roadStack;
	std::stack<Transform*> transformStack;
};

