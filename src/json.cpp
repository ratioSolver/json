#include "json.h"
#include <sstream>
#include <cassert>

namespace json
{
    inline char get_char(std::istream &is) { return static_cast<char>(is.get()); }

    JSON_EXPORT json::json() : m_type(json_type::null), m_root(nullptr) {}
    JSON_EXPORT json::json(bool val) : m_type(json_type::boolean), m_root(new bool_val(val)) {}
    JSON_EXPORT json::json(const char *val) : m_type(json_type::string), m_root(new string_val(val)) {}
    JSON_EXPORT json::json(const std::string &val) : m_type(json_type::string), m_root(new string_val(val)) {}
    JSON_EXPORT json::json(long val) : m_type(json_type::number), m_root(new number_val(std::to_string(val))) {}
    JSON_EXPORT json::json(unsigned long val) : m_type(json_type::number), m_root(new number_val(std::to_string(val))) {}
    JSON_EXPORT json::json(double val) : m_type(json_type::number), m_root(new number_val(std::to_string(val))) {}
    JSON_EXPORT json::json(std::nullptr_t) : m_type(json_type::null), m_root(new null_val()) {}
    JSON_EXPORT json::json(bool_val &&other) : m_type(json_type::boolean), m_root(new bool_val(other.m_val)) {}
    JSON_EXPORT json::json(string_val &&other) : m_type(json_type::string), m_root(new string_val(other.m_val)) {}
    JSON_EXPORT json::json(number_val &&other) : m_type(json_type::number), m_root(new number_val(other.m_val)) {}
    JSON_EXPORT json::json(null_val &&) : m_type(json_type::null), m_root(new null_val()) {}
    JSON_EXPORT json::json(object &&other) : m_type(json_type::object), m_root(new object(std::move(other.m_map))) {}
    JSON_EXPORT json::json(array &&other) : m_type(json_type::array), m_root(new array(std::move(other.m_array))) {}
    JSON_EXPORT json::json(json &&other) : m_type(other.m_type), m_root(nullptr)
    {
        switch (other.m_type)
        {
        case json_type::null:
            m_root.reset(new null_val());
            break;
        case json_type::boolean:
            if (other.m_root)
                m_root.reset(new bool_val(static_cast<bool_val &>(*other.m_root).m_val));
            else
                m_root.reset(new bool_val(static_cast<bool_val &>(other).m_val));
            break;
        case json_type::string:
            if (other.m_root)
                m_root.reset(new string_val(static_cast<string_val &>(*other.m_root).m_val));
            else
                m_root.reset(new string_val(static_cast<string_val &>(other).m_val));
            break;
        case json_type::number:
            if (other.m_root)
                m_root.reset(new number_val(static_cast<number_val &>(*other.m_root).m_val));
            else
                m_root.reset(new number_val(static_cast<number_val &>(other).m_val));
            break;
        case json_type::object:
            if (other.m_root)
                m_root.reset(new object(std::move(static_cast<object &>(*other.m_root).m_map)));
            else
                m_root.reset(new object(std::move(static_cast<object &>(other).m_map)));
            break;
        case json_type::array:
            if (other.m_root)
                m_root.reset(new array(std::move(static_cast<array &>(*other.m_root).m_array)));
            else
                m_root.reset(new array(std::move(static_cast<array &>(other).m_array)));
            break;
        }
    }
    JSON_EXPORT json::json(std::map<std::string, json> &&other) : m_type(json_type::object), m_root(new object(std::move(other))) {}
    JSON_EXPORT json::json(std::vector<json> &&other) : m_type(json_type::array), m_root(new array(std::move(other))) {}

    JSON_EXPORT json::operator bool_val &() const
    {
        assert(m_type == json_type::boolean);
        return static_cast<bool_val &>(*m_root);
    }
    JSON_EXPORT void json::operator=(bool val)
    {
        m_type = json_type::boolean;
        m_root.reset(new bool_val(val));
    }
    JSON_EXPORT json::operator bool() const
    {
        assert(m_type == json_type::boolean);
        return static_cast<bool_val &>(*m_root).m_val;
    }
    JSON_EXPORT bool json::operator==(bool val) const
    {
        assert(m_type == json_type::boolean);
        return static_cast<bool_val &>(*m_root).m_val == val;
    }

    JSON_EXPORT json::operator string_val &() const
    {
        assert(m_type == json_type::string);
        return static_cast<string_val &>(*m_root);
    }
    JSON_EXPORT void json::operator=(const char *val)
    {
        m_type = json_type::string;
        m_root.reset(new string_val(val));
    }
    JSON_EXPORT json::operator const char *() const
    {
        assert(m_type == json_type::string);
        return static_cast<string_val &>(*m_root).m_val.c_str();
    }
    JSON_EXPORT json::operator std::string() const
    {
        assert(m_type == json_type::string);
        return static_cast<string_val &>(*m_root).m_val;
    }
    JSON_EXPORT bool json::operator==(const char *val) const
    {
        assert(m_type == json_type::string);
        return static_cast<string_val &>(*m_root).m_val == val;
    }

