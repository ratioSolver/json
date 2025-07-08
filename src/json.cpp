#include "json.hpp"
#include <algorithm>
#include <sstream>

namespace json
{
    json::json(std::initializer_list<json> init)
    {
        if (init.size() == 2 && init.begin()->is_string())
        { // we have a key-value pair..
            value = std::map<std::string, json, std::less<>>();
            std::get<std::map<std::string, json, std::less<>>>(value)[static_cast<std::string>(*init.begin())] = *(init.begin() + 1);
        }
        else if (std::all_of(init.begin(), init.end(), [](const json &j)
                             { return j.is_object() && j.size() == 1; }))
        { // we have an array of key-value pairs..
            value = std::map<std::string, json, std::less<>>();
            for (const auto &j : init)
                std::get<std::map<std::string, json, std::less<>>>(value)[static_cast<std::string>(j.as_object().begin()->first)] = j.as_object().begin()->second;
        }
        else
        { // we have an array..
            value = std::vector<json>();
            for (const auto &j : init)
                std::get<std::vector<json>>(value).push_back(j);
        }
    }

    std::string parse_string(std::istream &is)
    {
        is.get();
        std::string val;
        while (is.peek() != '\"')
            if (is.peek() == '\\')
            {
                is.get();
                switch (is.get())
                {
                case '\"':
                    val += '\"';
                    break;
                case '\\':
                    val += '\\';
                    break;
                case '/':
                    val += '/';
                    break;
                case 'b':
                    val += '\b';
                    break;
                case 'f':
                    val += '\f';
                    break;
                case 'n':
                    val += '\n';
                    break;
                case 'r':
                    val += '\r';
                    break;
                case 't':
                    val += '\t';
                    break;
                case 'u':
                {
                    int codepoint = 0;
                    const auto factors = {12u, 8u, 4u, 0u};
                    char c;
                    for (const auto factor : factors)
                    {
                        c = static_cast<char>(is.get());
                        if (c >= '0' && c <= '9')
                            codepoint += static_cast<int>((static_cast<unsigned int>(c) - 0x30u) << factor);
                        else if (c >= 'A' && c <= 'F')
                            codepoint += static_cast<int>((static_cast<unsigned int>(c) - 0x37u) << factor);
                        else if (c >= 'a' && c <= 'f')
                            codepoint += static_cast<int>((static_cast<unsigned int>(c) - 0x57u) << factor);
                        else
                            throw std::invalid_argument("not a valid json");
                    }

                    // translate codepoint into bytes
                    if (codepoint < 0x80)
                    { // 1-byte characters: 0xxxxxxx (ASCII)
                        val.push_back(static_cast<char>(codepoint));
                    }
                    else if (codepoint <= 0x7FF)
                    { // 2-byte characters: 110xxxxx 10xxxxxx
                        val.push_back(static_cast<char>(0xC0u | (static_cast<unsigned int>(codepoint) >> 6u)));
                        val.push_back(static_cast<char>(0x80u | (static_cast<unsigned int>(codepoint) & 0x3Fu)));
                    }
                    else if (codepoint <= 0xFFFF)
                    { // 3-byte characters: 1110xxxx 10xxxxxx 10xxxxxx
                        val.push_back(static_cast<char>(0xE0u | (static_cast<unsigned int>(codepoint) >> 12u)));
                        val.push_back(static_cast<char>(0x80u | ((static_cast<unsigned int>(codepoint) >> 6u) & 0x3Fu)));
                        val.push_back(static_cast<char>(0x80u | (static_cast<unsigned int>(codepoint) & 0x3Fu)));
                    }
                    else
                    { // 4-byte characters: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
                        val.push_back(static_cast<char>(0xF0u | (static_cast<unsigned int>(codepoint) >> 18u)));
                        val.push_back(static_cast<char>(0x80u | ((static_cast<unsigned int>(codepoint) >> 12u) & 0x3Fu)));
                        val.push_back(static_cast<char>(0x80u | ((static_cast<unsigned int>(codepoint) >> 6u) & 0x3Fu)));
                        val.push_back(static_cast<char>(0x80u | (static_cast<unsigned int>(codepoint) & 0x3Fu)));
                    }
                    break;
                }
                default:
                    throw std::invalid_argument("not a valid json");
                }
            }
            else
                val += static_cast<char>(is.get());
        is.get();
        return val;
    }

