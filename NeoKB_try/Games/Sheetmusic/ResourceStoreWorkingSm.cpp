#include "ResourceStoreWorkingSm.h"


using namespace Games::Sheetmusics;
using namespace Games::Sheetmusics::Format;



ResourceStoreWorkingSm::ResourceStoreWorkingSm(ResourceStore<char*>* rStore, SmInfo * sInfo): WorkingSm(sInfo)
{
	resourceStore = rStore;
}

Sm<Event>* ResourceStoreWorkingSm::GetSm()
{
	// resource store還沒寫好，先用舊的方法讀sm

	return WorkingSm::GetSm();

	// 新的方法以後再用
	fstream* stream = resourceStore->GetStream(getPathForFile(smInfo->fileName));



	SmDecoder* smDecoder = SmDecoder::GetDecoder(stream);

	Sm<Event>* s = smDecoder->Decode(stream);

	s->SetSmInfo(smInfo);

	stream->close();
	delete stream;

	return s;
}

string ResourceStoreWorkingSm::getPathForFile(string fileName)
{
	// resource store還沒寫好，先用舊的方法讀sm

	return WorkingSm::getPathForFile(fileName);

	// 這邊應該要去storage茶有沒有？
	return fileName;
}
