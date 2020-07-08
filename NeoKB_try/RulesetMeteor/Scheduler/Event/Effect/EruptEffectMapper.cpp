#include "EruptEffectMapper.h"



using namespace Meteor::Schedulers::Events::Effects;



EruptEffectMapper::EruptEffectMapper()
{
	throw logic_error("EruptEffectMapper() : This constructor is only for compile-time assurance. Not available to execute.");
}

EruptEffectMapper::EruptEffectMapper(int w, int h) : EffectMapper<EruptEffect>(w, h)
{
}
