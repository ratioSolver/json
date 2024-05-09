#pragma once

#include <string>
#include <variant>
#include <map>
#include <vector>

namespace json
{
  enum class json_type
  {
    null,
    string,
    number,
    boolean,
    array,
    object
  };

  class json
  {
  public:
    json(json_type type = json_type::object) noexcept
    {
      switch (type)
      {
      case json_type::null:
        value = nullptr;
        break;
      case json_type::boolean:
        value = false;
        break;
      case json_type::number:
        value = 0;
        break;
      case json_type::string:
        value = std::string();
        break;
      case json_type::array:
        value = std::vector<json>();
        break;
      case json_type::object:
        value = std::map<std::string, json>();
        break;
      }
    }
    json(const json &other) noexcept : value(other.value) {}
    json(json &&other) noexcept : value(std::move(other.value)) { other.value = nullptr; }
    json(std::nullptr_t) noexcept : value(nullptr) {}
    json(bool b) noexcept : value(b) {}
    json(int l) noexcept : value(l) {}
    json(int64_t l) noexcept : value(l) {}
    json(uint64_t l) noexcept : value(l) {}
    json(double d) noexcept : value(d) {}
    json(const std::string &str) noexcept : value(str) {}
    json(const char *str) noexcept : value(std::string(str)) {}
    json(std::initializer_list<json> init)
    {
      if (init.size() == 2 && init.begin()->get_type() == json_type::string)
      {
        value = std::map<std::string, json>();
        operator[](static_cast<std::string>(*init.begin())) = *(init.begin() + 1);
      }
      else if (init.begin()->get_type() == json_type::object)
      {
        value = std::map<std::string, json>();
        for (auto &obj : init)
          for (auto &[key, value] : obj.as_object())
            operator[](key) = value;
      }
      else
        value = std::vector<json>(init);
    }

    json &operator=(const json &other) noexcept
    {
      value = other.value;
      return *this;
    }

    json &operator=(json &&other) noexcept
    {
      value = std::move(other.value);
      return *this;
    }

    json &operator=(std::nullptr_t) noexcept
    {
      value = nullptr;
      return *this;
    }

    json &operator=(const std::string &str) noexcept
    {
      value = str;
      return *this;
    }

    json &operator=(const char *str) noexcept
    {
      value = std::string(str);
      return *this;
    }

    json &operator=(bool b) noexcept
    {
      value = b;
      return *this;
    }

    json &operator=(int l) noexcept
    {
      value = l;
      return *this;
    }

    json &operator=(int64_t l) noexcept
    {
      value = l;
      return *this;
    }

    json &operator=(uint64_t l) noexcept
    {
      value = l;
      return *this;
    }

    json &operator=(double d) noexcept
    {
      value = d;
      return *this;
    }

    json &operator[](const char *str) { return operator[](std::string(str)); }
    const json &operator[](const char *str) const { return operator[](std::string(str)); }

    json &operator[](const std::string &key) { return std::get<std::map<std::string, json>>(value)[key]; }
    const json &operator[](const std::string &key) const { return std::get<std::map<std::string, json>>(value).at(key); }

    json &operator[](int index) { return operator[](static_cast<size_t>(index)); }
    const json &operator[](int index) const { return operator[](static_cast<size_t>(index)); }

    json &operator[](size_t index) { return std::get<std::vector<json>>(value)[index]; }
    const json &operator[](size_t index) const { return std::get<std::vector<json>>(value).at(index); }

    json_type get_type() const noexcept
    {
      switch (value.index())
      {
      case 0:
        return json_type::null;
      case 1:
        return json_type::boolean;
      case 2:
      case 3:
      case 4:
        return json_type::number;
      case 5:
        return json_type::string;
      case 6:
        return json_type::object;
      case 7:
        return json_type::array;
      default: // should never happen
        return json_type::null;
      }
    }

    size_t size() const
    {
      switch (get_type())
      {
      case json_type::array:
        return std::get<std::vector<json>>(value).size();
      case json_type::object:
        return std::get<std::map<std::string, json>>(value).size();
      default:
        return 0;
      }
    }

    bool contains(const std::string &key) const { return get_type() == json_type::object && std::get<std::map<std::string, json>>(value).count(key) > 0; }

    bool operator==(const json &other) const noexcept { return value == other.value; }
    bool operator==(std::nullptr_t) const noexcept { return value.index() == 0; }
    bool operator==(bool b) const noexcept { return value.index() == 1 && std::get<bool>(value) == b; }
    bool operator==(int l) const noexcept
    {
      switch (value.index())
      {
      case 1:
        return std::get<bool>(value) == l;
      case 2:
        return std::get<int64_t>(value) == l;
      case 3:
        return std::get<uint64_t>(value) == static_cast<uint64_t>(l);
      case 4:
        return std::get<double>(value) == l;
      default:
        return false;
      }
    }
    bool operator==(int64_t l) const noexcept { return value.index() == 2 && std::get<int64_t>(value) == l; }
    bool operator==(uint64_t l) const noexcept { return value.index() == 3 && std::get<uint64_t>(value) == l; }
    bool operator==(double d) const noexcept { return value.index() == 4 && std::get<double>(value) == d; }
    bool operator==(const std::string &str) const noexcept { return value.index() == 5 && std::get<std::string>(value) == str; }
    bool operator==(const char *str) const noexcept { return value.index() == 5 && std::get<std::string>(value) == str; }

