#include "Hook.h"
#include "DataHandler.h"

namespace BDI
{
	RE::hkbBehaviorGraph* CreateBehaviorGraphHook::CreateBehaviorGraph(const char* a_projectPath, const char* a_fileName, RE::hkbCharacter* a_character, RE::BSResource::ID* a_fileID, const RE::BSFixedString& a_projectName, void* a6)
	{
		DEBUG("{} Fired!", __FUNCTION__);

		auto graph = _CreateBehaviorGraph(a_projectPath, a_fileName, a_character, a_fileID, a_projectName, a6);
		if (graph && a_character) {
			auto handler = DataHandler::GetSingleton();
			handler->genericObjArr.InjectVariables(graph);

			auto it = handler->objMap.find(a_projectName.c_str());
			if (it != handler->objMap.end())
				it->second.InjectVariables(graph);
		}

		return graph;
	}
}