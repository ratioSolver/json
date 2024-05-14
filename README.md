# JSON

![Build Status](https://github.com/ratioSolver/json/actions/workflows/cmake.yml/badge.svg)
[![codecov](https://codecov.io/gh/ratioSolver/json/branch/master/graph/badge.svg)](https://codecov.io/gh/ratioSolver/json)

This repository contains a [JSON](http://www.json.org/) parser and generator written in C++.

## Read a JSON from a string

```c++
#include "json.hpp"
#include <iostream>

int main()
{
    std::string json_string = "{\"key\": \"value\"}";
    json::json j = json::load(json_string);
    std::cout << j["key"] << std::endl;
    return 0;
}
```

## Write a JSON to a string

```c++
#include "json.hpp"
#include <iostream>

int main()
{
    json::json j;
    j["key"] = "value";
    j["number"] = 42;
    j["array"] = {1, 2, 3};
    j["object"] = {{"key", "value"}};
    std::cout << j.dump() << std::endl;
    return 0;
}
```