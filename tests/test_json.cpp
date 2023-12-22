#include "json.hpp"
#include "logging.h"
#include <sstream>
#include <cassert>

void test_json()
{
    std::stringstream ss;
    ss << R"(
{
    "a": 1,
    "b": 2.0,
    "c": "3",
    "d": true,
    "e": null,
    "f": [1, 2]
}
)";
    json::json j = json::load(ss);
    assert(j["a"] == 1);
    assert(j["b"] == 2.0);
    assert(j["c"] == "3");
    assert(j["d"] == true);
    assert(j["e"] == nullptr);
    assert(j["f"][0] == 1);
    assert(j["f"][1] == 2);
}

void test_json_escapes()
{
    std::stringstream ss;
    ss << R"({
  "id": "chatcmpl-7ToQ062g3kFeb8Tkds98KnqrAKyX7",
  "object": "chat.completion",
  "created": 1687339676,
  "model": "gpt-3.5-turbo-0613",
  "choices": [
    {
      "index": 0,
      "message": {
        "role": "assistant",
        "content": null,
        "function_call": {
          "name": "create_solver",
          "arguments": "{\n  \"purpose\": \"attività personalizzate\"\n}"
        }
      },
      "finish_reason": "function_call"
    }
  ],
  "usage": {
    "prompt_tokens": 163,
    "completion_tokens": 19,
    "total_tokens": 182
  }
}
)";
    json::json j = json::load(ss);
}

void test_json_special_chars()
{
    std::stringstream ss;
    ss << R"(
{
    "a": "\b\f\n\r\t\"\\"
}
)";
    json::json j = json::load(ss);
    LOG(j);
    assert(j["a"] == "\b\f\n\r\t\"\\");
}

void test_initializer_lists()
{
    json::json j = {
        {"a", 1},
        {"b", 2.0},
        {"c", "3"},
        {"d", true},
        {"e", nullptr},
        {"f", {1, 2}},
        {"g", {{"h", 1}, {"i", 2}}}};
    LOG(j);
    assert(j["a"] == 1);
    assert(j["b"] == 2.0);
    assert(j["c"] == "3");
    assert(j["d"] == true);
    assert(j["e"] == nullptr);
    assert(j["f"][0] == 1);
    assert(j["f"][1] == 2);
    assert(j["g"]["h"] == 1);
    assert(j["g"]["i"] == 2);
}

void test_json_comparison()
{
    json::json j0 = nullptr;
    json::json j1 = nullptr;
    assert(j0 == j1);

    j0["a"] = 1;
    j0["b"] = 2.0;
    j0["c"] = "3";
    j0["d"] = true;
    j0["e"] = nullptr;

    j1["a"] = 1;
    j1["b"] = 2.0;
    j1["c"] = "3";
    j1["d"] = true;
    j1["e"] = nullptr;

    assert(j0 == j1);

    j0["f"].push_back(1);
    j0["f"].push_back(2);

    j1["f"].push_back(1);
    j1["f"].push_back(2);

    assert(j0 == j1);

    j0["f"].push_back(3);
    assert(j0 != j1);
}

void test_move_semantics()
{
    json::json j0 = nullptr;
    j0["a"] = 1;
    j0["b"] = 2.0;
    j0["c"] = "3";
    j0["d"] = true;
    j0["e"] = nullptr;
    j0["f"].push_back(1);
    j0["f"].push_back(2);
    j0["f"].push_back(3);

    json::json j1 = std::move(j0);
    assert(j0 == nullptr);
    assert(j1 != nullptr);
    assert(j1["a"] == 1);
    assert(j1["b"] == 2.0);
    assert(j1["c"] == "3");
    assert(j1["d"] == true);
    assert(j1["e"] == nullptr);
    assert(j1["f"][0] == 1);
    assert(j1["f"][1] == 2);
    assert(j1["f"][2] == 3);
}

void test_move_into_array()
{
    json::json j0;
    j0.push_back(1);
    j0.push_back(2);
    j0.push_back(3);

    json::json j1;
    j1.push_back(std::move(j0));

    assert(j0 == nullptr);
    assert(j1 != nullptr);
    assert(j1[0][0] == 1);
    assert(j1[0][1] == 2);
    assert(j1[0][2] == 3);
}

void test_iterate()
{
    json::json j0 = nullptr;
    j0["a"] = 1;
    j0["b"] = 2.0;
    j0["c"] = "3";
    j0["d"] = true;
    j0["e"] = nullptr;
    j0["f"].push_back(1);
    j0["f"].push_back(2);
    j0["f"].push_back(3);

    std::map<std::string, json::json> &m = j0.get_object();
    for ([[maybe_unused]] auto &[key, value] : m)
        LOG("key " << key << " value " << value);

    json::json j1 = nullptr;
    j1.push_back(1);
    j1.push_back(2);
    j1.push_back(3);

    for ([[maybe_unused]] auto &value : j1.get_array())
        LOG("value " << value);
}

void test_null()
{
    json::json j0 = nullptr;
    assert(j0 == nullptr);
    assert(j0.get_type() == json::json_type::null);
    assert(j0.to_string() == "null");
}

void test_empty_array()
{
    json::json j0 = json::json_type::array;
    assert(j0.get_type() == json::json_type::array);
    assert(j0.to_string() == "[]");
}

void test_empty_object()
{
    json::json j0 = json::json_type::object;
    assert(j0.get_type() == json::json_type::object);
    assert(j0.to_string() == "{}");
}

void test_scientific_numbers()
{
    json::json j0 = 1e+10;
    assert(j0.get_type() == json::json_type::number);
    assert(j0.to_string() == std::to_string(1e+10));

    json::json j1 = 1.23e+10;
    assert(j1.get_type() == json::json_type::number);
    assert(j1.to_string() == std::to_string(1.23e+10));

    json::json j2 = .23e+10;
    assert(j2.get_type() == json::json_type::number);
    assert(j2.to_string() == std::to_string(.23e+10));
}

void test_array_of_scientific_numbers()
{
    json::json j0 = json::json_type::array;
    j0.push_back(1e+10);
    j0.push_back(1.23e+10);
    j0.push_back(.23e+10);
    assert(j0.get_type() == json::json_type::array);
    assert(j0.to_string() == "[" + std::to_string(1e+10) + "," + std::to_string(1.23e+10) + "," + std::to_string(.23e+10) + "]");
}

int main(int, char **)
{
    test_json();
    test_json_escapes();
    test_json_special_chars();
    test_initializer_lists();

    test_json_comparison();
    test_iterate();
    test_move_semantics();
    test_move_into_array();

    test_null();
    test_empty_array();
    test_empty_object();

    test_scientific_numbers();
    test_array_of_scientific_numbers();

    return 0;
}