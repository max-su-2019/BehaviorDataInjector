#include "DataHandler.h"

namespace BDI
{
	inline std::vector<std::string> GetAllFiles(std::string_view a_path, std::string_view a_ext = {}, std::string_view a_prefix = {}, std::string_view a_suffix = {}, bool a_recursive = false) noexcept
	{
		std::vector<std::string> files;
		auto file_iterator = [&](const std::filesystem::directory_entry& a_file) {
			if (a_file.exists() &&
				!a_file.path().empty()) {
				if (!a_ext.empty() && a_file.path().extension() != a_ext) {
					return;
				}

				const auto path = a_file.path().string();

				if (!a_prefix.empty() && path.find(a_prefix) != std::string::npos) {
					files.push_back(path);
				} else if (!a_suffix.empty() && path.rfind(a_suffix) != std::string::npos) {
					files.push_back(path);
				} else if (a_prefix.empty() && a_suffix.empty()) {
					files.push_back(path);
				}
			}
		};

		std::string dir(MAX_PATH + 1, ' ');
		auto res = GetModuleFileNameA(nullptr, dir.data(), MAX_PATH + 1);
		if (res == 0) {
			ERROR("DKU_C: Unable to acquire valid path using default null path argument!\nExpected: Current directory\nResolved: NULL");
		}

		auto eol = dir.find_last_of("\\/");
		dir = dir.substr(0, eol);

		auto path = a_path.empty() ? std::filesystem::path{ dir } : std::filesystem::path{ a_path };
		if (!is_directory(path.parent_path())) {
			path = dir / path;
		}

		[[unlikely]] if (a_recursive)
		{
			std::ranges::for_each(std::filesystem::recursive_directory_iterator(path), file_iterator);
		}
		else
		{
			std::ranges::for_each(std::filesystem::directory_iterator(path), file_iterator);
		}

		std::ranges::sort(files);

		return files;
	}

	DataHandler::DataHandler()
	{
		try {
			auto BDIFilesName = GetAllFiles("Data\\SKSE\\Plugins\\BehaviorDataInjector"sv, ".json"sv, {}, "_BDI"sv);
			for (const auto& fileName : BDIFilesName) {
				auto js = json::parse(std::ifstream(fileName));
				for (auto& element : js) {
					auto obj = element.get<BDIObject>();
					if (obj.projectNames.empty())
						genericObjArr.AddObject(obj);
					else {
						for (auto name : obj.projectNames) {
							objMap[name].AddObject(obj);
						}
					}
				}
			}
		} catch (const json::exception& ex) {
			ERROR("{}", ex.what());
		}
	}

	void DataHandler::BDIObjArray::AddObject(const BDIObject& a_obj)
	{
		switch (a_obj.type) {
		case BDIDataTypes::kBool:
		case BDIDataTypes::kInt:
		case BDIDataTypes::kFloat:
			Objects[Type::kVariable].push_back(std::make_shared<BDIObject>(a_obj));
			break;

		case BDIDataTypes::kEvent:
			Objects[Type::kEvent].push_back(std::make_shared<BDIObject>(a_obj));
			break;

		default:
			break;
		}
	}

	void DataHandler::BDIObjArray::InjectVariables(RE::hkbBehaviorGraph* a_graph)
	{
		static auto ContainsVar = [](RE::hkbBehaviorGraph* a_graph, const std::string& a_string) -> bool {
			if (a_graph && a_graph->data && a_graph->data->stringData) {
				for (auto const& varName : a_graph->data->stringData->variableNames) {
					if (_strcmpi(varName.c_str(), a_string.c_str()) == 0)
						return true;
				}
			}

			return false;
		};

		if (a_graph && a_graph->data) {
			for (auto const& obj : Objects[Type::kVariable]) {
				if (obj && !ContainsVar(a_graph, obj->name)) {
					switch (obj->type) {
					case BDIDataTypes::kBool:
						a_graph->data->AddBoolVariable(RE::hkStringPtr::Create(obj->name), obj->value.b);
						break;

					case BDIDataTypes::kInt:
						a_graph->data->AddIntVariable(RE::hkStringPtr::Create(obj->name), obj->value.i);
						break;

					case BDIDataTypes::kFloat:
						a_graph->data->AddFloatVariable(RE::hkStringPtr::Create(obj->name), obj->value.f);
						break;

					default:
						break;
					}
				}
			}
		}
	}

}