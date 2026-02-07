#include "JSONReader.hpp"

#include <exception>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

std::string readFile(const std::string fileName) {
	std::fstream file(fileName.c_str());
	std::string content;

	std::getline(file, content, '\0');
	return (content);
}

void print_value(JSONReader &value) {
	if (value.isNumber())
		std::cerr << value.toDouble();
	else if (value.isBool()) {
		if (value.toBool())
			std::cerr << "true";
		else
			std::cerr << "false";
	} else if (value.isNull())
		std::cerr << "null";
	else
		std::cerr << "\"" << value.toString() << "\"";
}

void print_json(JSONReader &reader, int depth = 0) {
	if (reader.isDict()) {
		std::cerr << "{";
		std::vector<std::string> keys = reader.keys();
		for (std::vector<std::string>::iterator key = keys.begin(); key != keys.end(); key++) {
			std::cerr << "\"" << *key << "\":";
			JSONReader value = reader[*key];
			if (value.isArray() || value.isDict())
				print_json(value, depth + 1);
			else
				print_value(value);
			if (key != keys.end() - 1)
				std::cerr << ",";
		}
		std::cerr << "}";
	} else if (reader.isArray()) {
		std::cerr << "[";
		std::vector<JSONReader> values = reader.values();
		for (std::vector<JSONReader>::iterator value = values.begin(); value != values.end(); value++) {
			if (value->isArray() || value->isDict())
				print_json(*value, depth + 1);
			else
				print_value(*value);
			if (value != values.end() - 1)
				std::cerr << ",";
		}
		std::cerr << "]";
	}
	else {
		std::cerr << "Invalid type" << std::endl;
	}
	if (depth == 0)
		std::cerr << std::endl;
}

int	main(void) {
	std::string content = readFile("example.json");
	try{
		JSONReader reader(content);

		std::cout << reader["e"]["g"] << std::endl;
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
	return (0);
}