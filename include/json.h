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
    /**
     * @brief Construct a new json object.
     *
     */
    JSON_EXPORT json();
    /**
     * @brief Construct a new json object from a boolean value.
     *
     * @param val the value to construct the json object from.
     */
    JSON_EXPORT json(bool val);
    /**
     * @brief Construct a new json object from a string.
     *
     * @param str the string to construct the json object from.
     */
    JSON_EXPORT json(const std::string &str);
    /**
     * @brief Construct a new json object from a string.
     *
     * @param str the string to construct the json object from.
     */
    JSON_EXPORT json(const char *str);
    /**
     * @brief Construct a new json object from a short int.
     *
     * @param val the short int to construct the json object from.
     */
    JSON_EXPORT json(short int val);
    /**
     * @brief Construct a new json object from an unsigned short int.
     *
     * @param val the unsigned short int to construct the json object from.
     */
    JSON_EXPORT json(unsigned short int val);
    /**
     * @brief Construct a new json object from a long.
     *
     * @param val the long to construct the json object from.
     */
    JSON_EXPORT json(long val);
    /**
     * @brief Construct a new json object from a long long.
     *
     * @param val the long long to construct the json object from.
     */
    JSON_EXPORT json(long long val);
    /**
     * @brief Construct a new json object from an unsigned long.
     *
     * @param val the unsigned long to construct the json object from.
     */
    JSON_EXPORT json(unsigned long val);
    /**
     * @brief Construct a new json object from an unsigned long long.
     *
     * @param val the unsigned long long to construct the json object from.
     */
    JSON_EXPORT json(unsigned long long val);
    /**
     * @brief Construct a new json object from a double.
     *
     * @param val the double to construct the json object from.
     */
    JSON_EXPORT json(double val);
    /**
     * @brief Construct a new json object from a long double.
     *
     * @param val the long double to construct the json object from.
     */
    JSON_EXPORT json(long double val);
    /**
     * @brief Construct a new json object from a json object.
     *
     * @param orig the json object to construct the json object from.
     */
    JSON_EXPORT json(json &&orig);
    /**
     * @brief Construct a new json object from an object.
     *
     * @param orig the object to construct the json object from.
     */
    JSON_EXPORT json(object &&orig);
    /**
     * @brief Construct a new json object from an array.
     *
     * @param orig the array to construct the json object from.
     */
    JSON_EXPORT json(array &&orig);
    /**
     * @brief Construct a new json object from a bool_val.
     *
     * @param orig the bool_val to construct the json object from.
     */
    JSON_EXPORT json(bool_val &&orig);
    /**
     * @brief Construct a new json object from a string_val.
     *
     * @param orig the string_val to construct the json object from.
     */
    JSON_EXPORT json(string_val &&orig);
    /**
     * @brief Construct a new json object from a number_val.
     *
     * @param orig the number_val to construct the json object from.
     */
    JSON_EXPORT json(number_val &&orig);
    /**
     * @brief Construct a new json object from a null_val.
     *
     * @param orig the null_val to construct the json object from.
     */
    JSON_EXPORT json(null_val &&orig);
    virtual ~json() = default;

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

    virtual JSON_EXPORT json &operator[](const std::string &str);
    virtual JSON_EXPORT json &operator[](size_t index);

    JSON_EXPORT operator bool_val &() const;
    JSON_EXPORT operator string_val &() const;
    JSON_EXPORT operator number_val &() const;
    JSON_EXPORT operator array &() const;
    JSON_EXPORT operator object &() const;

    /**
     * @brief Dump the json object to a string.
     *
     * @return std::string the json object as a string.
     */
    JSON_EXPORT std::string dump() const noexcept;

  private:
    /**
     * @brief Dump the json object to a stream.
     *
     * @param os the stream to dump to.
     */
    virtual void dump(std::ostream &os) const noexcept { j->dump(os); }

  private:
    std::unique_ptr<json> j;
  };

  class null_val : public json
  {
  public:
    null_val() {}

  private:
    void dump(std::ostream &os) const noexcept override { os << "null"; }
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
    bool val; // the value of the bool object.
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
    std::string val; // the value of the string object.
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
    std::string val; // the value of the number object.
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
    auto begin() { return vals.begin(); }
    auto end() { return vals.end(); }

  private:
    JSON_EXPORT void dump(std::ostream &os) const noexcept override;

  private:
    std::map<std::string, json> vals; // the values of the object indexed by key.
  };

  class array : public json
  {
    friend class json;

  public:
    JSON_EXPORT array();
    JSON_EXPORT array(std::vector<json> &&vs);

    inline void reserve(size_t size) { vals.reserve(size); }

    size_t size() const { return vals.size(); }
    json &operator[](size_t index) override
    {
      if (vals.size() < index + 1)
        vals.resize(index + 1);
      return vals[index];
    }
    const json &at(const size_t index) const { return vals.at(index); }

    auto cbegin() const { return vals.begin(); }
    auto cend() const { return vals.end(); }
    auto begin() { return vals.begin(); }
    auto end() { return vals.end(); }

    void push_back(json &&val) { vals.push_back(std::move(val)); }

  private:
    JSON_EXPORT void dump(std::ostream &os) const noexcept override;

  private:
    std::vector<json> vals; // the values of the array.
  };

  JSON_EXPORT json load(const char *str);
  JSON_EXPORT json load(const std::string &str);
  JSON_EXPORT json load(std::istream &is);
} // namespace json
