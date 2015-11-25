#include "Draughts.h"

void OnMouseButton(GLFWwindow*, int b, int a, int m) {
	TwEventMouseButtonGLFW(b, a);
}
void OnMousePosition(GLFWwindow*, double x, double y) {
	TwEventMousePosGLFW((int)x, (int)y);
}
void OnMouseScroll(GLFWwindow*, double x, double y) {
	TwEventMouseWheelGLFW((int)y);
}
void OnKey(GLFWwindow*, int k, int s, int a, int m) {
	TwEventKeyGLFW(k, a);
}
void OnChar(GLFWwindow*, unsigned int c) {
	TwEventCharGLFW(c, GLFW_PRESS);
}
void OnWindowResize(GLFWwindow*, int w, int h) {
	TwWindowSize(w, h);
	glViewport(0, 0, w, h);
}

bool Draughts::startup()
{
	bool pass = false;
	playerTurn = 2;
	while (pass == false)
	{
		std::string input;

		std::cout << "Input Player Number: ";
		std::getline(std::cin, input);

		if (input == "1")
		{
			playerID = 1;
			pass = true;
			std::cout << "Welcome Server";
			std::cout << "\n";
			initServer();
			
		}
		else if (input == "2")
		{
			playerID = 2;
			pass = true;
			std::cout << "Welcome Client";
			std::cout << "\n";
			initClient();

		}
		else
		{
			std::cout << "Invalid";
			std::cout << "\n";
		}

		
	}


	passInput = vec4(0);


	if (Application::startup() == false)
	{
		return false;
	}

	m_camera = FlyCamera(1280.0f / 720.0f, 10.0f);
	m_camera.setLookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	m_camera.sensitivity = 3;

	//sets clear color (grey)
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	TwInit(TW_OPENGL_CORE, nullptr);
	TwWindowSize(1280, 720);



	glfwSetMouseButtonCallback(m_window, OnMouseButton);
	glfwSetCursorPosCallback(m_window, OnMousePosition);
	glfwSetScrollCallback(m_window, OnMouseScroll);
	glfwSetKeyCallback(m_window, OnKey);
	glfwSetCharCallback(m_window, OnChar);
	glfwSetWindowSizeCallback(m_window, OnWindowResize);

	TwBar* bar = TwNewBar("TWBar");

	TwAddVarRO(bar, "PlayerNumber", TW_TYPE_INT32, &playerID, "label='PlayerNumber'");
	TwAddVarRO(bar, "TurnNumber", TW_TYPE_INT32, &playerTurn, "label='TurnNumber'");


	hoverTile = vec2(5);


	//initialise checkers
	vec4 team1Color = vec4(1, 0, 0, 1);
	vec4 team2Color = vec4(0, 0, 1, 1);

	int currentIndex = 0;
	int continueCount = 12;
	bool team1 = true;
	bool addCheck = true;

	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (team1 == false && continueCount>0)
			{
				continueCount--;
			}
			else if (continueCount == 0 && team1 == true)
			{
				continueCount = 16;
				team1 = false;
			}
			else if (continueCount == 0 && team1 == false)
			{
				addCheck = true;
				continueCount--;
			}
			else if (addCheck == true)
			{


				if (team1 == true)
				{
					Checker* tempChecker=new Checker(vec2(x, y), team1Color, 1);
					checkers.push_back(tempChecker);
				}
				else
				{
					Checker* tempChecker = new Checker(vec2(x, y), team2Color, 2);
					checkers.push_back(tempChecker);
				}



				addCheck = false;
				continueCount--;
			}
			else
			{
				addCheck = true;
			}

		}

		if (addCheck == true)
		{
			addCheck = false;
		}
		else
		{
			addCheck = true;
		}
	}


	//Enable and Create Gizmos
	Gizmos::create();

	calculateAvailableMoves();


	return true;
}
void Draughts::shutdown()
{
	Gizmos::destroy();

	Application::shutdown();
}

bool Draughts::update()
{
	//Determines delta time and rotation values
	float dt = (float)glfwGetTime();
	glfwSetTime(0.0);

	Gizmos::clear();

	//resets color values
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	

	if (playerID == 1)
	{
		updateServer();
	}
	else
	{
		updateClient();
	}

	selectTile();
	
	m_camera.update(dt);

	//determines if the application should stay open or not
	return Application::update();
}
void Draughts::draw()
{
	drawBoard();

	for (int i = 0; i < checkers.size(); i++)
	{
		checkers[i]->Draw();
	}

	Gizmos::draw(m_camera.view_proj);

	TwDraw();


	glfwSwapBuffers(this->m_window);
	glfwPollEvents();
}

