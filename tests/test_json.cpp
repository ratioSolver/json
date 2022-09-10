#include "json.h"
#include <sstream>
#include <cassert>

void test_json_0()
{
    json::json o1;
    o1["a"] = 5l;
    o1["b"] = "test";

    json::json arr;
    arr[0] = 5l;
    arr[1] = "test";

    json::json o2;
    o2["a"] = 5l;
    o2["b"] = "test";
    arr[2] = std::move(o2);

    o1["c"] = std::move(arr);

    std::string js = o1.dump();
}

void test_json_1()
{
    std::stringstream s("{\"a\": \"a\", \"b\": 1, \"c\": 1.500000, \"d\": true, \"e\": [true, false], \"f\": null, \"g\": {\"a\": true, \"b\": 1}}");
    json::json j_obj = json::load(s);
    std::string js = j_obj.dump();
}

json::json get_json()
{
    json::array arr;
    return arr;
}

void test_json_2()
{
    auto arr = get_json();
    json::array &c_arr = arr;
    assert(c_arr.size() == 0);
}

int main(int, char **)
{
    test_json_0();
    test_json_1();
    test_json_2();
}