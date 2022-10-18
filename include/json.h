#pragma once

#include "json_export.h"
#include <map>
#include <memory>
#include <vector>
#include <string>
#include <iostream>

namespace json
{
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

  public:
    JSON_EXPORT json();
    JSON_EXPORT json(bool val);
    JSON_EXPORT json(const std::string &str);
    JSON_EXPORT json(const char *str);
    JSON_EXPORT json(short int val);
    JSON_EXPORT json(unsigned short int val);
    JSON_EXPORT json(long val);
    JSON_EXPORT json(long long val);
    JSON_EXPORT json(unsigned long val);
    JSON_EXPORT json(unsigned long long val);
    JSON_EXPORT json(double val);
    JSON_EXPORT json(long double val);
    JSON_EXPORT json(json &&orig);
    JSON_EXPORT json(object &&orig);
    JSON_EXPORT json(array &&orig);
    JSON_EXPORT json(bool_val &&orig);
    JSON_EXPORT json(string_val &&orig);
    JSON_EXPORT json(number_val &&orig);
    JSON_EXPORT json(null_val &&orig);

    JSON_EXPORT void operator=(bool val);
    JSON_EXPORT void operator=(const std::string &str);
    JSON_EXPORT void operator=(const char *str);
    JSON_EXPORT void operator=(short int val);
    JSON_EXPORT void operator=(unsigned short int val);
    JSON_EXPORT void operator=(long val);
    JSON_EXPORT void operator=(long long val);
    JSON_EXPORT void operator=(unsigned long val);
    JSON_EXPORT void operator=(unsigned long long val);
    JSON_EXPORT void operator=(double val);
    JSON_EXPORT void operator=(long double val);
    JSON_EXPORT void operator=(json val);

    virtual json &operator[](const std::string &str);
    virtual json &operator[](size_t index);

    JSON_EXPORT operator bool_val &() const;
    JSON_EXPORT operator string_val &() const;
    JSON_EXPORT operator number_val &() const;
    JSON_EXPORT operator array &() const;
    JSON_EXPORT operator object &() const;

    JSON_EXPORT std::string dump() const noexcept;

  private:
    virtual void dump(std::ostream &os) const noexcept { j->dump(os); }

  private:
    std::unique_ptr<json> j;
  };

  class null_val : public json
  {
  public:
    JSON_EXPORT null_val() {}

  private:
    JSON_EXPORT void dump(std::ostream &os) const noexcept override { os << "null"; }
  };

  class bool_val : public json
  {
    friend class json;

  public:
    JSON_EXPORT bool_val(const bool &val);

    operator bool() const { return val; }

  private:
    JSON_EXPORT void dump(std::ostream &os) const noexcept override;

  private:
    bool val;
  };

  class string_val : public json
  {
    friend class json;

  public:
    JSON_EXPORT string_val(const std::string &val);

    operator std::string() const { return val; }

  private:
    JSON_EXPORT void dump(std::ostream &os) const noexcept override;

  private:
    std::string val;
  };

  class number_val : public json
  {
    friend class json;

  public:
    JSON_EXPORT number_val(const std::string &val);

    operator unsigned long() const { return std::stoul(val); }
    operator long() const { return std::stol(val); }
    operator double() const { return std::stod(val); }

  private:
    JSON_EXPORT void dump(std::ostream &os) const noexcept override;

  private:
    std::string val;
  };

  class object : public json
  {
    friend class json;

  public:
    JSON_EXPORT object();
    JSON_EXPORT object(std::map<std::string, json> &&vals);

    bool has(const std::string &str) const { return vals.count(str); }
    json &operator[](const std::string &str) override { return vals[str]; }

    auto cbegin() const { return vals.begin(); }
    auto cend() const { return vals.end(); }
    auto begin() const { return vals.begin(); }
    auto end() const { return vals.end(); }

  private:
    JSON_EXPORT void dump(std::ostream &os) const noexcept override;

  private:
    std::map<std::string, json> vals;
  };

  class array : public json
  {
    friend class json;

  public:
    JSON_EXPORT array();
    JSON_EXPORT array(std::vector<json> &&vs);

    inline void reserve(size_t size) { vals.reserve(size); }

    size_t size() { return vals.size(); }
    json &operator[](size_t index) override
    {
      if (vals.size() < index + 1)
        vals.resize(index + 1);
      return vals[index];
    }

    auto cbegin() const { return vals.begin(); }
    auto cend() const { return vals.end(); }
    auto begin() const { return vals.begin(); }
    auto end() const { return vals.end(); }

    void push_back(json &&val) { vals.push_back(std::move(val)); }

  private:
    JSON_EXPORT void dump(std::ostream &os) const noexcept override;

  private:
    std::vector<json> vals;
  };

  JSON_EXPORT json load(std::istream &is);
} // namespace json
