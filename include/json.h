#pragma once

#include "json_export.h"
#include "memory.h"
#include <map>
#include <vector>
#include <sstream>

namespace json
{
  enum class json_type
  {
    null,
    boolean,
    string,
    number,
    object,
    array
  };

  class bool_val;
  class string_val;
  class number_val;
  class null_val;
  class object;
  class array;

  class json
  {
    friend class object;
    friend class array;

  protected:
    json(json_type tp) : m_type(tp), m_root(nullptr) {}

  public:
    JSON_EXPORT json();
    JSON_EXPORT json(bool val);
    JSON_EXPORT json(const char *val);
    JSON_EXPORT json(const std::string &val);
    JSON_EXPORT json(long val);
    JSON_EXPORT json(unsigned long val);
    JSON_EXPORT json(double val);
    JSON_EXPORT json(std::nullptr_t);
    JSON_EXPORT json(bool_val &&other);
    JSON_EXPORT json(string_val &&other);
    JSON_EXPORT json(number_val &&other);
    JSON_EXPORT json(null_val &&other);
    JSON_EXPORT json(object &&other);
    JSON_EXPORT json(array &&other);
    JSON_EXPORT json(json &&other);
    JSON_EXPORT json(std::map<std::string, json> &&other);
    JSON_EXPORT json(std::vector<json> &&other);

    virtual ~json() = default;

    JSON_EXPORT operator bool_val &() const;
    JSON_EXPORT void operator=(bool val);
    JSON_EXPORT operator bool() const;
    JSON_EXPORT bool operator==(bool val) const;

    JSON_EXPORT operator string_val &() const;
    JSON_EXPORT void operator=(const char *val);
    JSON_EXPORT operator const char *() const;
    JSON_EXPORT operator std::string() const;
    JSON_EXPORT bool operator==(const char *val) const;

    JSON_EXPORT operator number_val &() const;
    JSON_EXPORT void operator=(long val);
    JSON_EXPORT operator long() const;
    JSON_EXPORT bool operator==(long val) const;

    JSON_EXPORT void operator=(unsigned long val);
    JSON_EXPORT operator unsigned long() const;
    JSON_EXPORT bool operator==(unsigned long val) const;

    JSON_EXPORT void operator=(double val);
    JSON_EXPORT operator double() const;
    JSON_EXPORT bool operator==(double val) const;

    JSON_EXPORT operator null_val &() const;
    JSON_EXPORT void operator=(std::nullptr_t);
    JSON_EXPORT operator std::nullptr_t() const;
    JSON_EXPORT bool operator==(std::nullptr_t) const;

    JSON_EXPORT operator object &() const;
    JSON_EXPORT void operator=(object &&val);
    JSON_EXPORT operator const object &() const;
    JSON_EXPORT bool operator==(const object &val) const;

    JSON_EXPORT operator array &() const;
    JSON_EXPORT void operator=(array &&val);
    JSON_EXPORT operator const array &() const;
    JSON_EXPORT bool operator==(const array &val) const;

    JSON_EXPORT bool operator==(const json &other) const;
    bool operator!=(const json &other) const { return !(*this == other); }

    json_type type() const { return m_type; }

    json &operator[](const char *key);
    const json &operator[](const char *key) const { return operator[](key); }

    json &operator[](size_t index);
    const json &operator[](size_t index) const { return operator[](index); }

    JSON_EXPORT std::string dump() const noexcept;

    friend std::ostream &operator<<(std::ostream &os, const json &val)
    {
      val.dump(os);
      return os;
    }

  private:
    virtual void dump(std::ostream &os) const noexcept { m_root->dump(os); }

  private:
    json_type m_type;
    utils::u_ptr<json> m_root;
  };

  class bool_val : public json
  {
    friend class json;
    friend class array;

  public:
    bool_val(bool val) : json(json_type::boolean), m_val(val) {}

    void set(bool val) { m_val = val; }

    bool operator==(bool val) const { return m_val == val; }
    bool operator==(const json &val) const
    {
      if (val.type() != json_type::boolean)
        return false;

      return m_val == static_cast<bool_val &>(val).m_val;
    }
    operator bool() const { return m_val; }

  private:
    void dump(std::ostream &os) const noexcept override { os << (m_val ? "true" : "false"); }

  private:
    bool m_val;
  };

  class string_val : public json
  {
    friend class json;
    friend class array;

  public:
    string_val(const std::string &val) : json(json_type::string), m_val(val) {}

    void set(const char *val) { m_val = val; }

    bool operator==(const char *val) const { return std::string(m_val) == val; }
    bool operator==(const json &val) const
    {
      if (val.type() != json_type::string)
        return false;

      return m_val == static_cast<string_val &>(val).m_val;
    }

  private:
    void dump(std::ostream &os) const noexcept override { os << '"' << m_val << '"'; }

