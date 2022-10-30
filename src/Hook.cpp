#include "Hook.h"
#include "DataHandler.h"

namespace BDI
{

	hkbSymbolIdMap* CreateSymbolIdMapHook::CreateSymbolIdMap(RE::hkbBehaviorGraph* a_masterGraph, const RE::BSScrapArray<RE::hkbBehaviorGraph*>& a_graphArr, const char* a_projectPath, RE::hkbCharacter* a_character, hkbSymbolLinker* eventLinker, hkbSymbolLinker* variableLinker)
	{
		DEBUG("{} Fired!", __FUNCTION__);

		if (a_masterGraph && a_character && a_masterGraph->data) {
			const std::string path = a_projectPath;
			auto dataHandler = DataHandler::GetSingleton();
			for (auto pair : dataHandler->objMap) {
				if (path.starts_with(pair.first)) {
					pair.second.InjectVariables(a_masterGraph);

					for (auto graph : a_graphArr) {
						if (graph)
							pair.second.InjectEvents(graph);
					}
				}
			}
		}
		return _CreateSymbolIdMap(a_masterGraph, a_graphArr, a_projectPath, a_character, eventLinker, variableLinker);
	}
}