    bool operator!=(const json &other) const noexcept { return value != other.value; }
    bool operator!=(std::nullptr_t) const noexcept { return value.index() != 0; }
    bool operator!=(bool b) const noexcept { return value.index() != 1 || std::get<bool>(value) != b; }
    bool operator!=(int l) const noexcept { return !(*this == l); }
    bool operator!=(int64_t l) const noexcept { return value.index() != 2 || std::get<int64_t>(value) != l; }
    bool operator!=(uint64_t l) const noexcept { return value.index() != 3 || std::get<uint64_t>(value) != l; }
    bool operator!=(double d) const noexcept { return value.index() != 4 || std::get<double>(value) != d; }
    bool operator!=(const std::string &str) const noexcept { return value.index() != 5 || std::get<std::string>(value) != str; }
    bool operator!=(const char *str) const noexcept { return value.index() != 5 || std::get<std::string>(value) != str; }

    operator bool() const noexcept
    {
      switch (value.index())
      {
      case 1:
        return std::get<bool>(value);
      case 2:
        return std::get<int64_t>(value) != 0;
      case 3:
        return std::get<u_int64_t>(value) != 0;
      case 4:
        return !std::get<double>(value);
      case 5:
        return !std::get<std::string>(value).empty();
      case 6:
        return !std::get<std::map<std::string, json>>(value).empty();
      case 7:
        return !std::get<std::vector<json>>(value).empty();
      default:
        return false;
      }
    }

    operator int64_t() const noexcept
    {
      switch (value.index())
      {
      case 1:
        return std::get<bool>(value) ? 1 : 0;
      case 2:
        return std::get<int64_t>(value);
      case 3:
        return std::get<uint64_t>(value);
      case 4:
        return static_cast<int64_t>(std::get<double>(value));
      default:
        return 0;
      }
    }

    operator uint64_t() const noexcept
    {
      switch (value.index())
      {
      case 1:
        return std::get<bool>(value) ? 1 : 0;
      case 2:
        return std::get<int64_t>(value);
      case 3:
        return std::get<uint64_t>(value);
      case 4:
        return static_cast<uint64_t>(std::get<double>(value));
      default:
        return 0;
      }
    }

    operator double() const noexcept
    {
      switch (value.index())
      {
      case 1:
        return std::get<bool>(value) ? 1.0 : 0.0;
      case 2:
        return static_cast<double>(std::get<int64_t>(value));
      case 3:
        return static_cast<double>(std::get<uint64_t>(value));
      case 4:
        return std::get<double>(value);
      default:
        return 0.0;
      }
    }

    operator std::string() const noexcept
    {
      switch (value.index())
      {
      case 1:
        return std::get<bool>(value) ? "true" : "false";
      case 2:
        return std::to_string(std::get<int64_t>(value));
      case 3:
        return std::to_string(std::get<uint64_t>(value));
      case 4:
        return std::to_string(std::get<double>(value));
      case 5:
        return std::get<std::string>(value);
      default:
        return "";
      }
    }

    operator std::map<std::string, json>() const noexcept { return value.index() == 6 ? std::get<std::map<std::string, json>>(value) : std::map<std::string, json>(); }
    const std::map<std::string, json> &as_object() const noexcept { return std::get<std::map<std::string, json>>(value); }

    operator std::vector<json>() const noexcept { return value.index() == 7 ? std::get<std::vector<json>>(value) : std::vector<json>(); }
    const std::vector<json> &as_array() const noexcept { return std::get<std::vector<json>>(value); }

    void clear() noexcept { value = nullptr; }

    void push_back(const json &val) { std::get<std::vector<json>>(value).push_back(val); }
    void push_back(json &&val) { std::get<std::vector<json>>(value).push_back(std::move(val)); }

    void erase(const std::string &key) { std::get<std::map<std::string, json>>(value).erase(key); }

    void erase(size_t index) { std::get<std::vector<json>>(value).erase(std::get<std::vector<json>>(value).begin() + index); }

    std::string dump() const
    {
      switch (value.index())
      {
      case 0:
        return "null";
      case 1:
        return std::get<bool>(value) ? "true" : "false";
      case 2:
        return std::to_string(std::get<int64_t>(value));
      case 3:
        return std::to_string(std::get<uint64_t>(value));
      case 4:
        return std::to_string(std::get<double>(value));
      case 5:
        return "\"" + std::get<std::string>(value) + "\"";
      case 6:
      {
        std::string str = "{";
        const auto &m = std::get<std::map<std::string, json>>(value);
        for (auto it = m.begin(); it != m.end(); ++it)
        {
          if (it != m.begin())
            str += ",";
          str += "\"" + it->first + "\":" + it->second.dump();
        }
        return str + "}";
      }
      case 7:
      {
        std::string str = "[";
        const auto &v = std::get<std::vector<json>>(value);
        for (size_t i = 0; i < v.size(); ++i)
        {
          if (i != 0)
            str += ",";
          str += v[i].dump();
        }
        return str + "]";
      }
      default:
        return "";
      }
    }

    friend std::ostream &operator<<(std::ostream &os, const json &j) { return os << j.dump(); }

  private:
    std::variant<std::nullptr_t, bool, int64_t, uint64_t, double, std::string, std::map<std::string, json>, std::vector<json>> value;
  };

  json load(std::istream &is);

  json load(const char *str);

  inline json load(const std::string &str) { return load(str.c_str()); }
} // namespace json
