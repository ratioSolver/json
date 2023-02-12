#include "json.h"

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
                if (get_char(is) != '\"')
                    throw std::invalid_argument("not a valid json");
                std::string id;
                while (is.peek() != '\"')
                    id += get_char(is);
                get_char(is);
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
            if (get_char(is) == 'u' && get_char(is) == 'l' && get_char(is) == 'l')
                return nullptr;
            throw std::invalid_argument("not a valid json");
        }
        case '\"':
        { // we have a json string..
            get_char(is);
            std::string id;
            while (is.peek() != '\"')
                id += get_char(is);
            get_char(is);
            return id;
        }
        default:
            throw std::invalid_argument("not a valid json");
        }
    }
} // namespace json
