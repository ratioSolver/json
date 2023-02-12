#include "json.h"
#include "logging.h"
#include <sstream>
#include <cassert>

void test_json()
{
    json::json j = true;
    assert(j == true);

    j = "hello";
    assert(j == "hello");

    j = nullptr;
    assert(j == nullptr);

    j = 42l;
    assert(j == 42l);

    j = 42ul;
    assert(j == 42ul);

    j = 42.0;
    assert(j == 42.0);

    json::json j2 = std::move(j);
    assert(j2 == 42.0);

    json::object o;
    o["hello"] = "world";
    assert(o["hello"] == "world");

    json::array a;
    a.push_back(42l);
    assert(a[0] == 42l);

    json::array a2;
    a2.push_back(43l);
    LOG(a2);
    a.push_back(std::move(a2));
    LOG(a);

    j["object"] = std::move(o);
    assert(j["object"]["hello"] == "world");

    j["array"] = std::move(a);

    LOG(j);

    auto j3 = json::load(j.dump());
    LOG(j3);
    assert(j3 == j);
}

void test_json_0()
{
    json::json o1;
    o1["a"] = 5l;
    o1["b"] = "test";
    o1["c"] = 1.5;
    o1["d"] = true;

    [[maybe_unused]] long a = o1["a"];
    LOG(a);
    std::string b = o1["b"];
    LOG(b);
    [[maybe_unused]] bool d = o1["d"];
    LOG(d);

    json::array arr;
    arr.push_back(5l);
    arr.push_back("test");
    arr.push_back(1.5);
    arr.push_back(5ul);
    arr.push_back(true);
    LOG(arr);

    json::json o2;
    o2["a"] = 5l;
    o2["b"] = "test";
    arr.set(2, std::move(o2));
    LOG(arr);

    o1["c"] = std::move(arr);

    [[maybe_unused]] json::array &c_o2 = o1["c"];
    LOG(c_o2);

    std::string js = o1.dump();
    LOG(js);
}

void test_json_1()
{
    json::json j_obj_0;
    std::string js = j_obj_0.dump();
    LOG(js);

    json::json j_obj = json::load("{\"a\": \"a\", \"b\": 1, \"c\": 1.500000, \"d\": true, \"e\": [true, false], \"f\": null, \"g\": {\"a\": true, \"b\": 1}}");
    js = j_obj.dump();
    LOG(js);
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
    LOG(js0);
    [[maybe_unused]] double t0 = j_obj0["t0"];
    LOG(t0);
    [[maybe_unused]] double t1 = j_obj0["t1"];
    LOG(t1);
    [[maybe_unused]] double t2 = j_obj0["t2"];
    LOG(t2);
    [[maybe_unused]] double t3 = j_obj0["t3"];
    LOG(t3);

    json::json j_obj1 = json::load("{\"t0\": 1E-10, \"t1\": 1.23E-10, \"t2\": .23E-10}");
    std::string js1 = j_obj1.dump();
    LOG(js1);
    [[maybe_unused]] double t4 = j_obj1["t0"];
    LOG(t4);
    [[maybe_unused]] double t5 = j_obj1["t1"];
    LOG(t5);
    [[maybe_unused]] double t6 = j_obj1["t2"];
    LOG(t6);
}

int main(int, char **)
{
    test_json();
    test_json_0();
    test_json_1();
    test_json_2();
    test_json_3();

    return 0;
}