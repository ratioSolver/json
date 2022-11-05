#include "json.h"
#include <sstream>
#include <cassert>

void test_json_0()
{
    json::json o1;
    o1["a"] = 5l;
    o1["b"] = "test";
    o1["c"] = 1.5;
    o1["d"] = true;

    json::number_val &o_a = o1["a"];
    long a = o_a;
    json::string_val &o_b = o1["b"];
    std::string b = o_b;
    json::bool_val &o_d = o1["d"];
    bool d = o_d;

    json::json arr;
    arr[0] = 5l;
    arr[1] = "test";
    arr[2] = 5ll;
    arr[3] = 5ul;
    arr[4] = 5ull;
    arr[5] = 5.0l;
    arr[6] = true;

    json::json o2;
    o2["a"] = 5l;
    o2["b"] = "test";
    arr[2] = std::move(o2);

    o1["c"] = std::move(arr);

    json::object &c_o2 = o1["c"];

    std::string js = o1.dump();
}

void test_json_1()
{
    json::json j_obj_0;
    std::string js = j_obj_0.dump();

    json::json j_obj = json::load("{\"a\": \"a\", \"b\": 1, \"c\": 1.500000, \"d\": true, \"e\": [true, false], \"f\": null, \"g\": {\"a\": true, \"b\": 1}}");
    js = j_obj.dump();
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

void test_json_3()
{
    json::json j_obj0 = json::load("{\"exec0\": [], \"exec1\": {}, \"t0\": 1E+10, \"t1\": 1.23E+10, \"t2\": .23E+10, \"t3\": .23}");
    std::string js0 = j_obj0.dump();

    json::json j_obj1 = json::load("{\"t0\": 1E-10, \"t1\": 1.23E-10, \"t2\": .23E-10}");
    std::string js1 = j_obj1.dump();
}

int main(int, char **)
{
    test_json_0();
    test_json_1();
    test_json_2();
    test_json_3();
}