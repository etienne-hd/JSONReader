#include "JSONReader.hpp"

#include <stdexcept>
#include <string>
#include <cstdlib>
#include <vector>

JSONReader::JSONReader(std::string content, std::string content_base) {
	_content = content;
	_base_content = content_base;
}

JSONReader::JSONReader(std::string content) {
	//if (!JSONReader::isValidJSON(content))
	//	throw std::runtime_error("Invalid JSON file");

	for (std::string::iterator it = content.begin(); it != content.end(); it++) {
		if ((*it >= 8 && *it <= 13) || *it == ' ' || *it == '\n')
			content.erase(it--);
	}
	
	_content = content;
	_base_content = content;
}

//bool JSONReader::isValidJSON(std::string content) {
//	...
//}

JSONReader JSONReader::getValue(std::string::iterator &it) {
	std::string value;
	bool inScope = false;

	// Array / Dict
	if (*it == '{' || *it == '[') {
		char openChar = *it == '{' ? '{' : '[';
		char closeChar = *it == '{' ? '}' : ']';
		int depth = 0;
		while (it != _content.end() - 1)
		{
			if (*it == '"' && *(it - 1) != '\\')
				inScope = !inScope;
			else if (inScope == false && *it == openChar)
				depth++;
			else if (inScope == false && *it == closeChar)
				depth--;
			value += *it;
			it++;
			if (depth == 0)
				break;
		}
	}
	// Basic (e.g: "a": 1)
	else {
		while (!(*it == ',' && inScope == false) && it != _content.end() - 1) {
			if (*it == '"' && *(it - 1) != '\\')
				inScope = !inScope;
			value += *it;
			it++;
		}
	}

	return (JSONReader(value, _content));
}

std::string JSONReader::getKey(std::string::iterator &it) {
	std::string key;
	bool inScope = false;

	while (!(*it == ':' && inScope == false) && it != _content.end() - 1) {
		if (*it == '"' && *(it - 1) != '\\')
			inScope = !inScope;
		else
			key += *it;
		it++;
	}
	return (key);
}

JSONReader JSONReader::get(std::string key) {
	if (!(*_content.begin() == '{' && *(_content.end() - 1) == '}'))
		throw std::runtime_error("Invalid Content");

	std::string::iterator it = _content.begin() + 1;

	while (it != _content.end()) {
		std::string currentKey = this->getKey(it);
		it++; // skip ':'

		JSONReader currentValue = this->getValue(it);
		it++; // skip ','
		
		if (key == currentKey)
			return (currentValue);
	}
	throw std::runtime_error("Unable to find key");
}

JSONReader JSONReader::get(int index) {
	return(this->values(index + 1)[index]);
}

JSONReader JSONReader::operator[](int index) {
	return(this->get(index));
}

JSONReader JSONReader::operator[](std::string key) {
	return(this->get(key));
}

std::vector<JSONReader> JSONReader::values(int n) {
	std::vector<JSONReader> values;

	if (!(*_content.begin() == '[' && *(_content.end() - 1) == ']'))
		throw std::runtime_error("JSONReader::values is reserved for arrays.");

	std::string::iterator it = _content.begin() + 1;
	
	int currentIndex = 0;
	while (it != _content.end() && (currentIndex < n || n == -1)) {
		JSONReader currentValue = this->getValue(it);
		it++; // skip ','

		values.push_back(currentValue);
		currentIndex--;
	}
	return (values);
}

std::vector<std::string> JSONReader::keys(void) {
	std::vector<std::string> keys;

	if (!(*_content.begin() == '{' && *(_content.end() - 1) == '}'))
		throw std::runtime_error("JSONReader::values is reserved for arrays.");

	std::string::iterator it = _content.begin() + 1;

	while (it != _content.end()) {
		std::string currentKey = this->getKey(it);
		it++; // skip ':'

		JSONReader currentValue = this->getValue(it);
		it++; // skip ','
		
		keys.push_back(currentKey);
	}
	
	return (keys);
}

std::string JSONReader::toString(void) const {
	if (*_content.begin() == '"')
		return std::string(_content.begin() + 1, _content.end() - 1);
	return (_content);
}

int JSONReader::toInt(void) const {
	if (!std::isdigit(*_content.begin()))
		throw std::runtime_error("Invalid conversion");
	return (std::atoi(_content.c_str()));
}

long JSONReader::toLong(void) const {
	if (!std::isdigit(*_content.begin()))
		throw std::runtime_error("Invalid conversion");
	return (std::atol(_content.c_str()));
}

long long JSONReader::toLongLong(void) const {
	if (!std::isdigit(*_content.begin()))
		throw std::runtime_error("Invalid conversion");
	return (std::atoll(_content.c_str()));
}

float JSONReader::toFloat(void) const {
	if (!std::isdigit(*_content.begin()))
		throw std::runtime_error("Invalid conversion");
	return (std::atof(_content.c_str()));
}

double JSONReader::toDouble(void) const {
	if (!std::isdigit(*_content.begin()))
		throw std::runtime_error("Invalid conversion");
	return (std::strtod(_content.c_str(), __null));
}

bool JSONReader::isArray(void) const {
	return (*_content.begin() == '[');
}

bool JSONReader::isDict(void) const {
	return (*_content.begin() == '{');
}

bool JSONReader::isNumber(void) const {
	return (std::isdigit(*_content.begin()));
}

bool JSONReader::isString(void) const {
	return (*_content.begin() == '"');
}