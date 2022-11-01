#include "LoadGame.h"
#include "ConsoleCommands.h"
#include "DataHandler.h"
#include "Hook.h"

namespace BDI
{
	void EventCallback(SKSE::MessagingInterface::Message* msg)
	{
		if (msg->type == SKSE::MessagingInterface::kPostLoad) {
			auto dataHandler = DataHandler::GetSingleton();
			if (dataHandler->enableDebugLog.get_data()) {
				spdlog::set_level(spdlog::level::debug);
				DEBUG("Enable Debug Log!");
			}

			ConsoleCommands::Register();
			CreateSymbolIdMapHook::InstallHook();
		}
	}

}
