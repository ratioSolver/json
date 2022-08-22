#include "json.h"
#include <sstream>

namespace json
{
    JSON_EXPORT json::json() {}
    JSON_EXPORT json::json(json &&orig) : j(std::move(orig.j)) {}
    JSON_EXPORT void json::operator=(bool val) { j.reset(new bool_val(val)); }
    JSON_EXPORT void json::operator=(const std::string &str) { j.reset(new string_val(str)); }
    JSON_EXPORT void json::operator=(const char *str) { j.reset(new string_val(str)); }
    JSON_EXPORT void json::operator=(long val) { j.reset(new int_val(val)); }
    JSON_EXPORT void json::operator=(double val) { j.reset(new float_val(val)); }
    JSON_EXPORT void json::operator=(json val) { j.swap(val.j); }

    JSON_EXPORT std::string json::dump() const noexcept
    {
        std::stringstream ss;
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

    JSON_EXPORT object::object() {}

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

    JSON_EXPORT bool_val::bool_val(const bool &val) : val(val) {}
    JSON_EXPORT void bool_val::dump(std::ostream &os) const noexcept { os << std::boolalpha << val; }
    JSON_EXPORT string_val::string_val(const std::string &val) : val(val) {}
    JSON_EXPORT void string_val::dump(std::ostream &os) const noexcept { os << '"' << val << '"'; }
    JSON_EXPORT int_val::int_val(const long &val) : val(val) {}
    JSON_EXPORT void int_val::dump(std::ostream &os) const noexcept { os << std::to_string(val); }
    JSON_EXPORT float_val::float_val(const double &val) : val(val) {}
    JSON_EXPORT void float_val::dump(std::ostream &os) const noexcept { os << std::to_string(val); }
} // namespace json
