#pragma once
#include "BDIObject.h"

namespace BDI
{
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

		bool istarts_with(std::string_view a_str1, std::string_view a_str2)
		{
			return std::ranges::starts_with(a_str1, a_str2, [=](char ch1, char ch2) {
				return std::toupper(ch1) == std::toupper(ch2);
			});
		}

	public:
		[[nodiscard]] auto get_subpaths(std::string_view a_fullPath) noexcept
		{
			return objMap | std::views::filter([=](auto& pair) { return istarts_with(a_fullPath, pair.first); }) | std::views::transform([](auto& pair) -> auto& { return pair.second; });
		}

		std::unordered_map<std::string, BDIObjArray> objMap;
	};

}