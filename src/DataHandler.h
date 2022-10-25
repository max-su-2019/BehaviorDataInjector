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

			void AddObject(const BDIObject& a_obj);
			void InjectVariables(RE::hkbBehaviorGraph* a_graph);

		private:
			std::vector<std::shared_ptr<BDIObject>> Objects[Type::kTotal];
		};

	public:
		static DataHandler* GetSingleton()
		{
			static DataHandler singleton;
			return std::addressof(singleton);
		}

		BDIObjArray genericObjArr;
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