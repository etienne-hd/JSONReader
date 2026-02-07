#ifndef JSON_HPP
# define JSON_HPP

# include <string>
# include <vector>

class JSONReader {
	private:
		std::string		_data;
		std::string		_base_data;

		JSONReader getValue(std::string::iterator &it);
		std::string getKey(std::string::iterator &it);
		JSONReader(std::string data, std::string base_data);
	public:
		JSONReader(std::string data);

		static bool isValidJSON(std::string data);
		
		JSONReader get(std::string key);
		JSONReader get(int index);
		JSONReader operator[](int index);
		JSONReader operator[](std::string key);
		std::vector<JSONReader> values(int n = -1);
		std::vector<std::string> keys(void);

		std::string toString(void) const;
		int toInt(void) const;
		long toLong(void) const;
		long long toLongLong(void) const;
		float toFloat(void) const;
		double toDouble(void) const;
		bool toBool(void) const;
		
		bool isArray(void) const;
		bool isDict(void) const;
		bool isNumber(void) const;
		bool isString(void) const;
		bool isBool(void) const;
		bool isNull(void) const;
		unsigned int length(void);
};

#endif