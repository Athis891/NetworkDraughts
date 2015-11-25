#pragma once
#include "Application.h"
#include "Gizmos.h"
#include "GLMHeader.h"
#include "Camera.h"
#include <vector>
#include "Checker.h"
#include <iostream>
#include "GameMessages.h"
#include "TurnData.h"
#include "AntTweakBar.h"


#include "ServerData.h"
#include "ClientData.h"

class Draughts :public Application
{
public:
	virtual bool startup();
	virtual void shutdown();

	virtual bool update();
	virtual void draw();

	void initServer();
	void initClient();

	void drawBoard();

	void updateServer();
	void updateClient();

	void applyTurn(TurnData turn);

	void sendTurn(TurnData turn);

	void selectTile();
	Checker* getCheck(vec2 gridPos);
	void resetVisualisation();
	void displayAvailableMoves();
	void calculateAvailableMoves();
	bool rigAvailableMoves(vec2 target);

	void removeCheckAtPoint(vec2 target);
	bool calculateCheckAvailableMoves(Checker* check,bool onlyCaptures);


private:
	FlyCamera m_camera;

	std::vector<Checker*> checkers;

	//visualisation
	vec2 hoverTile;
	vec2 selectedTile;
	std::vector<vec2> availableMoves;


	int playerID;
	int playerTurn;


	//temp input
	vec4 passInput;


	//will only populate with data of appropriate application type
	ServerData serverData;
	ClientData clientData;



};


