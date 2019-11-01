#include "ResourceStoreWorkingSm.h"


using namespace Games::Sheetmusics;
using namespace Games::Sheetmusics::Format;



ResourceStoreWorkingSm::ResourceStoreWorkingSm(ResourceStore<char*>* rStore, SmInfo * sInfo): WorkingSm(sInfo)
{
	resourceStore = rStore;
}

Sm<Event>* ResourceStoreWorkingSm::GetSm()
{
	ifstream* stream = resourceStore->GetStream(getPathForFile(smInfo->fileName));

	SmDecoder* smDecoder = SmDecoder::GetDecoder(stream);

	Sm<Event>* s = smDecoder->Decode(stream);

	s->SetSmInfo(smInfo);

	stream->close();
	delete stream;

	return s;
}

string ResourceStoreWorkingSm::getPathForFile(string fileName)
{
	// 這邊應該要去storage茶有沒有？
	return fileName;
}
