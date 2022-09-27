#include "json.h"
#include <sstream>

namespace json
{
    inline char get_char(std::istream &is) { return static_cast<char>(is.get()); }

    JSON_EXPORT json::json() {}
    JSON_EXPORT json::json(bool val) : j(new bool_val(val)) {}
    JSON_EXPORT json::json(const std::string &str) : j(new string_val(str)) {}
    JSON_EXPORT json::json(const char *str) : j(new string_val(str)) {}
    JSON_EXPORT json::json(short int val) : j(new number_val(std::to_string(val))) {}
    JSON_EXPORT json::json(unsigned short int val) : j(new number_val(std::to_string(val))) {}
    JSON_EXPORT json::json(long val) : j(new number_val(std::to_string(val))) {}
    JSON_EXPORT json::json(long long val) : j(new number_val(std::to_string(val))) {}
    JSON_EXPORT json::json(unsigned long val) : j(new number_val(std::to_string(val))) {}
    JSON_EXPORT json::json(unsigned long long val) : j(new number_val(std::to_string(val))) {}
    JSON_EXPORT json::json(double val) : j(new number_val(std::to_string(val))) {}
    JSON_EXPORT json::json(long double val) : j(new number_val(std::to_string(val))) {}
    JSON_EXPORT json::json(json &&orig) : j(std::move(orig.j)) {}
    JSON_EXPORT json::json(object &&orig) { j.reset(new object(std::move(orig.vals))); }
    JSON_EXPORT json::json(array &&orig) { j.reset(new array(std::move(orig.vals))); }
    JSON_EXPORT json::json(bool_val &&orig) { j.reset(new bool_val(std::move(orig.val))); }
    JSON_EXPORT json::json(string_val &&orig) { j.reset(new string_val(std::move(orig.val))); }
    JSON_EXPORT json::json(number_val &&orig) { j.reset(new number_val(std::move(orig.val))); }
    JSON_EXPORT json::json(null_val &&) { j.reset(new null_val()); }
    JSON_EXPORT void json::operator=(bool val) { j.reset(new bool_val(val)); }
    JSON_EXPORT void json::operator=(const std::string &str) { j.reset(new string_val(str)); }
    JSON_EXPORT void json::operator=(const char *str) { j.reset(new string_val(str)); }
    JSON_EXPORT void json::operator=(short int val) { j.reset(new number_val(std::to_string(val))); }
    JSON_EXPORT void json::operator=(unsigned short int val) { j.reset(new number_val(std::to_string(val))); }
    JSON_EXPORT void json::operator=(long val) { j.reset(new number_val(std::to_string(val))); }
    JSON_EXPORT void json::operator=(long long val) { j.reset(new number_val(std::to_string(val))); }
    JSON_EXPORT void json::operator=(unsigned long val) { j.reset(new number_val(std::to_string(val))); }
    JSON_EXPORT void json::operator=(unsigned long long val) { j.reset(new number_val(std::to_string(val))); }
    JSON_EXPORT void json::operator=(double val) { j.reset(new number_val(std::to_string(val))); }
    JSON_EXPORT void json::operator=(long double val) { j.reset(new number_val(std::to_string(val))); }
    JSON_EXPORT void json::operator=(json val) { j.swap(val.j); }

    JSON_EXPORT json::operator bool_val &() const { return static_cast<bool_val &>(*j); }
    JSON_EXPORT json::operator string_val &() const { return static_cast<string_val &>(*j); }
    JSON_EXPORT json::operator number_val &() const { return static_cast<number_val &>(*j); }
    JSON_EXPORT json::operator object &() const { return static_cast<object &>(*j); }
    JSON_EXPORT json::operator array &() const { return static_cast<array &>(*j); }

    JSON_EXPORT std::string json::dump() const noexcept
    {
        std::stringstream ss;
        if (!j)
            return "{}";
        j->dump(ss);
        return ss.str();
    }

    json &json::operator[](const std::string &str)
    {
        if (!j)
            j = std::make_unique<object>();
        return (*j)[str];
    }
    json &json::operator[](size_t index)
    {
        if (!j)
            j = std::make_unique<array>();
        return (*j)[index];
    }

    JSON_EXPORT bool_val::bool_val(const bool &val) : val(val) {}
    JSON_EXPORT void bool_val::dump(std::ostream &os) const noexcept { os << std::boolalpha << val; }

    JSON_EXPORT string_val::string_val(const std::string &val) : val(val) {}
    JSON_EXPORT void string_val::dump(std::ostream &os) const noexcept { os << '"' << val << '"'; }

    JSON_EXPORT number_val::number_val(const std::string &val) : val(val) {}
    JSON_EXPORT void number_val::dump(std::ostream &os) const noexcept { os << val; }

    JSON_EXPORT object::object() {}
    JSON_EXPORT object::object(std::map<std::string, json> &&vs) : vals(std::move(vs)) {}

    JSON_EXPORT void object::dump(std::ostream &os) const noexcept
    {
        os << '{';
        for (auto v_it = vals.cbegin(); v_it != vals.cend(); ++v_it)
        {
            if (v_it != vals.cbegin())
                os << ", ";
            os << '\"' << v_it->first << "\": ";
            v_it->second.j->dump(os);
        }
        os << '}';
    }

    JSON_EXPORT array::array() {}
    JSON_EXPORT array::array(std::vector<json> &&vs) : vals(std::move(vs)) {}
    JSON_EXPORT void array::dump(std::ostream &os) const noexcept
    {
        os << '[';
        for (auto v_it = vals.cbegin(); v_it != vals.cend(); ++v_it)
        {
            if (v_it != vals.cbegin())
                os << ", ";
            (*v_it).j->dump(os);
        }
        os << ']';
    }

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
            if (is.peek() == '}') // we have an empty object..
                return object(std::move(vals));
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
                vals.emplace(id, load(is));
                is >> std::ws;
            } while (is.peek() == ',' && get_char(is));
            if (get_char(is) != '}')
                throw std::invalid_argument("not a valid json");
            return object(std::move(vals));
        }
        case '[':
        { // we have a json array..
            get_char(is);
            std::vector<json> vals;
            do
            {
                vals.push_back(load(is));
                is >> std::ws;
            } while (is.peek() == ',' && get_char(is));
            if (get_char(is) != ']')
                throw std::invalid_argument("not a valid json");
            return array(std::move(vals));
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
                    return number_val(num);
                }
                return number_val(num);
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
                return number_val(num);
            }
            else
                return number_val(num);
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
                return number_val(num);
            }
            return number_val(num);
        }
        case 'f':
        { // we have a false literal..
            get_char(is);
            if (get_char(is) == 'a' && get_char(is) == 'l' && get_char(is) == 's' && get_char(is) == 'e')
                return bool_val(false);
            throw std::invalid_argument("not a valid json");
        }
        case 't':
        { // we have a true literal..
            get_char(is);
            if (get_char(is) == 'r' && get_char(is) == 'u' && get_char(is) == 'e')
                return bool_val(true);
            throw std::invalid_argument("not a valid json");
        }
        case 'n':
        { // we have a null literal..
            get_char(is);
            if (get_char(is) == 'u' && get_char(is) == 'l' && get_char(is) == 'l')
                return null_val();
            throw std::invalid_argument("not a valid json");
        }
        case '\"':
        { // we have a json string..
            get_char(is);
            std::string id;
            while (is.peek() != '\"')
                id += get_char(is);
            get_char(is);
            return string_val(id);
        }
        default:
            throw std::invalid_argument("not a valid json");
        }
    }
} // namespace json
