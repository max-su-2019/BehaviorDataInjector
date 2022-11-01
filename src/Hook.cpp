#include "Hook.h"
#include "DataHandler.h"

namespace BDI
{

	hkbSymbolIdMap* CreateSymbolIdMapHook::CreateSymbolIdMap(RE::hkbBehaviorGraph* a_masterGraph, const RE::BSScrapArray<RE::hkbBehaviorGraph*>& a_graphArr, const char* a_projectPath, RE::hkbCharacter* a_character, hkbSymbolLinker* eventLinker, hkbSymbolLinker* variableLinker)
	{
		DEBUG("{} Fired!", __FUNCTION__);

		if (a_masterGraph && a_character && a_masterGraph->data && !a_masterGraph->eventIDMap && !a_masterGraph->variableIDMap) {
			auto dataHandler = DataHandler::GetSingleton();
			auto objArr = dataHandler->get_subpaths(a_projectPath);
			if (!objArr.empty()) {
				DEBUG("Injecting Data for project: \"{}\" in path \"{}\"", a_masterGraph->name.c_str(), a_projectPath);
				for (auto& obj : objArr) {
					obj.InjectVariables(a_masterGraph);
					for (auto graph : a_graphArr) {
						if (graph)
							obj.InjectEvents(graph);
					}
				}
			}
		}
		return _CreateSymbolIdMap(a_masterGraph, a_graphArr, a_projectPath, a_character, eventLinker, variableLinker);
	}
}