    json load(std::istream &is)
    {
        is >> std::ws; // we remove all the leading whitespace..
        switch (is.peek())
        {
        case '{':
        { // we have a json object..
            is.get();
            json vals;
            is >> std::ws;
            if (is.peek() == '}')
            { // we have an empty object..
                is.get();
                return vals;
            }
            do
            {
                is >> std::ws;
                if (is.peek() != '\"')
                    throw std::invalid_argument("not a valid json");
                std::string id = parse_string(is);
                is >> std::ws;
                if (is.get() != ':')
                    throw std::invalid_argument("not a valid json");
                auto val = load(is);
                vals[id] = std::move(val);
                is >> std::ws;
            } while (is.peek() == ',' && is.get());
            if (is.get() != '}')
                throw std::invalid_argument("not a valid json");
            return vals;
        }
        case '[':
        { // we have a json array..
            is.get();
            json vals(json_type::array);
            is >> std::ws;
            if (is.peek() == ']')
            { // we have an empty array..
                is.get();
                return vals;
            }
            do
            {
                vals.push_back(load(is));
                is >> std::ws;
            } while (is.peek() == ',' && is.get());
            if (is.get() != ']')
                throw std::invalid_argument("not a valid json");
            return vals;
        }
        case '-': // we have a json number..
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        {
            std::string num;
            num += static_cast<char>(is.get());
            while (is.peek() == '0' || is.peek() == '1' || is.peek() == '2' || is.peek() == '3' || is.peek() == '4' || is.peek() == '5' || is.peek() == '6' || is.peek() == '7' || is.peek() == '8' || is.peek() == '9')
                num += static_cast<char>(is.get());
            if (is.peek() == '.')
            {
                num += static_cast<char>(is.get());
                while (is.peek() == '0' || is.peek() == '1' || is.peek() == '2' || is.peek() == '3' || is.peek() == '4' || is.peek() == '5' || is.peek() == '6' || is.peek() == '7' || is.peek() == '8' || is.peek() == '9')
                    num += static_cast<char>(is.get());
                if (is.peek() == 'e' || is.peek() == 'E')
                {
                    num += static_cast<char>(is.get());
                    if (is.peek() == '+')
                        num += static_cast<char>(is.get());
                    if (is.peek() == '-')
                        num += static_cast<char>(is.get());
                    while (is.peek() == '0' || is.peek() == '1' || is.peek() == '2' || is.peek() == '3' || is.peek() == '4' || is.peek() == '5' || is.peek() == '6' || is.peek() == '7' || is.peek() == '8' || is.peek() == '9')
                        num += static_cast<char>(is.get());
                    return json(std::stod(num));
                }
                return json(std::stod(num));
            }
            else if (is.peek() == 'e' || is.peek() == 'E')
            {
                num += static_cast<char>(is.get());
                if (is.peek() == '+')
                    num += static_cast<char>(is.get());
                if (is.peek() == '-')
                    num += static_cast<char>(is.get());
                while (is.peek() == '0' || is.peek() == '1' || is.peek() == '2' || is.peek() == '3' || is.peek() == '4' || is.peek() == '5' || is.peek() == '6' || is.peek() == '7' || is.peek() == '8' || is.peek() == '9')
                    num += static_cast<char>(is.get());
                return json(std::stod(num));
            }
            else
                return json(static_cast<int64_t>(std::stol(num)));
        }
        case '.':
        {
            std::string num;
            num += static_cast<char>(is.get());
            while (is.peek() == '0' || is.peek() == '1' || is.peek() == '2' || is.peek() == '3' || is.peek() == '4' || is.peek() == '5' || is.peek() == '6' || is.peek() == '7' || is.peek() == '8' || is.peek() == '9')
                num += static_cast<char>(is.get());
            if (is.peek() == 'e' || is.peek() == 'E')
            {
                num += static_cast<char>(is.get());
                if (is.peek() == '+')
                    num += static_cast<char>(is.get());
                if (is.peek() == '-')
                    num += static_cast<char>(is.get());
                while (is.peek() == '0' || is.peek() == '1' || is.peek() == '2' || is.peek() == '3' || is.peek() == '4' || is.peek() == '5' || is.peek() == '6' || is.peek() == '7' || is.peek() == '8' || is.peek() == '9')
                    num += static_cast<char>(is.get());
                return json(std::stod(num));
            }
            return json(std::stod(num));
        }
        case 'f':
        { // we have a false literal..
            is.get();
            if (is.get() == 'a' && is.get() == 'l' && is.get() == 's' && is.get() == 'e')
                return false;
            throw std::invalid_argument("not a valid json");
        }
        case 't':
        { // we have a true literal..
            is.get();
            if (is.get() == 'r' && is.get() == 'u' && is.get() == 'e')
                return true;
            throw std::invalid_argument("not a valid json");
        }
        case 'n':
        { // we have a null literal..
            is.get();
            switch (is.get())
            {
            case 'a':
                if (is.get() == 'n')
                    return nullptr;
                throw std::invalid_argument("not a valid json");
            case 'u':
                if (is.get() == 'l' && is.get() == 'l')
                    return nullptr;
                throw std::invalid_argument("not a valid json");
            default:
                throw std::invalid_argument("not a valid json");
            }
        }
        case '\"': // we have a json string..
            return parse_string(is);
        case '/':
        { // we have a json comment..
            is.get();
            if (is.peek() == '/')
            {
                while (is.peek() != '\n')
                    is.get();
                return load(is);
            }
            else if (is.peek() == '*')
            {
                while (is.peek() != '*')
                    is.get();
                is.get();
                if (is.peek() == '/')
                {
                    is.get();
                    return load(is);
                }
                else
                    throw std::invalid_argument("not a valid json");
            }
            else
                throw std::invalid_argument("not a valid json");
        }
        default:
            throw std::invalid_argument("not a valid json");
        }
    }

