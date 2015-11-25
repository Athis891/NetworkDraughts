#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"


struct ServerData
{
	RakNet::SystemAddress client;
	RakNet::Packet* packet;
	RakNet::RakPeerInterface* pPeerInterface;
	const unsigned short PORT = 3327;

};