#include "GattServer.h"

#include "../../../Util/Log.h"

using namespace Desktop::Devices::Gatt;
using namespace Util;



GattClient * GattServer::Listen()
{
	if (gattListener != nullptr) {
		LOG(LogLevel::Error) << "int GattServer::Listen() : already has a listener.";
		delete gattListener;
		gattListener = nullptr;
	}

	gattListener = createListener();

	GattClient* client = accept();

	delete gattListener;
	gattListener = nullptr;

	return client;
}

int GattServer::Run(GattClient * gClient)
{

	unique_lock<mutex> guard(m_mutex);
	m_client = gClient;
	guard.unlock();

	m_client->Run();
	
	guard.lock();
	delete m_client;
	m_client = nullptr;

	return 0;
}
