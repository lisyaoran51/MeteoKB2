#include "SheetmusicConverter.h"


using namespace std;
using namespace Util;
using namespace Games::Sheetmusics;




SmConverter::SmConverter(PatternGenerator * pg)
{
	patternGenerator = pg;
}

/// <summary>
/// �b�C���}�l�e�����@�ӷs��sm�A�M���̭����S��events�������s�ഫ�@��
/// We always operate on a clone of the original sm, to not modify it game-wide
///	</summary>
Sm<Event>* SmConverter::Convert(Sm<Event>* s)
{
	
	LOG(LogLevel::Fine) << "SmConverter::Convert(Sm<Event>*) : Start converting sheetmusic...";
	
	LOG(LogLevel::Finer) << "Sm<Event>* SmConverter::Convert(Sm<Event>*) : Cloning sm ...";
	Sm<Event>* sm = new Sm<Event>(s);

	patternGenerator->Initialize(sm);

	// �qsmŪ�X�Ӫ�event
	LOG(LogLevel::Finer) << "Sm<Event>* SmConverter::Convert(Sm<Event>*) : Getting events ...";
	vector<Event*>* originalEvents = sm->GetEvents();

	// �g�L�B�z�ഫ��event
	vector<Event*>* newEvents = new vector<Event*>();

	// �ഫ���Ȧs�ΡA���Ы��ഫ�X�Ӫ�vector�O�¦bpattern��
	// �n�B�~�ͦ��@��vector�Ӧs�Ȧs����]�O�Aconvert event�X�Ӫ�event�i�঳�������S�g�L�B�z�ȥ�X��
	// �o�ӮɭԦp�G�B�~�h�ͦ��@��vector�Ӹ˳o�@��event�A�N�|�����D�o��Vector�n�b����ɭԧR���A�]���p�G�O
	// pattern���ܡA�|�Τ@�bclean pattern���ɭԧR���A�B�~�ͪ�vector�N�S��k
	vector<Event*>* tempEvents = new vector<Event*>();

	for (int i = 0; i < originalEvents->size(); i++) {

		LOG(LogLevel::Finest) << "Sm<Event>* SmConverter::Convert(Sm<Event>*) : converting #" << i << " event ..." << originalEvents->at(i)->GetStartTime();
		convertEvent(tempEvents, originalEvents->at(i));

		for (int j = 0; j < tempEvents->size(); j++) {
			newEvents->push_back( tempEvents->at(j) );
		}
		newEvents->push_back(originalEvents->at(i)->Clone());

		tempEvents->clear();
	
	}

	// TODO:�\��posy rocessor��
	/* �o�@�q����n�\��pot processor�̭��A�w��converter�Mpost processor�����u�@��delegate��pattern generator�M��L�l */
	/* ���� */
	LOG(LogLevel::Finer) << "Sm<Event>* SmConverter::Convert(Sm<Event>*) : Creating other events ...";
	patternGenerator->CreateOtherEvent(newEvents);

	patternGenerator->PostProcess();

	LOG(LogLevel::Finer) << "Sm<Event>* SmConverter::Convert(Sm<Event>*) : Clean patterns ...";
	patternGenerator->CleanPatterns();

	sm->SetEvents(newEvents);
	delete tempEvents;

	return sm;
}

int SmConverter::convertEvent(vector<Event*>* es, Event * e)
{
	// �糧�ӳ�ª����СA�ͦ��U�دS�ġA�p���ܥ��B���U�B��L�S�ġC
	Pattern* newPattern = patternGenerator->Generate(es, e);
	patternGenerator->Add(newPattern);



	return 0;
}
