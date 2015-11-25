#pragma once

#include "BaseApplication.h"
#include <vector>
#include <glm\vec3.hpp>
#include <iostream>
#include <string>

#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"

#include "GameMessages.h"


#include "Gizmos.h"

#include "GLFW\glfw3.h"

#include "Camera.h"

namespace RakNet
{
	class RakPeerInterface;
}

class BasicNetworkingApplication : public BaseApplication
{
public:
	BasicNetworkingApplication();
	~BasicNetworkingApplication();

	//Initialize the connection
	void handleNetworkConnection();
	void initialiseClientConnection();
	//Handle incoming packets
	void handleNetworkMessages();

	virtual bool startup();

	virtual void shutdown();

	virtual bool update(float deltaTime);

	virtual void draw();

	void readObjectDataFromServer(RakNet::BitStream& bsIn);
	void createGameObject();
	void moveClientObject(float deltaTime);
	//void sendUpdatedObjectPositionToServer(GameObject clientObject);

	RakNet::RakPeerInterface* m_pPeerInterface;
	const char* IP = "127.0.0.1";
	const unsigned short PORT = 3327;	unsigned int m_uiClientId;	Camera* m_camera;

private:
};