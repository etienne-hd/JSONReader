# JSONReader

**a C++ 98 JSON Reader**

## Todo
- [x] Check if data is a valid JSON
- [ ] Custom exception (std::runtime_error 🙏)

## Usage

Example of `json` data:

```json
{
	"a": "abc\"",
	"b": [1, 2, [2, 4], 3],
	"c": ["1", "2,", "3"],
	"d": 67,
	"\"[][][]]]]}}{{[}]": "\"\"}{{{}}[]]]\"\"",
	"e": {
		"f": 123,
		"g": {
			"h": "Hello, World!"
		},
		"i": 123.456
	}
}
```

Read its content using `JSONReader`:

```c++
	#include "JSONReader.hpp"

	std::string content = {...};
	JSONReader jsonReader(content);

	std::cout << jsonReader["e"]["g"]["h"].toString() << std::endl; // Hello World
	std::cout << jsonReader["c"][1].toString() << std::endl; // 2,
	std::cout << jsonReader.get("b").get(1).toInt() << std::endl; // 2
	std::cout << jsonReader["e"].get("i").toDouble() << std::endl; // 123.456

	std::cout << "Getting all values of json[\"c\"]" << std::endl;
	std::vector<JSONReader> values = jsonReader["b"].values();
	for (std::vector<JSONReader>::iterator it = values.begin(); it != values.end(); it++) {
		try {
			std::cout << it->toInt() << std::endl;
		} catch (...) {
			std::cerr << "Cannot convert to int" << std::endl;
		}
	}
	// 1
	// 2
	// Cannot convert to int
	// 3

	std::cout << "Getting all keys of json" << std::endl;
	std::vector<std::string> keys = jsonReader.keys();
	for (std::vector<std::string>::iterator it = keys.begin(); it != keys.end(); it++) {
		std::cout << *it << std::endl;
	}
	// a
	// b
	// c
	// d
	// \"[][][]]]]}}{{[}]
	// e
```

## License

This project is licensed under the MIT License.