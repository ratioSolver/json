#include "json.h"

int main(int, char **)
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