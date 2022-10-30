#pragma once

namespace BDI
{
	class hkbSymbolIdMap;
	class hkbSymbolLinker;

	class CreateSymbolIdMapHook
	{
	public:
		static void InstallHook()
		{
			SKSE::AllocTrampoline(1 << 4);

			REL::Relocation<std::uintptr_t> CreateSymbolIdMapBase{ REL::ID(62640) };
			auto& trampoline = SKSE::GetTrampoline();
			_CreateSymbolIdMap = trampoline.write_call<5>(CreateSymbolIdMapBase.address() + 0x22A, CreateSymbolIdMap);

			INFO("{} Done!", __FUNCTION__);
		}

	private:
		static hkbSymbolIdMap* CreateSymbolIdMap(RE::hkbBehaviorGraph* a_masterGraph, const RE::BSScrapArray<RE::hkbBehaviorGraph*>& a_graphArr, const char* a_projectPath, RE::hkbCharacter* a_character, hkbSymbolLinker* eventLinker, hkbSymbolLinker* variableLinker);

		static inline REL::Relocation<decltype(CreateSymbolIdMap)> _CreateSymbolIdMap;
	};
}
