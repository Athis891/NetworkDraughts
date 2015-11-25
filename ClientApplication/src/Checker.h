#pragma once

#include "Gizmos.h"
#include "GLMHeader.h"
#include <vector>

class Checker
{
public:
	Checker();
	Checker(vec2 a_gridPos, vec4 a_color, int a_team);

	void Draw();

	vec2 gridPos;
	vec4 color;


	bool isKing;
	int team;

	std::vector<vec2> availableMoves;


};