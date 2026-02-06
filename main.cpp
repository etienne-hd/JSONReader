#include "JSONReader.hpp"

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

int	main(void) {
	std::string content = readFile("example.json");
	JSONReader jsonReader(content);

	std::cout << jsonReader["e"]["g"]["h"].toString() << std::endl;
	std::cout << jsonReader["c"][1].toString() << std::endl;
	std::cout << jsonReader.get("b").get(1).toInt() << std::endl;
	std::cout << jsonReader["e"].get("i").toDouble() << std::endl;

	std::cout << "Getting all values of json[\"c\"]" << std::endl;
	std::vector<JSONReader> values = jsonReader["b"].values();
	for (std::vector<JSONReader>::iterator it = values.begin(); it != values.end(); it++) {
		try {
			std::cout << it->toInt() << std::endl;
		} catch (...) {
			std::cerr << "Cannot convert to int" << std::endl;
		}
	}

	std::cout << "Getting all keys of json" << std::endl;
	std::vector<std::string> keys = jsonReader.keys();
	for (std::vector<std::string>::iterator it = keys.begin(); it != keys.end(); it++) {
		std::cout << *it << std::endl;
	}
	return (0);
}