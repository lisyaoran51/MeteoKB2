#include "ResourceStoreWorkingSm.h"


using namespace Games::Sheetmusics;
using namespace Games::Sheetmusics::Format;



ResourceStoreWorkingSm::ResourceStoreWorkingSm(ResourceStore<char*>* rStore, SmInfo * sInfo): WorkingSm(sInfo)
{
	resourceStore = rStore;
}

Sm<Event>* ResourceStoreWorkingSm::GetSm()
{
	// resource store�٨S�g�n�A�����ª���kŪsm

	return WorkingSm::GetSm();

	// �s����k�H��A��
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
	// resource store�٨S�g�n�A�����ª���kŪsm

	return WorkingSm::getPathForFile(fileName);

	// �o�����ӭn�hstorage�����S���H
	return fileName;
}
