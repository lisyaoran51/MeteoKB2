#ifndef RULESET_STORE_H
#define RULESET_STORE_H

#include "../../Framework/Database/DatabaseStore.h"


using namespace Framework::Database;


namespace Games {
namespace Rulesets{

	class RulesetStore : public DatabaseStore {

	public:

		RulesetStore(function<DatabaseContext*(void)> getContext, Storage* s = nullptr);

	};

}}


#endif