#include "json.hpp"
#include <iostream>

namespace json
{
    json to_list(const std::vector<json> &vals)
    {
        json j(json_type::array);
        for (const auto &val : vals)
            j.push_back(val);
        return j;
    }

    json load(std::istream &is)
    {
        is >> std::ws; // we remove all the leading whitespace..
        switch (is.peek())
        {
        case '{':
        { // we have a json object..
            is.get();
            std::map<std::string, json> vals;
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
                vals.emplace(id, std::move(val));
                is >> std::ws;
            } while (is.peek() == ',' && is.get());
            if (is.get() != '}')
                throw std::invalid_argument("not a valid json");
            return vals;
        }
        case '[':
        { // we have a json array..
            is.get();
            std::vector<json> vals;
            if (is.peek() == ']')
            { // we have an empty array..
                is.get();
                return vals;
            }
            do
            {
                vals.emplace_back(load(is));
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
            num += is.get();
            while (is.peek() == '0' || is.peek() == '1' || is.peek() == '2' || is.peek() == '3' || is.peek() == '4' || is.peek() == '5' || is.peek() == '6' || is.peek() == '7' || is.peek() == '8' || is.peek() == '9')
                num += is.get();
            if (is.peek() == '.')
            {
                num += is.get();
                while (is.peek() == '0' || is.peek() == '1' || is.peek() == '2' || is.peek() == '3' || is.peek() == '4' || is.peek() == '5' || is.peek() == '6' || is.peek() == '7' || is.peek() == '8' || is.peek() == '9')
                    num += is.get();
                if (is.peek() == 'e' || is.peek() == 'E')
                {
                    num += is.get();
                    if (is.peek() == '+')
                        num += is.get();
                    if (is.peek() == '-')
                        num += is.get();
                    while (is.peek() == '0' || is.peek() == '1' || is.peek() == '2' || is.peek() == '3' || is.peek() == '4' || is.peek() == '5' || is.peek() == '6' || is.peek() == '7' || is.peek() == '8' || is.peek() == '9')
                        num += is.get();
                    return json(num, true);
                }
                return json(num, true);
            }
            else if (is.peek() == 'e' || is.peek() == 'E')
            {
                num += is.get();
                if (is.peek() == '+')
                    num += is.get();
                if (is.peek() == '-')
                    num += is.get();
                while (is.peek() == '0' || is.peek() == '1' || is.peek() == '2' || is.peek() == '3' || is.peek() == '4' || is.peek() == '5' || is.peek() == '6' || is.peek() == '7' || is.peek() == '8' || is.peek() == '9')
                    num += is.get();
                return json(num, true);
            }
            else
                return json(num, true);
        }
        case '.':
        {
            std::string num;
            num += is.get();
            while (is.peek() == '0' || is.peek() == '1' || is.peek() == '2' || is.peek() == '3' || is.peek() == '4' || is.peek() == '5' || is.peek() == '6' || is.peek() == '7' || is.peek() == '8' || is.peek() == '9')
                num += is.get();
            if (is.peek() == 'e' || is.peek() == 'E')
            {
                num += is.get();
                if (is.peek() == '+')
                    num += is.get();
                if (is.peek() == '-')
                    num += is.get();
                while (is.peek() == '0' || is.peek() == '1' || is.peek() == '2' || is.peek() == '3' || is.peek() == '4' || is.peek() == '5' || is.peek() == '6' || is.peek() == '7' || is.peek() == '8' || is.peek() == '9')
                    num += is.get();
                return json(num, true);
            }
            return json(num, true);
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
                        c = is.get();
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
                val += is.get();
        is.get();
        return val;
    }
} // namespace json
