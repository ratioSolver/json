#pragma once

#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <type_traits>

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
  private:
    json(std::map<std::string, json> &obj) : type(json_type::object), obj_val(std::move(obj)) {}
    json(std::vector<json> &arr) : type(json_type::array), arr_val(std::move(arr)) {}

  public:
    json(json_type type = json_type::object) : type(type) {}
    json(std::nullptr_t) : type(json_type::null) {}
    json(const std::string &str, bool is_number = false) : type(is_number ? json_type::number : json_type::string), str_val(str) {}
    json(const char *str, bool is_number = false) : type(is_number ? json_type::number : json_type::string), str_val(str) {}
    json(bool b) : type(json_type::boolean), bool_val(b) {}
    template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    json(T t) : type(json_type::number), str_val(std::to_string(t)) {}
    json(std::map<std::string, json> &&obj) : type(json_type::object), obj_val(std::move(obj)) {}
    json(std::vector<json> &&arr) : type(json_type::array), arr_val(std::move(arr)) {}
    json(std::initializer_list<json> list) : type(json_type::array)
    {
      if (list.size() == 2 && list.begin()->type == json_type::string)
      {
        type = json_type::object;
        obj_val.emplace(list.begin()->str_val, *(list.begin() + 1));
      }
      else if (list.begin()->type == json_type::object)
      {
        type = json_type::object;
        for (auto &p : list)
          for (auto &q : p.obj_val)
            obj_val.emplace(q.first, q.second);
      }
      else
        for (auto &p : list)
          arr_val.emplace_back(p);
    }
    json(const json &other) : type(other.type), bool_val(other.bool_val), str_val(other.str_val), obj_val(other.obj_val), arr_val(other.arr_val) {}
    json(json &&other) : type(other.type), bool_val(other.bool_val), str_val(std::move(other.str_val)), obj_val(std::move(other.obj_val)), arr_val(std::move(other.arr_val)) { other.type = json_type::null; }

    json &operator=(const json &other)
    {
      type = other.type;
      bool_val = other.bool_val;
      str_val = other.str_val;
      obj_val = other.obj_val;
      arr_val = other.arr_val;
      return *this;
    }
    json &operator=(json &&other)
    {
      type = other.type;
      bool_val = other.bool_val;
      str_val = std::move(other.str_val);
      obj_val = std::move(other.obj_val);
      arr_val = std::move(other.arr_val);
      return *this;
    }
    json &operator=(const std::string &str)
    {
      set_type(json_type::string);
      str_val = str;
      return *this;
    }
    json &operator=(const char *str)
    {
      set_type(json_type::string);
      str_val = str;
      return *this;
    }
    json &operator=(bool b)
    {
      set_type(json_type::boolean);
      bool_val = b;
      return *this;
    }
    template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    json &operator=(T t)
    {
      set_type(json_type::number);
      str_val = std::to_string(t);
      return *this;
    }
    json &operator=(std::nullptr_t)
    {
      set_type(json_type::null);
      return *this;
    }

    bool has(const std::string &key) const { return type == json_type::object && obj_val.find(key) != obj_val.end(); }
    bool has(const char *key) const { return type == json_type::object && obj_val.find(key) != obj_val.end(); }
    bool has(size_t index) const { return type == json_type::array && index < arr_val.size(); }

    json &operator[](const std::string &key)
    {
      set_type(json_type::object);
      return obj_val[key];
    }
    const json &operator[](const std::string &key) const { return obj_val.at(key); }
    json &operator[](const char *key)
    {
      set_type(json_type::object);
      return obj_val[key];
    }
    const json &operator[](const char *key) const { return obj_val.at(key); }
    json &operator[](int index)
    {
      set_type(json_type::array);
      return arr_val[index];
    }
    const json &operator[](int index) const { return arr_val[index]; }
    json &operator[](size_t index)
    {
      set_type(json_type::array);
      return arr_val[index];
    }
    const json &operator[](size_t index) const { return arr_val.at(index); }

    bool operator==(const json &other) const
    {
      if (type != other.type)
        return false;
      switch (type)
      {
      case json_type::null:
        return true;
      case json_type::string:
        return str_val == other.str_val;
      case json_type::number:
        return str_val == other.str_val;
      case json_type::boolean:
        return bool_val == other.bool_val;
      case json_type::array:
        return arr_val == other.arr_val;
      case json_type::object:
        return obj_val == other.obj_val;
      default:
        return false;
      }
    }
    bool operator!=(const json &other) const { return !(*this == other); }

    bool operator==(const std::string &str) const { return type == json_type::string && str_val == str; }
    bool operator!=(const std::string &str) const { return !(*this == str); }
    bool operator==(const char *str) const { return type == json_type::string && str_val == str; }
    bool operator!=(const char *str) const { return !(*this == str); }
    bool operator==(bool b) const { return type == json_type::boolean && bool_val == b; }
    bool operator!=(bool b) const { return type != json_type::boolean || bool_val != b; }
    bool operator==(int i) const { return type == json_type::number && std::stoi(str_val) == i; }
    bool operator!=(int i) const { return !(*this == i); }
    bool operator==(double d) const { return type == json_type::number && std::stod(str_val) == d; }
    bool operator!=(double d) const { return !(*this == d); }
    bool operator==(long l) const { return type == json_type::number && std::stol(str_val) == l; }
    bool operator!=(long l) const { return !(*this == l); }
    bool operator==(unsigned long l) const { return type == json_type::number && std::stoul(str_val) == l; }
    bool operator!=(unsigned long l) const { return !(*this == l); }
    bool operator==(std::nullptr_t) const { return type == json_type::null; }
    bool operator!=(std::nullptr_t) const { return type != json_type::null; }

    json_type get_type() const { return type; }
    size_t size() const
    {
      switch (type)
      {
      case json_type::null:
        return 0;
      case json_type::string:
        return str_val.size();
      case json_type::number:
        return str_val.size();
      case json_type::boolean:
        return 1;
      case json_type::array:
        return arr_val.size();
      case json_type::object:
        return obj_val.size();
      default:
        return 0;
      }
    }

    operator bool() const
    {
      switch (type)
      {
      case json_type::null:
        return false;
      case json_type::string:
        return !str_val.empty();
      case json_type::number:
        return str_val != "0";
      case json_type::boolean:
        return bool_val;
      case json_type::array:
        return !arr_val.empty();
      case json_type::object:
        return !obj_val.empty();
      default:
        return false;
      }
    }
    operator int() const
    {
      switch (type)
      {
      case json_type::null:
        return 0;
      case json_type::string:
        return std::stoi(str_val);
      case json_type::number:
        return std::stoi(str_val);
      case json_type::boolean:
        return bool_val ? 1 : 0;
      case json_type::array:
        return static_cast<int>(arr_val.size());
      case json_type::object:
        return static_cast<int>(obj_val.size());
      default:
        return 0;
      }
    }
    operator double() const
    {
      switch (type)
      {
      case json_type::null:
        return 0;
      case json_type::string:
        return std::stod(str_val);
      case json_type::number:
        return std::stod(str_val);
      case json_type::boolean:
        return bool_val ? 1 : 0;
      case json_type::array:
        return static_cast<double>(arr_val.size());
      case json_type::object:
        return static_cast<double>(obj_val.size());
      default:
        return 0;
      }
    }
    operator long() const
    {
      switch (type)
      {
      case json_type::null:
        return 0;
      case json_type::string:
        return std::stol(str_val);
      case json_type::number:
        return std::stol(str_val);
      case json_type::boolean:
        return bool_val ? 1 : 0;
      case json_type::array:
        return static_cast<long>(arr_val.size());
      case json_type::object:
        return static_cast<long>(obj_val.size());
      default:
        return 0;
      }
    }
    operator unsigned long() const
    {
      switch (type)
      {
      case json_type::null:
        return 0;
      case json_type::string:
        return std::stoul(str_val);
      case json_type::number:
        return std::stoul(str_val);
      case json_type::boolean:
        return bool_val ? 1 : 0;
      case json_type::array:
        return static_cast<unsigned long>(arr_val.size());
      case json_type::object:
        return static_cast<unsigned long>(obj_val.size());
      default:
        return 0;
      }
    }
    operator std::string() const
    {
      switch (type)
      {
      case json_type::null:
        return "";
      case json_type::string:
        return str_val;
      case json_type::number:
        return str_val;
      case json_type::boolean:
        return bool_val ? "true" : "false";
      case json_type::array:
        return std::to_string(arr_val.size());
      case json_type::object:
        return std::to_string(obj_val.size());
      default:
        return "";
      }
    }
    operator std::map<std::string, json>() const
    {
      switch (type)
      {
      case json_type::null:
        return {};
      case json_type::string:
        return {};
      case json_type::number:
        return {};
      case json_type::boolean:
        return {};
      case json_type::array:
        return {};
      case json_type::object:
        return obj_val;
      default:
        return {};
      }
    }
    operator std::vector<json>() const
    {
      switch (type)
      {
      case json_type::null:
        return {};
      case json_type::string:
        return {};
      case json_type::number:
        return {};
      case json_type::boolean:
        return {};
      case json_type::array:
        return arr_val;
      case json_type::object:
        return {};
      default:
        return {};
      }
    }

    std::map<std::string, json> &get_object()
    {
      set_type(json_type::object);
      return obj_val;
    }
    const std::map<std::string, json> &get_object() const { return obj_val; }
    std::vector<json> &get_array()
    {
      set_type(json_type::array);
      return arr_val;
    }
    const std::vector<json> &get_array() const { return arr_val; }

    void set(const std::string &key, json &&j)
    {
      set_type(json_type::object);
      obj_val[key] = std::move(j);
    }

    void push_back(json &&j)
    {
      set_type(json_type::array);
      arr_val.push_back(std::move(j));
    }

    std::string to_string() const
    {
      std::stringstream ss;
      dump(ss);
      return ss.str();
    }

    friend std::ostream &operator<<(std::ostream &os, const json &j)
    {
      j.dump(os);
      return os;
    }

  private:
    void set_type(json_type tp)
    {
      if (tp != type)
      {
        bool_val = false;
        str_val = tp == json_type::string ? "" : "0";
        obj_val.clear();
        arr_val.clear();
        type = tp;
      }
    }

    void dump(std::ostream &os) const
    {
      switch (type)
      {
      case json_type::null:
        os << "null";
        break;
      case json_type::string:
        os << "\"" << str_val << "\"";
        break;
      case json_type::number:
        os << str_val;
        break;
      case json_type::boolean:
        os << (bool_val ? "true" : "false");
        break;
      case json_type::array:
        os << "[";
        for (size_t i = 0; i < arr_val.size(); i++)
        {
          arr_val[i].dump(os);
          if (i != arr_val.size() - 1)
            os << ",";
        }
        os << "]";
        break;
      case json_type::object:
        os << "{";
        for (auto it = obj_val.begin(); it != obj_val.end(); it++)
        {
          os << "\"" << it->first << "\":";
          it->second.dump(os);
          if (it != --obj_val.end())
            os << ",";
        }
        os << "}";
        break;
      }
    }

  private:
    json_type type;
    bool bool_val = false;
    std::string str_val;
    std::map<std::string, json> obj_val;
    std::vector<json> arr_val;
  };

  /**
   * Loads a JSON object from the specified input stream.
   *
   * @param is The input stream to read the JSON object from.
   * @return The loaded JSON object.
   */
  json load(std::istream &is);
  /**
   * Loads a JSON object from a C-style string.
   *
   * @param str The C-style string containing the JSON data.
   * @return A JSON object representing the parsed data.
   */
  inline json load(const char *str)
  {
    std::stringstream ss;
    ss << str;
    return load(ss);
  }
  /**
   * Loads a JSON object from a string.
   *
   * @param str The string containing the JSON data.
   * @return The loaded JSON object.
   */
  inline json load(const std::string &str)
  {
    std::stringstream ss;
    ss << str;
    return load(ss);
  }
  /**
   * Parses a string from the given input stream.
   *
   * @param is The input stream to read the string from.
   * @return The parsed string.
   */
  std::string parse_string(std::istream &is);
} // namespace json
