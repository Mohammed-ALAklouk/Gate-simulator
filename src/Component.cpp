#include "Component.h"

Component Component::BaseComponents[]{
		Component(Type::AND, "AND", { -1, -1 }, Pin()),
		Component(Type::OR, "OR", { -1, -1 }, Pin()),
		Component(Type::NOT, "NOT", { -1 }, Pin()),
		Component(Type::HIGH, "HIGH", {}, Pin(LogicLevel::HIGH)),
		Component(Type::LOW, "LOW", {}, Pin(LogicLevel::LOW))

};

LogicLevel Component::AndLookupTable[9] = {
	//		LOW				HIGH				UNDEFINED
	LogicLevel::LOW, LogicLevel::LOW,		LogicLevel::LOW, // LOW
	LogicLevel::LOW, LogicLevel::HIGH,		LogicLevel::UNDEFINED, // HIGH
	LogicLevel::LOW, LogicLevel::UNDEFINED,	LogicLevel::UNDEFINED // UNDEFINED
};

LogicLevel Component::OrLookupTable[9] = {
	//		LOW					HIGH				UNDEFINED
	LogicLevel::LOW,		LogicLevel::HIGH,	LogicLevel::UNDEFINED, // LOW
	LogicLevel::HIGH,		LogicLevel::HIGH,	LogicLevel::HIGH, // HIGH
	LogicLevel::UNDEFINED,	LogicLevel::HIGH,	LogicLevel::UNDEFINED // UNDEFINED
};

LogicLevel Component::NotLookupTable[3] = {
	LogicLevel::HIGH, // LOW
	LogicLevel::LOW,  // HIGH
	LogicLevel::UNDEFINED // UNDEFINED
};

LogicLevel Component::HighLookupTable[3] = {
	LogicLevel::HIGH, // LOW
	LogicLevel::HIGH,  // HIGH
	LogicLevel::HIGH // UNDEFINED
};

LogicLevel Component::LowLookupTable[3] = {
	LogicLevel::LOW, // LOW
	LogicLevel::LOW,  // HIGH
	LogicLevel::LOW // UNDEFINED
};

LogicLevel* Component::LookupTables[]{
	AndLookupTable,
	OrLookupTable,
	NotLookupTable,
	HighLookupTable,
	LowLookupTable
};