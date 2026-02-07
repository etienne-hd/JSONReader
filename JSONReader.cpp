#include "JSONReader.hpp"

#include <stdexcept>
#include <string>
#include <cstdlib>
#include <vector>

JSONReader::JSONReader(std::string content, std::string content_base) {
	_data = content;
	_base_data = content_base;
}

JSONReader::JSONReader(std::string data) {
	if (!JSONReader::isValidJSON(data))
		throw std::runtime_error("Invalid JSON data");

	bool inScope = false;
	for (std::string::iterator it = data.begin(); it != data.end(); it++) {
		if (*it == '"' && *(it - 1) != '\\')
			inScope = !inScope;
		else if (inScope == false && ((*it >= 8 && *it <= 13) || *it == ' ' || *it == '\n'))
			data.erase(it--);
	}
	
	_data = data;
	_base_data = data;
}

JSONReader JSONReader::getValue(std::string::iterator &it) {
	std::string value;
	bool inScope = false;

	// Array / Dict
	if (*it == '{' || *it == '[') {
		char openChar = *it == '{' ? '{' : '[';
		char closeChar = *it == '{' ? '}' : ']';
		int depth = 0;
		while (it != _data.end() - 1)
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
		while (!(*it == ',' && inScope == false) && it != _data.end() - 1) {
			if (*it == '"' && *(it - 1) != '\\')
				inScope = !inScope;
			value += *it;
			it++;
		}
	}
	return (JSONReader(value, _base_data));
}

std::string JSONReader::getKey(std::string::iterator &it) {
	std::string key;
	bool inScope = false;

	while (!(*it == ':' && inScope == false) && it != _data.end() - 1) {
		if (*it == '"' && *(it - 1) != '\\')
			inScope = !inScope;
		else
			key += *it;
		it++;
	}
	return (key);
}

JSONReader JSONReader::get(std::string key) {
	if (!(*_data.begin() == '{' && *(_data.end() - 1) == '}'))
		throw std::runtime_error("Invalid Content");

	std::string::iterator it = _data.begin() + 1;

	while (it != _data.end()) {
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

	if (!(*_data.begin() == '[' && *(_data.end() - 1) == ']'))
		throw std::runtime_error("JSONReader::values is reserved for arrays.");

	std::string::iterator it = _data.begin() + 1;
	
	int currentIndex = 0;
	while (it < _data.end() - 1 && (currentIndex < n || n == -1)) {
		JSONReader currentValue = this->getValue(it);
		it++; // skip ','

		values.push_back(currentValue);
		currentIndex--;
	}
	return (values);
}

std::vector<std::string> JSONReader::keys(void) {
	std::vector<std::string> keys;

	if (!(*_data.begin() == '{' && *(_data.end() - 1) == '}'))
		throw std::runtime_error("JSONReader::values is reserved for arrays.");

	std::string::iterator it = _data.begin() + 1;

	while (it < _data.end() - 1) {
		std::string currentKey = this->getKey(it);
		it++; // skip ':'

		JSONReader currentValue = this->getValue(it);
		it++; // skip ','
		
		keys.push_back(currentKey);
	}
	
	return (keys);
}

std::string JSONReader::toString(void) const {
	if (*_data.begin() != '"')
		throw std::runtime_error("Invalid conversion");
	return std::string(_data.begin() + 1, _data.end() - 1);
}

int JSONReader::toInt(void) const {
	if (!(std::isdigit(*_data.begin()) || *_data.begin() == '-'))
		throw std::runtime_error("Invalid conversion");
	return (std::atoi(_data.c_str()));
}

long JSONReader::toLong(void) const {
	if (!(std::isdigit(*_data.begin()) || *_data.begin() == '-'))
		throw std::runtime_error("Invalid conversion");
	return (std::atol(_data.c_str()));
}

long long JSONReader::toLongLong(void) const {
	if (!(std::isdigit(*_data.begin()) || *_data.begin() == '-'))
		throw std::runtime_error("Invalid conversion");
	return (std::atoll(_data.c_str()));
}

float JSONReader::toFloat(void) const {
	if (!(std::isdigit(*_data.begin()) || *_data.begin() == '-'))
		throw std::runtime_error("Invalid conversion");
	return (std::atof(_data.c_str()));
}

double JSONReader::toDouble(void) const {
	if (!(std::isdigit(*_data.begin()) || *_data.begin() == '-'))
		throw std::runtime_error("Invalid conversion");
	return (std::strtod(_data.c_str(), __null));
}

bool JSONReader::toBool(void) const {
	if (!(_data == "true" || _data == "false"))
		throw std::runtime_error("Invalid conversion");
	return (_data == "true");
}

bool JSONReader::isArray(void) const {
	return (*_data.begin() == '[');
}

bool JSONReader::isDict(void) const {
	return (*_data.begin() == '{');
}

bool JSONReader::isNumber(void) const {
	return (std::isdigit(*_data.begin()) || *_data.begin() == '-');
}

bool JSONReader::isString(void) const {
	return (*_data.begin() == '"');
}

bool JSONReader::isBool(void) const {
	return (_data == "true" || _data == "false");
}

bool JSONReader::isNull(void) const {
	return (_data == "null");
}


unsigned int JSONReader::length(void) {
	return (this->values().size());
}