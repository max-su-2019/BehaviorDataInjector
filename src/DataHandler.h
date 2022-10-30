#pragma once
#include "BDIObject.h"

namespace BDI
{
	class DataHandler
	{
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

	public:
		static DataHandler* GetSingleton()
		{
			static DataHandler singleton;
			return std::addressof(singleton);
		}

		std::map<std::string, BDIObjArray> objMap;

	private:
		DataHandler();

		~DataHandler() = default;

		DataHandler(const DataHandler&) = delete;

		DataHandler(DataHandler&&) = delete;

		DataHandler& operator=(const DataHandler&) = delete;

		DataHandler& operator=(DataHandler&&) = delete;
	};

}