void Draughts::drawBoard()
{


	Gizmos::addAABB(vec3(0, 0, 0), vec3(8, 0.1, 8), vec4(1, 1, 1, 1));

	bool filled = true;
	vec4 filledColor = vec4(0.5, 0.5, 0.5, 1);
	vec4 hoverColor=vec4(1,1,0,1);
	vec4 selectColor = vec4(0, 1, 0, 1);
	vec4 avalableColor = vec4(0,1,1,1);

	vec4 targetColor;

	for (int x = 0; x < 8; x++)
	{
		for (int z = 0; z < 8; z++)
		{

			if (filled == true)
			{
				targetColor = filledColor;
			}
			else if (filled == false)
			{
				targetColor = vec4(1);
			}

			for (int i = 0; i < availableMoves.size(); i++)
			{
				if (x + 1 == availableMoves[i].x && z + 1 == availableMoves[i].y)
				{
					targetColor = avalableColor;
				}
			}

			if (x+1 == selectedTile.x && z+1 == selectedTile.y)
			{
				targetColor = selectColor;
			}
			else if (x + 1 == hoverTile.x && z + 1 == hoverTile.y)
			{
				targetColor = hoverColor;
			}






			if (filled == false)
			{
				Gizmos::addAABBFilled(vec3((x - 3.5) * 2, 0, (z - 3.5) * 2), vec3(0.9, 0, 0.9), targetColor);
				filled = true;
			}
			else
			{
				Gizmos::addAABBFilled(vec3((x - 3.5) * 2, 0, (z - 3.5) * 2), vec3(0.9, 0, 0.9), targetColor);

				filled = false;
			}
		}

		if (filled == false)
		{
			filled = true;
		}
		else
		{
			filled = false;
		}
	}
}

void Draughts::initServer()
{

	serverData.pPeerInterface = nullptr;

	//Startup the server, and start it listening to clients
	std::cout << "Starting up the server..." << std::endl;

	//Initialize the Raknet peer interface first
	serverData.pPeerInterface = RakNet::RakPeerInterface::GetInstance();

	//Create a socket descriptor to describe this connection
	RakNet::SocketDescriptor sd(serverData.PORT, 0);

	//Now call startup - max of 32 connections, on the assigned port
	serverData.pPeerInterface->Startup(32, &sd, 1);
	serverData.pPeerInterface->SetMaximumIncomingConnections(32);

	serverData.packet = nullptr;

}

void Draughts::initClient()
{
	std::string input;

	std::cout << "Input IP: ";
	std::getline(std::cin, input);
	if (input == "")
	{

	}
	else
	{
		clientData.IP = input.c_str();
	}
	
		std::cout << "\n";

	clientData.pPeerInterface = RakNet::RakPeerInterface::GetInstance();

	//Create a socket descriptor to describe this connection
	//No data needed, as we will be connecting to a server
	RakNet::SocketDescriptor sd;
	//Now call startup - max of 1 connections (to the server)
	clientData.pPeerInterface->Startup(1, &sd, 1);
	std::cout << "Connecting to server at: " << clientData.IP << std::endl;
	//Now call connect to attempt to connect to the given server
	RakNet::ConnectionAttemptResult res = clientData.pPeerInterface->Connect(clientData.IP, clientData.PORT, nullptr, 0);
	//Finally, check to see if we connected, and if not, throw a error
	if (res != RakNet::CONNECTION_ATTEMPT_STARTED)
	{
		std::cout << "Unable to start connection, Error number: " << res << std::endl;
	}
}

