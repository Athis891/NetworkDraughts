#include "Checker.h"

Checker::Checker(vec2 a_gridPos, vec4 a_color, int a_team)
{
	gridPos = a_gridPos;
	color = a_color;
	isKing = false;
	team = a_team;
}

void Checker::Draw()
{ 
	if (isKing == false)
	{
		Gizmos::addCylinderFilled(vec3(((gridPos.x - 4) * 2) + 1, 0.1, ((gridPos.y - 4) * 2) + 1), 0.8, 0.09, 30, color);
	}
	else
	{
		Gizmos::addCylinderFilled(vec3(((gridPos.x - 4) * 2) + 1, 0.3, ((gridPos.y - 4) * 2) + 1), 0.8, 0.27, 30, color);
	}
}