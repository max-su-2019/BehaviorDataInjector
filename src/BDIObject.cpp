#include "BDIObject.h"

namespace BDI
{
	void from_json(const json& j, BDIObject& a_data)
	{
		j.at("projectPath").get_to(a_data.projectPath);

		auto typeName = j.at("type").get<std::string>();
		auto enumType = BDITypeEnumTbl.from_string(typeName, false);
		if (enumType.has_value()) {
			a_data.type = enumType.value();
		} else {
			ERROR("Invaild Data Type: {}", typeName);
			return;
		}

		j.at("name").get_to(a_data.name);

		switch (a_data.type) {
		case BDIDataTypes::kBool:
			j.at("value").get_to(a_data.value.b);
			break;

		case BDIDataTypes::kInt:
			j.at("value").get_to(a_data.value.i);
			break;

		case BDIDataTypes::kFloat:
			j.at("value").get_to(a_data.value.f);
			break;

		default:
			break;
		}
	}
}
