#include "json.h"
#include <iostream>

inline char get_char(std::istream &is) { return static_cast<char>(is.get()); }

namespace json
{
    JSON_EXPORT json load(std::istream &is)
    {
        is >> std::ws; // we remove all the leading whitespace..
        switch (is.peek())
        {
        case '{':
        { // we have a json object..
            get_char(is);
            std::map<std::string, json> vals;
            is >> std::ws;
            if (is.peek() == '}')
            { // we have an empty object..
                get_char(is);
                return vals;
            }
            do
            {
                is >> std::ws;
                if (is.peek() != '\"')
                    throw std::invalid_argument("not a valid json");
                std::string id = parse_string(is);
                is >> std::ws;
                if (get_char(is) != ':')
                    throw std::invalid_argument("not a valid json");
                auto val = load(is);
                vals.emplace(id, std::move(val));
                is >> std::ws;
            } while (is.peek() == ',' && get_char(is));
            if (get_char(is) != '}')
                throw std::invalid_argument("not a valid json");
            return vals;
        }
        case '[':
        { // we have a json array..
            get_char(is);
            std::vector<json> vals;
            if (is.peek() == ']')
            { // we have an empty array..
                get_char(is);
                return vals;
            }
            do
            {
                vals.emplace_back(load(is));
                is >> std::ws;
            } while (is.peek() == ',' && get_char(is));
            if (get_char(is) != ']')
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
            num += get_char(is);
            while (is.peek() == '0' || is.peek() == '1' || is.peek() == '2' || is.peek() == '3' || is.peek() == '4' || is.peek() == '5' || is.peek() == '6' || is.peek() == '7' || is.peek() == '8' || is.peek() == '9')
                num += get_char(is);
            if (is.peek() == '.')
            {
                num += get_char(is);
                while (is.peek() == '0' || is.peek() == '1' || is.peek() == '2' || is.peek() == '3' || is.peek() == '4' || is.peek() == '5' || is.peek() == '6' || is.peek() == '7' || is.peek() == '8' || is.peek() == '9')
                    num += get_char(is);
                if (is.peek() == 'e' || is.peek() == 'E')
                {
                    num += get_char(is);
                    if (is.peek() == '+')
                        num += get_char(is);
                    if (is.peek() == '-')
                        num += get_char(is);
                    while (is.peek() == '0' || is.peek() == '1' || is.peek() == '2' || is.peek() == '3' || is.peek() == '4' || is.peek() == '5' || is.peek() == '6' || is.peek() == '7' || is.peek() == '8' || is.peek() == '9')
                        num += get_char(is);
                    return json(num, true);
                }
                return json(num, true);
            }
            else if (is.peek() == 'e' || is.peek() == 'E')
            {
                num += get_char(is);
                if (is.peek() == '+')
                    num += get_char(is);
                if (is.peek() == '-')
                    num += get_char(is);
                while (is.peek() == '0' || is.peek() == '1' || is.peek() == '2' || is.peek() == '3' || is.peek() == '4' || is.peek() == '5' || is.peek() == '6' || is.peek() == '7' || is.peek() == '8' || is.peek() == '9')
                    num += get_char(is);
                return json(num, true);
            }
            else
                return json(num, true);
        }
        case '.':
        {
            std::string num;
            num += get_char(is);
            while (is.peek() == '0' || is.peek() == '1' || is.peek() == '2' || is.peek() == '3' || is.peek() == '4' || is.peek() == '5' || is.peek() == '6' || is.peek() == '7' || is.peek() == '8' || is.peek() == '9')
                num += get_char(is);
            if (is.peek() == 'e' || is.peek() == 'E')
            {
                num += get_char(is);
                if (is.peek() == '+')
                    num += get_char(is);
                if (is.peek() == '-')
                    num += get_char(is);
                while (is.peek() == '0' || is.peek() == '1' || is.peek() == '2' || is.peek() == '3' || is.peek() == '4' || is.peek() == '5' || is.peek() == '6' || is.peek() == '7' || is.peek() == '8' || is.peek() == '9')
                    num += get_char(is);
                return json(num, true);
            }
            return json(num, true);
        }
        case 'f':
        { // we have a false literal..
            get_char(is);
            if (get_char(is) == 'a' && get_char(is) == 'l' && get_char(is) == 's' && get_char(is) == 'e')
                return false;
            throw std::invalid_argument("not a valid json");
        }
        case 't':
        { // we have a true literal..
            get_char(is);
            if (get_char(is) == 'r' && get_char(is) == 'u' && get_char(is) == 'e')
                return true;
            throw std::invalid_argument("not a valid json");
        }
        case 'n':
        { // we have a null literal..
            get_char(is);
            switch (get_char(is))
            {
            case 'a':
                if (get_char(is) == 'n')
                    return nullptr;
                throw std::invalid_argument("not a valid json");
            case 'u':
                if (get_char(is) == 'l' && get_char(is) == 'l')
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
            get_char(is);
            if (is.peek() == '/')
            {
                while (is.peek() != '\n')
                    get_char(is);
                return load(is);
            }
            else if (is.peek() == '*')
            {
                while (is.peek() != '*')
                    get_char(is);
                get_char(is);
                if (is.peek() == '/')
                {
                    get_char(is);
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

    JSON_EXPORT std::string parse_string(std::istream &is)
    {
        get_char(is);
        std::string val;
        while (is.peek() != '\"')
            if (is.peek() == '\\')
            {
                get_char(is);
                switch (get_char(is))
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
                        c = get_char(is);
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
                val += get_char(is);
        get_char(is);
        return val;
    }
} // namespace json