void Draughts::updateServer()
{
	for (serverData.packet = serverData.pPeerInterface->Receive(); serverData.packet;
		serverData.pPeerInterface->DeallocatePacket(serverData.packet),
		serverData.packet = serverData.pPeerInterface->Receive())
	{
		switch (serverData.packet->data[0])
		{
		case ID_NEW_INCOMING_CONNECTION:
			std::cout << "A connection is incoming.\n";

				serverData.client = serverData.packet->systemAddress;


			break;
		case ID_DISCONNECTION_NOTIFICATION:
			std::cout << "A client has disconnected.\n";


				serverData.client = nullptr;


			break;
		case ID_CONNECTION_LOST:
			std::cout << "A client lost the connection.\n";


				serverData.client = nullptr;


			break;
		case ID_TURN_DATA:
		{
			std::cout << "Turn Data recieved..\n";
			RakNet::BitStream bsIn(serverData.packet->data, serverData.packet->length, false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			
			TurnData turnData;
			bsIn.Read(turnData.t1x);
			bsIn.Read(turnData.t1y);
			bsIn.Read(turnData.t2x);
			bsIn.Read(turnData.t2y);

			applyTurn(turnData);

			break;
		}
		default:
			std::cout << "Received a message with a unknown id: " <<
				serverData.packet->data[0];
			break;
		}
	}
}

void Draughts::updateClient()
{
	RakNet::Packet* packet;


	for (packet = clientData.pPeerInterface->Receive(); packet;
		clientData.pPeerInterface->DeallocatePacket(packet),
		packet = clientData.pPeerInterface->Receive())
	{
		switch (packet->data[0])
		{
		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			std::cout << "Another client has disconnected.\n";
			break;
		case ID_REMOTE_CONNECTION_LOST:
			std::cout << "Another client has lost the connection.\n";
			break;
		case ID_REMOTE_NEW_INCOMING_CONNECTION:
			std::cout << "Another client has connected.\n";
			break;
		case ID_CONNECTION_REQUEST_ACCEPTED:
			std::cout << "Our connection request has been accepted.\n";
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			std::cout << "The server is full.\n";
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			std::cout << "We have been disconnected.\n";
			break;
		case ID_CONNECTION_LOST:
			std::cout << "Connection lost.\n";
			break;
		case ID_TURN_DATA:
		{
			std::cout << "Turn Data recieved.\n";
			RakNet::BitStream bsIn(packet->data, packet->length, false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

			TurnData turnData;
			bsIn.Read(turnData.t1x);
			bsIn.Read(turnData.t1y);
			bsIn.Read(turnData.t2x);
			bsIn.Read(turnData.t2y);

			applyTurn(turnData);

			break;
		}

		default:
			std::cout << "Received a message with a unknown id: " << packet->data[0];
			break;
		}
	}
}


void Draughts::applyTurn(TurnData turn)
{

	std::cout << "Move ";
	std::cout << turn.t1x;
	std::cout << ",";
	std::cout << turn.t1y;

	std::cout << " To ";
	std::cout << turn.t2x;
	std::cout << ",";
	std::cout << turn.t2y;

	std::cout<<"\n";

	bool capturedPiece=false;

	Checker* targetCheck=getCheck(vec2(turn.t1x,turn.t1y));
	targetCheck->gridPos = vec2(turn.t2x - 1, turn.t2y - 1);

	if (targetCheck->team == 1 && targetCheck->gridPos.y == 7)
	{
		targetCheck->isKing = true;
	}
	else if ( targetCheck->team==2 && targetCheck->gridPos.y ==0)
	{
		targetCheck->isKing = true;
	}

	if (turn.t1y + 2 == turn.t2y | turn.t1y-2==turn.t2y)
	{
		vec2 targetDelete;

		if (turn.t1x > turn.t2x)
		{
			targetDelete.x = turn.t1x - 1;
		}
		else
		{
			targetDelete.x = turn.t1x + 1;
		}

		if (turn.t1y > turn.t2y)
		{
			targetDelete.y = turn.t1y - 1;
		}
		else
		{
			targetDelete.y = turn.t1y + 1;
		}

		removeCheckAtPoint(targetDelete);
		capturedPiece = true;
	}
	

	if (capturedPiece == true && targetCheck->isKing==false)
	{
		if (rigAvailableMoves(vec2(turn.t2x, turn.t2y))==false)
		{
			calculateAvailableMoves();
			
		}

	}
	else
	{
		calculateAvailableMoves();
	}
	
	
}

void Draughts::sendTurn(TurnData turn)
{
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)GameMessages::ID_TURN_DATA);
	bsOut.Write(turn.t1x);
	bsOut.Write(turn.t1y);
	bsOut.Write(turn.t2x);
	bsOut.Write(turn.t2y);

	if (playerID == 1)
	{
		serverData.pPeerInterface->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, serverData.client, false);
	}
	else
	{
		clientData.pPeerInterface->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0,RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	}
	

}

