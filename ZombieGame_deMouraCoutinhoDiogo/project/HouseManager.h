#pragma once
#include "Exam_HelperStructs.h"
#include <unordered_map>
#include <vector>
class IBaseInterface;
class IExamInterface;



using namespace std;
using namespace Elite;


struct VisitedHouse
{
	Vector2 Location;
	float timeSinceVisited;
	bool needRevisiting;
};



class HouseManager
{
public:



private:
	//unordered_map<int, VisitedHouse> m_Visited;
	vector<VisitedHouse> m_Visited;
};

