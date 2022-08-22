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
  class int_val;
  class float_val;
  class null_val;
  class object;
  class array;

  class json
  {
    friend class object;
    friend class array;

  public:
    JSON_EXPORT json();
    JSON_EXPORT json(json &&orig);
    JSON_EXPORT json(object &&orig);
    JSON_EXPORT json(array &&orig);
    JSON_EXPORT json(bool_val &&orig);
    JSON_EXPORT json(string_val &&orig);
    JSON_EXPORT json(int_val &&orig);
    JSON_EXPORT json(float_val &&orig);
    JSON_EXPORT json(null_val &&orig);

    JSON_EXPORT void operator=(bool val);
    JSON_EXPORT void operator=(const std::string &str);
    JSON_EXPORT void operator=(const char *str);
    JSON_EXPORT void operator=(long val);
    JSON_EXPORT void operator=(double val);
    JSON_EXPORT void operator=(json val);

    virtual json &operator[](const std::string &str);
    virtual json &operator[](size_t index);

    JSON_EXPORT std::string dump() const noexcept;

  private:
    virtual void dump(std::ostream &os) const noexcept { j->dump(os); }

  private:
    std::unique_ptr<json> j;
  };

  class object : public json
  {
    friend class json;

  public:
    JSON_EXPORT object();
    JSON_EXPORT object(std::map<std::string, json> &&vals);

    json &operator[](const std::string &str) override { return vals[str]; }

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

  private:
    JSON_EXPORT void dump(std::ostream &os) const noexcept override;

  private:
    std::vector<json> vals;
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

  class int_val : public json
  {
    friend class json;

  public:
    JSON_EXPORT int_val(const long &val);

    operator long() const { return val; }

  private:
    JSON_EXPORT void dump(std::ostream &os) const noexcept override;

  private:
    long val;
  };

  class float_val : public json
  {
    friend class json;

  public:
    JSON_EXPORT float_val(const double &val);

    operator double() const { return val; }

  private:
    JSON_EXPORT void dump(std::ostream &os) const noexcept override;

  private:
    double val;
  };

  class null_val : public json
  {
  public:
    JSON_EXPORT null_val() {}

  private:
    JSON_EXPORT void dump(std::ostream &os) const noexcept override { os << "null"; }
  };

  JSON_EXPORT json load(std::istream &is);
} // namespace json