    JSON_EXPORT json::operator number_val &() const
    {
        assert(m_type == json_type::number);
        return static_cast<number_val &>(*m_root);
    }
    JSON_EXPORT void json::operator=(long val)
    {
        m_type = json_type::number;
        m_root.reset(new number_val(std::to_string(val)));
    }
    JSON_EXPORT json::operator long() const
    {
        assert(m_type == json_type::number);
        return std::stol(static_cast<number_val &>(*m_root).m_val);
    }
    JSON_EXPORT bool json::operator==(long val) const
    {
        assert(m_type == json_type::number);
        return std::stol(static_cast<number_val &>(*m_root).m_val) == val;
    }

    JSON_EXPORT void json::operator=(unsigned long val)
    {
        m_type = json_type::number;
        m_root.reset(new number_val(std::to_string(val)));
    }
    JSON_EXPORT json::operator unsigned long() const
    {
        assert(m_type == json_type::number);
        return std::stoul(static_cast<number_val &>(*m_root).m_val);
    }
    JSON_EXPORT bool json::operator==(unsigned long val) const
    {
        assert(m_type == json_type::number);
        return std::stoul(static_cast<number_val &>(*m_root).m_val) == val;
    }

    JSON_EXPORT void json::operator=(double val)
    {
        m_type = json_type::number;
        m_root.reset(new number_val(std::to_string(val)));
    }
    JSON_EXPORT json::operator double() const
    {
        assert(m_type == json_type::number);
        return std::stod(static_cast<number_val &>(*m_root).m_val);
    }
    JSON_EXPORT bool json::operator==(double val) const
    {
        assert(m_type == json_type::number);
        return std::stod(static_cast<number_val &>(*m_root).m_val) == val;
    }

    JSON_EXPORT json::operator null_val &() const
    {
        assert(m_type == json_type::null);
        return static_cast<null_val &>(*m_root);
    }
    JSON_EXPORT void json::operator=(std::nullptr_t)
    {
        m_type = json_type::null;
        m_root.reset(new null_val());
    }
    JSON_EXPORT json::operator std::nullptr_t() const
    {
        assert(m_type == json_type::null);
        return nullptr;
    }
    JSON_EXPORT bool json::operator==(std::nullptr_t) const
    {
        assert(m_type == json_type::null);
        return true;
    }

    JSON_EXPORT json::json::operator object &() const
    {
        assert(m_type == json_type::object);
        return static_cast<object &>(*m_root);
    }
    JSON_EXPORT void json::operator=(object &&val)
    {
        m_type = json_type::object;
        m_root.reset(new object(std::move(val.m_map)));
    }
    JSON_EXPORT json::operator const object &() const
    {
        assert(m_type == json_type::object);
        return static_cast<object &>(*m_root);
    }
    JSON_EXPORT bool json::operator==(const object &val) const
    {
        assert(m_type == json_type::object);
        return static_cast<object &>(*m_root) == val;
    }

    JSON_EXPORT json::operator array &() const
    {
        assert(m_type == json_type::array);
        return static_cast<array &>(*m_root);
    }
    JSON_EXPORT void json::operator=(array &&val)
    {
        m_type = json_type::array;
        m_root.reset(new array(std::move(val.m_array)));
    }
    JSON_EXPORT json::operator const array &() const
    {
        assert(m_type == json_type::array);
        return static_cast<array &>(*m_root);
    }
    JSON_EXPORT bool json::operator==(const array &val) const
    {
        assert(m_type == json_type::array);
        return static_cast<array &>(*m_root) == val;
    }

    JSON_EXPORT bool json::operator==(const json &other) const
    {
        if (m_type != other.m_type)
            return false;

        switch (m_type)
        {
        case json_type::boolean:
            return static_cast<bool_val &>(*m_root) == other;
        case json_type::string:
            return static_cast<string_val &>(*m_root) == other;
        case json_type::number:
            return static_cast<number_val &>(*m_root) == other;
        case json_type::object:
            return static_cast<object &>(*m_root) == static_cast<object &>(*other.m_root);
        case json_type::array:
            return static_cast<array &>(*m_root) == static_cast<array &>(*other.m_root);
        default:
            return true;
        }
    }

    JSON_EXPORT json &json::operator[](const char *key)
    {
        if (m_type != json_type::object)
        {
            m_type = json_type::object;
            m_root.reset(new object());
        }
        return static_cast<object &>(*m_root)[key];
    }
    JSON_EXPORT const json &json::operator[](const char *key) const
    {
        assert(m_type == json_type::object);
        return static_cast<object &>(*m_root)[key];
    }

    JSON_EXPORT json &json::operator[](size_t index)
    {
        if (m_type != json_type::array)
        {
            m_type = json_type::array;
            m_root.reset(new array());
        }
        return static_cast<array &>(*m_root)[index];
    }
    JSON_EXPORT const json &json::operator[](size_t index) const
    {
        assert(m_type == json_type::array);
        return static_cast<array &>(*m_root)[index];
    }

    JSON_EXPORT std::string json::dump() const noexcept
    {
        if (!m_root)
            return "{}";
        std::stringstream ss;
        dump(ss);
        return ss.str();
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
