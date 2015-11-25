#include "BasicNetworking.h"


/*





void BasicNetworkingApplication::handleNetworkMessages()
{
	RakNet::Packet* packet;
	for (packet = m_pPeerInterface->Receive(); packet;
		m_pPeerInterface->DeallocatePacket(packet),
		packet = m_pPeerInterface->Receive())
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
		case ID_SERVER_FULL_OBJECT_DATA:
		{
										   RakNet::BitStream bsIn(packet->data, packet->length, false);
										   bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
										   readObjectDataFromServer(bsIn);
										   break;
		}

		case ID_SERVER_CLIENT_ID:
		{
									RakNet::BitStream bsIn(packet->data, packet->length, false);
									bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
									bsIn.Read(m_uiClientId);
									std::cout << "Server has given us an id of: " << m_uiClientId << std::endl;

									createGameObject();
									break;
		}

		default:
			std::cout << "Received a message with a unknown id: " << packet->data[0];
			break;
		}
	}
}


bool BasicNetworkingApplication::startup()
{
	//Setup the basic window
	m_camera = new Camera(0, 0, 0, 0);
	createWindow("Client Application", 1280, 720);
	handleNetworkConnection();
	Gizmos::create();

	return true;
}

void BasicNetworkingApplication::shutdown()
{
	Gizmos::create();
}

bool BasicNetworkingApplication::update(float deltaTime)
{
	handleNetworkMessages();

	moveClientObject(deltaTime);


	m_camera->update(deltaTime);

	return true;
}

void BasicNetworkingApplication::draw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Gizmos::clear();

	//define color values
	glm::vec4 white(1);
	glm::vec4 black(0, 0, 0, 1);
	glm::vec4 yellow(1, 1, 0, 1);
	glm::vec4 green(0, 1, 0, 1);
	glm::vec4 orange(1.0f, 0.7f, 0.5f, 1.0f);

	//Adds Line Grid
	for (int i = 0; i <= 20; i++)
	{
		Gizmos::addLine(glm::vec3(-10 + i, 0, -10), glm::vec3(-10 + i, 0, 10), i == 10 ? yellow : white);
		Gizmos::addLine(glm::vec3(-10, 0, -10 + i), glm::vec3(10, 0, -10 + i), i == 10 ? yellow : white);
	}

	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		GameObject& obj = m_gameObjects[i];
		Gizmos::addSphere(glm::vec3(obj.fXPos, 2, obj.fZPos),
			2, 32, 32, glm::vec4(obj.fRedColour, obj.fGreenColour,
			obj.fBlueColour, 1), nullptr);
	}
	Gizmos::draw(m_camera->getProjectionView());



}

void BasicNetworkingApplication::readObjectDataFromServer(RakNet::BitStream& bsIn)
{
	//Create a temp object that we will pull all the object data into
	GameObject tempGameObject;
	//Read in the object data
	bsIn.Read(tempGameObject.uiOwnerClientID);
	bsIn.Read(tempGameObject.uiObjectID);
	bsIn.Read(tempGameObject.fXPos);
	bsIn.Read(tempGameObject.fZPos);
	bsIn.Read(tempGameObject.fRedColour);
	bsIn.Read(tempGameObject.fGreenColour);
	bsIn.Read(tempGameObject.fBlueColour);

	//Check to see whether or not this object is already stored in our local object list

	bool bFound = false;
	for (int i = 0; i < m_gameObjects.size(); i++)
	{
		if (m_gameObjects[i].uiObjectID == tempGameObject.uiObjectID)
		{
			bFound = true;
			//Update the game object
			GameObject& obj = m_gameObjects[i];
			obj.fXPos = tempGameObject.fXPos;
			obj.fZPos = tempGameObject.fZPos;
			obj.fRedColour = tempGameObject.fRedColour;
			obj.fGreenColour = tempGameObject.fGreenColour;
			obj.fBlueColour = tempGameObject.fBlueColour;
			obj.uiOwnerClientID = tempGameObject.uiOwnerClientID;
		}
	}
	//If we didn't find it, then it is a new object - add it to our object list
	if (!bFound)
	{
		m_gameObjects.push_back(tempGameObject);
		
		if (tempGameObject.uiOwnerClientID == m_uiClientId)
		{
			m_uiclientObjectIndex = m_gameObjects.size() - 1;
		}
		
	}
}

void BasicNetworkingApplication::createGameObject()
{
	//Tell the server we want to create a new game object that will represent us
	RakNet::BitStream bsOut;
	GameObject tempGameObject;
	tempGameObject.fXPos = 0.0f;
	tempGameObject.fZPos = 0.0f;
	tempGameObject.fRedColour = m_color.r;
	tempGameObject.fGreenColour = m_color.g;
	tempGameObject.fBlueColour = m_color.b;

	//Ensure that the write order is the same as the read order on the server!
	bsOut.Write((RakNet::MessageID)GameMessages::ID_CLIENT_CREATE_OBJECT);
	bsOut.Write(tempGameObject.fXPos);
	bsOut.Write(tempGameObject.fZPos);
	bsOut.Write(tempGameObject.fRedColour);
	bsOut.Write(tempGameObject.fGreenColour);
	bsOut.Write(tempGameObject.fBlueColour);
	m_pPeerInterface->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0,
		RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);

}


*/