void Draughts::selectTile()
{
	

	if (playerID == playerTurn)
	{
		GLFWwindow* curr_window = glfwGetCurrentContext();

		if (glfwGetKey(curr_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			resetVisualisation();
		}

		if (glfwGetKey(curr_window, GLFW_KEY_UP) == GLFW_PRESS &&passInput.x==0)
		{
			if (playerID == 1)
			{
				if (hoverTile.y == 8)
				{
					hoverTile.y = 1;
				}
				else
				{
					hoverTile.y++;
				}
			}
			else
			{
				if (hoverTile.y == 1)
				{
					hoverTile.y = 8;
				}
				else
				{
					hoverTile.y--;
				}
			}
			passInput.x = 1;
		}
		if (glfwGetKey(curr_window, GLFW_KEY_UP) == GLFW_RELEASE)
		{
			passInput.x = 0;
		}
		if (glfwGetKey(curr_window, GLFW_KEY_DOWN) == GLFW_PRESS&&passInput.y == 0)
		{
			if (playerID == 1)
			{
				if (hoverTile.y == 1)
				{
					hoverTile.y = 8;
				}
				else
				{
					hoverTile.y--;
				}
			}
			else
			{
				if (hoverTile.y == 8)
				{
					hoverTile.y = 1;
				}
				else
				{
					hoverTile.y++;
				}
			}
			passInput.y = 1;

		}
		if (glfwGetKey(curr_window, GLFW_KEY_DOWN) == GLFW_RELEASE)
		{
			passInput.y = 0;
		}
		if (glfwGetKey(curr_window, GLFW_KEY_RIGHT) == GLFW_PRESS&&passInput.z == 0)
		{
			if (playerID == 1)
			{
				if (hoverTile.x == 1)
				{
					hoverTile.x = 8;
				}
				else
				{
					hoverTile.x--;
				}
			}
			else
			{
				if (hoverTile.x == 8)
				{
					hoverTile.x = 1;
				}
				else
				{
					hoverTile.x++;
				}
			}
			passInput.z = 1;
		}
		if (glfwGetKey(curr_window, GLFW_KEY_RIGHT) == GLFW_RELEASE)
		{
			passInput.z = 0;
		}

		if (glfwGetKey(curr_window, GLFW_KEY_LEFT) == GLFW_PRESS&&passInput.w == 0)
		{
			if (playerID == 1)
			{
				if (hoverTile.x == 8)
				{
					hoverTile.x = 1;
				}
				else
				{
					hoverTile.x++;
				}
			}
			else
			{
				if (hoverTile.x == 1)
				{
					hoverTile.x = 8;
				}
				else
				{
					hoverTile.x--;
				}
			}
			passInput.w = 1;
		}
		if (glfwGetKey(curr_window, GLFW_KEY_LEFT) == GLFW_RELEASE)
		{
			passInput.w = 0;
		}


		//if(mouse clicks on tile
		if (glfwGetKey(curr_window, GLFW_KEY_ENTER) == GLFW_PRESS)	
		{
					

			if (selectedTile == vec2(0))
			{
				Checker* targetCheck = getCheck(hoverTile);

					if (targetCheck->team == playerID)
					{
						selectedTile = hoverTile;

						displayAvailableMoves();
					}
			
			}
			else
			{
				for (int i = 0; i < availableMoves.size(); i++)
				{
					if (availableMoves[i] == hoverTile)
					{
						TurnData turnData;

						turnData.t1x = selectedTile.x;
						turnData.t1y = selectedTile.y;
						turnData.t2x = hoverTile.x;
						turnData.t2y = hoverTile.y;

						sendTurn(turnData);
						applyTurn(turnData);

						resetVisualisation();
					}
				}
			
			}






		}
	}
}

Checker* Draughts::getCheck(vec2 gridPos)
{
	gridPos.x--;
	gridPos.y--;

	for (int i = 0; i < checkers.size(); i++)
	{
		if (checkers[i]->gridPos == gridPos)
		{
			return checkers[i];
		}
	}

	Checker* tempCheck=new Checker(vec2(0),vec4(0),0);

	return tempCheck;
}

void Draughts::resetVisualisation()
{
	hoverTile = vec2(5);
	selectedTile =vec2(0);
	availableMoves = std::vector<vec2>();
}

void Draughts::displayAvailableMoves()
{
	Checker* targetCheck = getCheck(selectedTile);
	availableMoves = targetCheck->availableMoves;
}

void Draughts::calculateAvailableMoves()
{
	if (playerTurn == 1)
	{
		playerTurn = 2;
	}
	else
	{
		playerTurn = 1;
	}

	//resettingMoves
	for (int i = 0; i < checkers.size(); i++)
	{
		checkers[i]->availableMoves = std::vector<vec2>();
	}

	bool captureMode=false;

	//checking for capture Options
	for (int i = 0; i < checkers.size(); i++)
	{
		if (checkers[i]->team == playerTurn)
		{
			if (calculateCheckAvailableMoves(checkers[i], false) == true)
			{
				captureMode = true;
				break;
			}
		}
	}

	if (captureMode == true)
	{
		//resettingMoves
		for (int i = 0; i < checkers.size(); i++)
		{
			checkers[i]->availableMoves = std::vector<vec2>();
		}

		for (int i = 0; i < checkers.size(); i++)
		{
			if (checkers[i]->team == playerTurn)
			{
				calculateCheckAvailableMoves(checkers[i], true);
			}
		}
	}
}

bool Draughts::rigAvailableMoves(vec2 target)
{

	for (int i = 0; i < checkers.size(); i++)
	{
		checkers[i]->availableMoves = std::vector<vec2>();
	}


	Checker* targetCheck = getCheck(target);

	return calculateCheckAvailableMoves(targetCheck,false);


}

void Draughts::removeCheckAtPoint(vec2 target)
{
	Checker* targetChecker = getCheck(target);

	for (int i = 0; i < checkers.size(); i++)
	{
		if (targetChecker == checkers[i])
		{
			checkers.erase(checkers.begin() + i);
		}
	}
}

bool Draughts::calculateCheckAvailableMoves(Checker* check,bool onlyCaptures)
{
	bool returnTarget = false;

	int teamTarget = 2;

	if (check->team == 2)
	{
		teamTarget = 1;
	}


	if (check->team == 1 | check->isKing==true)
	{
		if (getCheck(vec2((check->gridPos.x+2),(check->gridPos.y+2)))->team == 0 &&onlyCaptures==false)
		{
			if (check->gridPos.x + 1 < 8 && check->gridPos.y + 1 < 8)
			{
				check->availableMoves.push_back(vec2((check->gridPos.x + 2), (check->gridPos.y + 2)));
			}			
		}
		else if (getCheck(vec2((check->gridPos.x + 2), (check->gridPos.y + 2)))->team == teamTarget &&
			getCheck(vec2((check->gridPos.x + 3), (check->gridPos.y + 3)))->team == 0)
		{
			if (check->gridPos.x + 2 < 8 && check->gridPos.y + 2 < 8)
			{
				check->availableMoves.push_back(vec2((check->gridPos.x + 3), (check->gridPos.y + 3)));
				returnTarget = true;
			}
		}

		if (getCheck(vec2((check->gridPos.x - 0), (check->gridPos.y + 2)))->team == 0 && onlyCaptures == false)
		{
			if (check->gridPos.x-1>=0 && check->gridPos.y + 1 < 8)
			{
				check->availableMoves.push_back(vec2((check->gridPos.x - 0), (check->gridPos.y + 2)));
			}

		}
		else if (getCheck(vec2((check->gridPos.x - 0), (check->gridPos.y + 2)))->team == teamTarget &&
			getCheck(vec2((check->gridPos.x - 1), (check->gridPos.y + 3)))->team == 0)
		{
			if (check->gridPos.x - 2 >=0 && check->gridPos.y + 2 < 8)
			{
				check->availableMoves.push_back(vec2((check->gridPos.x -1), (check->gridPos.y + 3)));
				returnTarget = true;
			}
		}
	}
	if (check->team == 2 | check->isKing == true)
	{



		if (getCheck(vec2((check->gridPos.x + 2), (check->gridPos.y + 0)))->team == 0 && onlyCaptures == false)
		{
			if (check->gridPos.x + 1 < 8 && check->gridPos.y -1 >=0)
			{
				check->availableMoves.push_back(vec2((check->gridPos.x + 2), (check->gridPos.y + 0)));
			}
		}


		else if (getCheck(vec2((check->gridPos.x + 2), (check->gridPos.y + 0)))->team == teamTarget &&
			getCheck(vec2((check->gridPos.x + 3), (check->gridPos.y -1)))->team == 0)
		{
			if (check->gridPos.x +2 < 8 && check->gridPos.y -2 >= 0)
			{
				check->availableMoves.push_back(vec2((check->gridPos.x + 3), (check->gridPos.y -1)));
				returnTarget = true;
			}
		}

		if (getCheck(vec2((check->gridPos.x - 0), (check->gridPos.y - 0)))->team == 0 && onlyCaptures == false)
		{
			if (check->gridPos.x - 1 >= 0 && check->gridPos.y -1 >=0)
			{
				check->availableMoves.push_back(vec2((check->gridPos.x - 0), (check->gridPos.y -0)));
			}

		}
		else if (getCheck(vec2((check->gridPos.x - 0), (check->gridPos.y - 0)))->team == teamTarget &&
			getCheck(vec2((check->gridPos.x - 1), (check->gridPos.y - 1)))->team == 0)
		{
			if (check->gridPos.x -2 >=0 && check->gridPos.y -2 >=0)
			{
				check->availableMoves.push_back(vec2((check->gridPos.x - 1), (check->gridPos.y - 1)));
				returnTarget = true;
			}
		}

	}

	return returnTarget;
}