    json load(std::string_view str)
    {
        std::stringstream ss;
        ss << str;
        return load(ss);
    }

    bool validate(const json &value, const json &schema, const json &schema_refs)
    {
        if (schema.contains("type"))
        { // we have a type..
            if (schema["type"] == "object")
            {
                if (!value.is_object())
                    return false;
                for (const auto &property : schema["properties"].as_object())
                {
                    if (!value.contains(property.first))
                        return false;
                    if (!validate(value[property.first], property.second, schema_refs))
                        return false;
                }
                return true;
            }
            else if (schema["type"] == "array")
            {
                if (!value.is_array())
                    return false;
                if (schema.contains("minItems"))
                {
                    uint64_t min = schema["minItems"];
                    if (value.size() < min)
                        return false;
                }
                if (schema.contains("maxItems"))
                {
                    uint64_t max = schema["maxItems"];
                    if (value.size() > max)
                        return false;
                }
                for (const auto &v : value.as_array())
                    if (!validate(v, schema["items"], schema_refs))
                        return false;
                return true;
            }
            else if (schema["type"] == "string")
            {
                if (!value.is_string())
                    return false;
                if (schema.contains("enum"))
                    return std::find(schema["enum"].as_array().begin(), schema["enum"].as_array().end(), value) != schema["enum"].as_array().end();
                return true;
            }
            else if (schema["type"] == "number")
            {
                if (!value.is_number())
                    return false;
                if (schema.contains("minimum"))
                {
                    double min = schema["minimum"];
                    double v = value;
                    if (v < min)
                        return false;
                }
                if (schema.contains("maximum"))
                {
                    double max = schema["maximum"];
                    double v = value;
                    if (v > max)
                        return false;
                }
                return true;
            }
            else if (schema["type"] == "integer")
            {
                if (!value.is_number())
                    return false;
                if (schema.contains("minimum"))
                {
                    int64_t min = schema["minimum"];
                    int64_t v = value;
                    if (v < min)
                        return false;
                }
                if (schema.contains("maximum"))
                {
                    int64_t max = schema["maximum"];
                    int64_t v = value;
                    if (v > max)
                        return false;
                }
                return true;
            }
            else if (schema["type"] == "boolean")
                return value.is_boolean();
            else if (schema["type"] == "null")
                return value.is_null();
            else if (schema["type"] == "any")
                return true;
            else
                return false;
        }
        else if (schema.contains("$ref"))
        { // we have a reference to another schema..
            std::string ref = schema["$ref"];
            size_t pos = 0;
            std::string token;
            json current = schema_refs;
            while ((pos = ref.find('/')) != std::string::npos)
            {
                token = ref.substr(0, pos);
                if (token != "#")
                {
                    if (!current.contains(token))
                        return false;
                    json next = current[token];
                    current = next;
                }
                ref.erase(0, pos + 1);
            }
            return validate(value, current[ref], schema_refs);
        }
        else if (schema.contains("allOf"))
        { // we have a list of schemas that must all be valid..
            for (const auto &s : schema["allOf"].as_array())
                if (!validate(value, s, schema_refs))
                    return false;
            return true;
        }
        else if (schema.contains("anyOf"))
        { // we have a list of schemas where at least one must be valid..
            for (const auto &s : schema["anyOf"].as_array())
                if (validate(value, s, schema_refs))
                    return true;
            return false;
        }
        else if (schema.contains("oneOf"))
        { // we have a list of schemas where exactly one must be valid..
            int count = 0;
            for (const auto &s : schema["oneOf"].as_array())
                if (validate(value, s, schema_refs))
                    count++;
            return count == 1;
        }
        else if (schema.contains("not")) // we have a schema that must not be valid..
            return !validate(value, schema["not"], schema_refs);
        return false;
    }
} // namespace json