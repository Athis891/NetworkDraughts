#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"


struct ClientData
{
	const unsigned short PORT = 3327;
	RakNet::RakPeerInterface* pPeerInterface;
	const char* IP = "127.0.0.1";

};