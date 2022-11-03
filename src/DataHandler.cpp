#include "DataHandler.h"

namespace BDI
{
	static auto MainConfig = COMPILE_PROXY("BehaviorDataInjector.ini"sv);

	DataHandler::DataHandler()
	{
		MainConfig.Bind(enableDebugLog, false);
		MainConfig.Load();

		auto BDIFilesName = DKUtil::Config::GetAllFiles("Data\\SKSE\\Plugins\\BehaviorDataInjector"sv, ".json"sv, {}, "_BDI"sv);
		for (const auto& fileName : BDIFilesName) {
			INFO("Parsing BDI Data In \"{}\"", fileName);
			try {
				auto js = json::parse(std::ifstream(fileName));
				for (auto& element : js) {
					auto obj = element.get<BDIObject>();
					std::ranges::for_each(obj.projectPath, [](char& c) { c = std::toupper(c); });
					objMap[obj.projectPath].AddObject(obj);
				}
			} catch (const json::exception& ex) {
				ERROR("Caught expection when parsing file \"{}\":\n {}", fileName, ex.what());
			}
		}
	}

	void DataHandler::BDIObjArray::AddObject(const BDIObject& a_obj)
	{
		switch (a_obj.type) {
		case BDIDataTypes::kBool:
		case BDIDataTypes::kInt:
		case BDIDataTypes::kFloat:
			Objects[Type::kVariable].push_back(std::make_shared<BDIObject>(a_obj));
			INFO("Stored a BDI variable: name: \"{}\", type: \"{}\", projectPath: \"{}\".",
				a_obj.name, BDITypeEnumTbl.to_string(a_obj.type), a_obj.projectPath);
			break;

		case BDIDataTypes::kEvent:
			Objects[Type::kEvent].push_back(std::make_shared<BDIObject>(a_obj));
			INFO("Stored a BDI event: name: \"{}\", type: \"{}\", projectPath: \"{}\".",
				a_obj.name, BDITypeEnumTbl.to_string(a_obj.type), a_obj.projectPath);
			break;

		default:
			break;
		}
	}

	void DataHandler::BDIObjArray::InjectVariables(RE::hkbBehaviorGraph* a_graph)
	{
		if (a_graph && a_graph->data && a_graph->data->stringData && a_graph->data->stringData->variableNames.data()) {
			for (auto const& obj : Objects[Type::kVariable]) {
				if (obj) {
					int result = -1;
					switch (obj->type) {
					case BDIDataTypes::kBool:
						result = a_graph->data->AddBoolVariable(RE::hkStringPtr::Create(obj->name), obj->value.b);
						break;

					case BDIDataTypes::kInt:
						result = a_graph->data->AddIntVariable(RE::hkStringPtr::Create(obj->name), obj->value.i);
						break;

					case BDIDataTypes::kFloat:
						result = a_graph->data->AddFloatVariable(RE::hkStringPtr::Create(obj->name), obj->value.f);
						break;

					default:
						break;
					}

					if (result >= 0) {
						DEBUG("Injected a variable \"{}\" to graph \"{}\" successfully", obj->name, a_graph->name.c_str());
					} else {
						WARN("Fail to Inject a variable \"{}\" to graph \"{}\"", obj->name, a_graph->name.c_str());
					}
				}
			}
		}
	}

	void DataHandler::BDIObjArray::InjectEvents(RE::hkbBehaviorGraph* a_graph)
	{
		if (a_graph && a_graph->data && a_graph->data->stringData && a_graph->data->stringData->eventNames.data()) {
			for (auto const& obj : Objects[Type::kEvent]) {
				if (obj)
					if (a_graph->data->AddEvent(RE::hkStringPtr::Create(obj->name)) >= 0) {
						DEBUG("Injected a event \"{}\" to graph \"{}\" successfully", obj->name, a_graph->name.c_str());
					} else {
						WARN("Fail to Inject a event \"{}\" to graph \"{}\"", obj->name, a_graph->name.c_str());
					}
			}
		}
	}

}