#include "SheetmusicConverter.h"


using namespace std;
using namespace Util;
using namespace Base::Sheetmusics;




SmConverter::SmConverter(PatternGenerator * pg)
{
	patternGenerator = pg;
}

/// <summary>
/// 在遊戲開始前附智一個新的sm，然後把裡面的特效events全部重新轉換一次
/// We always operate on a clone of the original sm, to not modify it game-wide
///	</summary>
Sm<Event>* SmConverter::Convert(Sm<Event>* s)
{
	
	LOG(LogLevel::Fine) << "SmConverter::Convert(Sm<Event>*) : Start converting sheetmusic...";
	
	LOG(LogLevel::Finer) << "Sm<Event>* SmConverter::Convert(Sm<Event>*) : Cloning sm ...";
	Sm<Event>* sm = new Sm<Event>(s);

	// 從sm讀出來的event
	LOG(LogLevel::Finer) << "Sm<Event>* SmConverter::Convert(Sm<Event>*) : Getting events ...";
	vector<Event*>* originalEvents = sm->GetEvents();

	// 經過處理轉換的event
	vector<Event*>* newEvents = new vector<Event*>();

	// 轉換中暫存用，指標指轉換出來的vector是純在pattern裡
	// 要額外生成一個vector來存暫存的原因是，convert event出來的event可能有的完全沒經過處理糾丟出來
	// 這個時候如果額外去生成一個vector來裝這一個event，就會不知道這個Vector要在什麼時候刪掉，因為如果是
	// pattern的話，會統一在clean pattern的時候刪掉，額外生的vector就沒辦法
	vector<Event*>* tempEvents = new vector<Event*>();

	for (int i = 0; i < originalEvents->size(); i++) {

		LOG(LogLevel::Finest) << "Sm<Event>* SmConverter::Convert(Sm<Event>*) : converting #" << i << " event ...";
		convertEvent(tempEvents, originalEvents->at(i));

		for (int j = 0; j < tempEvents->size(); j++) {
			newEvents->push_back( tempEvents->at(j) );
		}

		tempEvents->clear();
	
	}

	// TODO:擺到posy rocessor裡
	/* 這一段之後要擺到pot processor裡面，已後converter和post processor做的工作都delegate給pattern generator和其他子 */
	/* 元件做 */
	LOG(LogLevel::Finer) << "Sm<Event>* SmConverter::Convert(Sm<Event>*) : Creating other events ...";
	patternGenerator->CreateOtherEvent(tempEvents);
	
	for (int j = 0; j < tempEvents->size(); j++) {
		newEvents->push_back(tempEvents->at(j));
	}

	tempEvents->clear();

	LOG(LogLevel::Finer) << "Sm<Event>* SmConverter::Convert(Sm<Event>*) : Clean patterns ...";
	patternGenerator->CleanPatterns();

	sm->SetEvents(newEvents);
	delete tempEvents;

	return sm;
}

int SmConverter::convertEvent(vector<Event*>* es, Event * e)
{
	// 對本來單純的樂譜，生成各種特效，如提示光、落下、其他特效。
	Pattern* newPattern = patternGenerator->Generate(es, e);
	patternGenerator->Add(newPattern);



	return 0;
}
