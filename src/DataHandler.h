#pragma once
#include "BDIObject.h"

namespace BDI
{
	using namespace DKUtil::Alias;

	class DataHandler : public dku::model::Singleton<DataHandler>
	{
		friend dku::model::Singleton<DataHandler>;

		struct BDIObjArray
		{
			enum Type
			{
				kVariable = 0,
				kEvent,
				kTotal
			};

			void InjectVariables(RE::hkbBehaviorGraph* a_graph);
			void InjectEvents(RE::hkbBehaviorGraph* a_graph);

			friend class DataHandler;

		private:
			void AddObject(const BDIObject& a_obj);

			std::vector<std::shared_ptr<BDIObject>> Objects[Type::kTotal];
		};

		DataHandler();

	public:
		[[nodiscard]] auto get_subpaths(std::string_view a_fullPath) noexcept
		{
			return objMap | std::views::filter([=](auto& pair) { return DKUtil::string::istarts_with(a_fullPath, pair.first); }) | std::views::transform([](auto& pair) -> auto& { return pair.second; });
		}

		std::unordered_map<std::string, BDIObjArray> objMap;

		Boolean enableDebugLog{ "EnableDebugLog", "Debug" };
	};

}