/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JSONUtils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehode <ehode@student.42angouleme.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 17:20:36 by ehode             #+#    #+#             */
/*   Updated: 2026/02/06 20:45:44 by ehode            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "JSONReader.hpp"
#include <stdexcept>
#include <string>

static std::string::iterator checker(std::string::iterator begin, std::string::iterator end) {
	std::string::iterator it = begin;
	bool isDict = false;
	if (*it == '{' || *it == '[')
	{
		isDict = *it == '{';
		it++;
	}
	else
		throw std::runtime_error("Invalid checker begin");
	
	for (; it != end; ) {
		if ((*it == '}' || *it == ']'))
			return (it + 1);

		// key (must start with ")
		if (isDict) {
			bool inScope = false;
			if (*it != '"')
				throw std::runtime_error("Invalid token: key must start by \"");
			std::string key;
			while (it != end) {
				if (*it == '"' && *(it - 1) != '\\')
					inScope = !inScope;
				else
					key += *it;
				it++;
				if (inScope == false)
					break;
			}

			// next token is :
			if (*it != ':')
				throw std::runtime_error("Invalid token: expecting : after key");
			it++;
		}

		// value
		std::string value;
		// Dict / Array
		if (*it == '[' || *it == '{')
		{
			std::string::iterator endValue = checker(it, end);
			value = std::string(it, endValue);
			it = endValue;
		// String
		} else if (*it == '"') {
			bool inScope = false;
			while (it != end) {
				if (*it == '"' && *(it - 1) != '\\')
					inScope = !inScope;
				else
					value += *it;
				it++;
				if (inScope == false)
					break;
			}
		} else if (std::isdigit(*it)) {
			int dotCount = 0;
			while (std::isdigit(*it) || *it == '.')
			{
				if (*it == '.')
					dotCount++;
				value += *it;
				it++;
			}
			if (dotCount > 1)
				throw std::runtime_error("Invalid number");
		}

		// next token is , }, ]
		if (*it == ',' || *it == '}' || *it == ']') {
			if (*it == ',')
				it++;
		} else
			throw std::runtime_error("Invalid token: expecting [, }, ]] after value");
	}
	return (it);
}

bool JSONReader::isValidJSON(std::string content) {
	const std::string allowedOutScope = "{}[],:0123456789.";

	for (std::string::iterator it = content.begin(); it != content.end(); it++) {
		if ((*it >= 8 && *it <= 13) || *it == ' ' || *it == '\n')
			content.erase(it--);
	}
	// begin / end checker ({} [])
	if (!((*content.begin() == '{' && *(content.end() - 1) == '}') ||
		(*content.begin() == '[' && *(content.end() - 1) == ']')))
		return (false);
	bool inScope = false;
	int depth[2] = {};
	for (std::string::iterator it = content.begin(); it != content.end(); it++) {
		if (*it == '"' && *(it - 1) != '\\')
			inScope = !inScope;
		else if (inScope == false && (*it == '{' || *it == '['))
			depth[*it == '{']++;
		else if (inScope == false && (*it == '}' || *it == ']'))
			depth[*it == '}']--;
		else if (inScope == false && allowedOutScope.find(*it) == std::string::npos) {
			throw std::runtime_error(std::string("Invalid character have been found: ") + *it);
			return (false);
		}
		if (depth[0] < 0 || depth[1] < 0)
			return (false);
	}
	if (inScope == true || depth[0] != 0 || depth[0] != 0)
		return (false);
	if (checker(content.begin(), content.end()) != content.end()) {
		return (false);
	}
	return (true);
}