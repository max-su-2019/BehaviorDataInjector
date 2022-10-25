#pragma once

namespace BDI
{
	class CreateBehaviorGraphHook
	{
	public:
		static void InstallHook()
		{
			SKSE::AllocTrampoline(1 << 4);

			REL::Relocation<std::uintptr_t> CreateBehaviorGraphBase{ REL::ID(62640) };
			auto& trampoline = SKSE::GetTrampoline();
			_CreateBehaviorGraph = trampoline.write_call<5>(CreateBehaviorGraphBase.address() + 0x108, CreateBehaviorGraph);

			INFO("{} Done!", __FUNCTION__);
		}

	private:
		static RE::hkbBehaviorGraph* CreateBehaviorGraph(const char* a_projectPath, const char* a_fileName, RE::hkbCharacter* a_character, RE::BSResource::ID* a_fileID, const RE::BSFixedString& a_projectName, void* a6);

		static inline REL::Relocation<decltype(CreateBehaviorGraph)> _CreateBehaviorGraph;
	};
}