  private:
    std::string m_val;
  };

  class number_val : public json
  {
    friend class json;
    friend class array;

  public:
    number_val(const std::string &val) : json(json_type::number), m_val(val) {}

    void set(const std::string &val) { m_val = val; }

    bool operator==(const json &val) const
    {
      if (val.type() != json_type::number)
        return false;

      return m_val == static_cast<number_val &>(val).m_val;
    }

  private:
    void dump(std::ostream &os) const noexcept override { os << m_val; }

  private:
    std::string m_val;
  };

  class null_val : public json
  {
  public:
    null_val() : json(json_type::null) {}

  private:
    void dump(std::ostream &os) const noexcept override { os << "null"; }
  };

  class object : public json
  {
    friend class json;
    friend class array;

  public:
    object() : json(json_type::object) {}
    object(std::map<std::string, json> &&map) : json(json_type::object), m_map(std::move(map)) {}

    bool has(const char *key) const { return m_map.find(key) != m_map.end(); }

    auto begin() { return m_map.begin(); }
    auto end() { return m_map.end(); }
    auto begin() const { return m_map.begin(); }
    auto end() const { return m_map.end(); }

    bool operator==(const object &other) const
    {
      if (m_map.size() != other.m_map.size())
        return false;

      for (auto &pair : m_map)
      {
        auto it = other.m_map.find(pair.first);
        if (it == other.m_map.end())
          return false;

        if (pair.second != it->second)
          return false;
      }

      return true;
    }

    json &operator[](const char *key) { return m_map[key]; }

  private:
    void dump(std::ostream &os) const noexcept override
    {
      os << '{';
      for (auto it = m_map.begin(); it != m_map.end(); ++it)
      {
        if (it != m_map.begin())
          os << ',';

        os << '"' << it->first << "\":";
        it->second.dump(os);
      }
      os << '}';
    }

  private:
    std::map<std::string, json> m_map;
  };

  class array : public json
  {
    friend class json;

  public:
    array() : json(json_type::array) {}
    array(std::vector<json> &&array) : json(json_type::array), m_array(std::move(array)) {}

    size_t size() const { return m_array.size(); }
    void push_back(json &&val) { m_array.emplace_back(std::move(val)); }
    void set(size_t index, json &&val)
    {
      switch (val.m_type)
      {
      case json_type::null:
        break;
      case json_type::boolean:
        if (val.m_root)
          m_array[index].m_root.reset(new bool_val(static_cast<bool_val &>(*val.m_root).m_val));
        else
          m_array[index].m_root.reset(new bool_val(static_cast<bool_val &>(val).m_val));
        break;
      case json_type::string:
        if (val.m_root)
          m_array[index].m_root.reset(new string_val(static_cast<string_val &>(*val.m_root).m_val));
        else
          m_array[index].m_root.reset(new string_val(static_cast<string_val &>(val).m_val));
        break;
      case json_type::number:
        if (val.m_root)
          m_array[index].m_root.reset(new number_val(static_cast<number_val &>(*val.m_root).m_val));
        else
          m_array[index].m_root.reset(new number_val(static_cast<number_val &>(val).m_val));
        break;
      case json_type::object:
        if (val.m_root)
          m_array[index].m_root.reset(new object(std::move(static_cast<object &>(*val.m_root).m_map)));
        else
          m_array[index].m_root.reset(new object(std::move(static_cast<object &>(val).m_map)));
        break;
      case json_type::array:
        if (val.m_root)
          m_array[index].m_root.reset(new array(std::move(static_cast<array &>(*val.m_root).m_array)));
        else
          m_array[index].m_root.reset(new array(std::move(static_cast<array &>(val).m_array)));
        break;
      }
    }

    auto begin() { return m_array.begin(); }
    auto end() { return m_array.end(); }
    auto begin() const { return m_array.begin(); }
    auto end() const { return m_array.end(); }

    bool operator==(const array &other) const
    {
      if (m_array.size() != other.m_array.size())
        return false;

      for (size_t i = 0; i < m_array.size(); ++i)
        if (m_array[i] != other.m_array[i])
          return false;

      return true;
    }

    json &operator[](size_t index) { return m_array[index]; }

  private:
    void dump(std::ostream &os) const noexcept override
    {
      os << '[';
      for (auto it = m_array.begin(); it != m_array.end(); ++it)
      {
        if (it != m_array.begin())
          os << ',';

        it->dump(os);
      }
      os << ']';
    }

  private:
    std::vector<json> m_array;
  };

  JSON_EXPORT json load(std::istream &is);
  JSON_EXPORT json load(const char *str)
  {
    std::stringstream ss;
    ss << str;
    return load(ss);
  }
  JSON_EXPORT json load(const std::string &str)
  {
    std::stringstream ss;
    ss << str;
    return load(ss);
  }
} // namespace json
