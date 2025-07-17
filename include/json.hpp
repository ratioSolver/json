#pragma once

#include <string>
#include <string_view>
#include <variant>
#include <map>
#include <vector>
#include <cstdint>
#include <iostream>

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

  /**
   * @brief A class representing a JSON value.
   *
   * The `json` class is a versatile container that can hold different types of JSON values, including null, boolean, number, string, array, and object.
   * It provides various member functions and overloaded operators for accessing and manipulating JSON values.
   */
  class json
  {
  public:
    /**
     * @brief Constructs a JSON object with the specified type.
     *
     * This constructor initializes a JSON object with the specified type. The type can be one of the following:
     * - json_type::null: Initializes the JSON object with a null value.
     * - json_type::boolean: Initializes the JSON object with a boolean value (false).
     * - json_type::number: Initializes the JSON object with a numeric value (0).
     * - json_type::string: Initializes the JSON object with an empty string.
     * - json_type::array: Initializes the JSON object with an empty array.
     * - json_type::object: Initializes the JSON object with an empty object.
     *
     * @param type The type of the JSON object to be constructed.
     */
    json(json_type type = json_type::object) noexcept : value(nullptr)
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
        value = static_cast<int64_t>(0);
        break;
      case json_type::string:
        value = std::string();
        break;
      case json_type::array:
        value = std::vector<json>();
        break;
      case json_type::object:
        value = std::map<std::string, json, std::less<>>();
        break;
      }
    }

    /**
     * @brief Copy constructor for the json class.
     *
     * This constructor creates a new json object as a copy of another json object.
     *
     * @param other The json object to be copied.
     */
    json(const json &other) : value(other.value) {}

    /**
     * @brief Constructs a JSON object from a value of type T.
     *
     * This constructor allows you to create a `json` object from any type that can be converted to a JSON value.
     * The type T must be convertible to one of the supported JSON types (null, boolean, number, string, array, or object).
     *
     * @param v The value to be converted to a JSON object.
     */
    template <typename T, std::enable_if_t<!std::is_same_v<std::decay_t<T>, json>, int> = 0>
    json(T &&v) : value(std::forward<T>(v)) {}

    /**
     * @brief Constructs a `json` object from an initializer list of `json` objects.
     *
     * This constructor allows you to create a `json` object by providing an initializer list
     * of `json` objects. The elements in the initializer list will be used to initialize the
     * `json` object in the same order as they appear in the initializer list.
     *
     * @param init An initializer list of `json` objects.
     */
    json(std::initializer_list<json> init);

    /**
     * @brief Assignment operator for the json class.
     *
     * This operator assigns the value of another json object to the current object.
     *
     * @param other The json object to be assigned.
     * @return A reference to the current json object after assignment.
     */
    json &operator=(const json &other) noexcept
    {
      value = other.value;
      return *this;
    }

    /**
     * @brief Assignment operator for the json class.
     *
     * This operator allows you to assign a value of type T to the current json object.
     * The type T must be convertible to one of the supported JSON types (null, boolean, number, string, array, or object).
     *
     * @param v The value to be assigned to the json object.
     * @return A reference to the current json object after assignment.
     */
    template <typename T, std::enable_if_t<!std::is_same_v<std::decay_t<T>, json>, int> = 0>
    json &operator=(T &&v)
    {
      value = std::forward<T>(v);
      return *this;
    }

    /**
     * @brief Accesses the JSON value associated with the given key.
     *
     * This operator allows access to the JSON object element corresponding to the specified key.
     * If the key does not exist, a new element is created and returned.
     *
     * @tparam Key Type of the key, which must be convertible to std::string.
     * @param key The key to access in the JSON object.
     * @return Reference to the JSON value associated with the key.
     */
    template <typename Key, std::enable_if_t<std::is_same_v<std::decay_t<Key>, const char *> || std::is_same_v<std::decay_t<Key>, std::string> || std::is_same_v<std::decay_t<Key>, std::string_view>, int> = 0>
    json &operator[](Key &&key) { return std::get<std::map<std::string, json, std::less<>>>(value)[std::string(std::forward<Key>(key))]; }

    /**
     * @brief Accesses the JSON value associated with the given key.
     *
     * This operator allows access to the JSON object element corresponding to the specified key.
     * If the key does not exist, an exception is thrown.
     *
     * @tparam Key Type of the key, which must be convertible to std::string.
     * @param key The key to access in the JSON object.
     * @return A constant reference to the JSON value associated with the key.
     */
    template <typename Key, std::enable_if_t<std::is_same_v<std::decay_t<Key>, const char *> || std::is_same_v<std::decay_t<Key>, std::string> || std::is_same_v<std::decay_t<Key>, std::string_view>, int> = 0>
    const json &operator[](Key &&key) const { return std::get<std::map<std::string, json, std::less<>>>(value).at(std::string(std::forward<Key>(key))); }

    /**
     * @brief Accesses the JSON value at the specified index.
     *
     * This operator allows access to the JSON array element at the specified index.
     * If the index is out of bounds, an exception is thrown.
     *
     * @tparam Index Type of the index, which must be an integral type.
     * @param idx The index to access in the JSON array.
     * @return Reference to the JSON value at the specified index.
     */
    template <typename Index, std::enable_if_t<std::is_integral_v<Index>, int> = 0>
    json &operator[](Index idx) { return std::get<std::vector<json>>(value)[static_cast<size_t>(idx)]; }

    /**
     * @brief Accesses the JSON value at the specified index.
     *
     * This operator allows access to the JSON array element at the specified index.
     * If the index is out of bounds, an exception is thrown.
     *
     * @tparam Index Type of the index, which must be an integral type.
     * @param idx The index to access in the JSON array.
     * @return A constant reference to the JSON value at the specified index.
     */
    template <typename Index, std::enable_if_t<std::is_integral_v<Index>, int> = 0>
    const json &operator[](Index idx) const { return std::get<std::vector<json>>(value).at(static_cast<size_t>(idx)); }

    [[nodiscard]] bool operator==(const json &other) const noexcept { return value == other.value; }
    [[nodiscard]] bool operator!=(const json &other) const noexcept { return !(*this == other); }

    /**
     * @brief Checks if the JSON value is null.
     *
     * @return true if the JSON value is null, false otherwise.
     */
    [[nodiscard]] bool is_null() const noexcept { return value.index() == 0; }
    /**
     * @brief Checks if the JSON value is a boolean.
     *
     * @return true if the JSON value is a boolean, false otherwise.
     */
    [[nodiscard]] bool is_boolean() const noexcept { return value.index() == 1; }
    /**
     * @brief Checks if the JSON value is an integer.
     *
     * @return true if the JSON value is an integer, false otherwise.
     */
    [[nodiscard]] bool is_integer() const noexcept { return value.index() == 2; }
    /**
     * @brief Checks if the JSON value is an unsigned integer.
     *
     * @return true if the JSON value is an unsigned integer, false otherwise.
     */
    [[nodiscard]] bool is_unsigned() const noexcept { return value.index() == 3; }
    /**
     * @brief Checks if the JSON value is a floating-point number.
     *
     * @return true if the JSON value is a floating-point number, false otherwise.
     */
    [[nodiscard]] bool is_float() const noexcept { return value.index() == 4; }
    /**
     * @brief Checks if the JSON value is a number.
     *
     * This function checks if the JSON value is of type integer, unsigned integer, or floating-point number.
     *
     * @return true if the JSON value is a number, false otherwise.
     */
    [[nodiscard]] bool is_number() const noexcept { return value.index() == 2 || value.index() == 3 || value.index() == 4; }
    /**
     * @brief Checks if the JSON value is a string.
     *
     * @return true if the JSON value is a string, false otherwise.
     */
    [[nodiscard]] bool is_string() const noexcept { return value.index() == 5; }
    /**
     * @brief Checks if the JSON value is an object.
     *
     * @return true if the JSON value is an object, false otherwise.
     */
    [[nodiscard]] bool is_object() const noexcept { return value.index() == 6; }
    /**
     * @brief Checks if the JSON value is an array.
     *
     * @return true if the JSON value is an array, false otherwise.
     */
    [[nodiscard]] bool is_array() const noexcept { return value.index() == 7; }

    /**
     * @brief Checks if the JSON value is a primitive type.
     *
     * This function checks if the JSON value is of a primitive type, which includes null, boolean, integer, unsigned integer,
     * floating-point number, or string.
     *
     * @return true if the JSON value is a primitive type, false otherwise.
     */
    [[nodiscard]] bool is_primitive() const noexcept { return is_null() || is_boolean() || is_integer() || is_unsigned() || is_float() || is_string(); }
    /**
     * @brief Checks if the JSON value is a structured type.
     *
     * This function checks if the JSON value is either an object or an array.
     *
     * @return true if the JSON value is a structured type (object or array), false otherwise.
     */
    [[nodiscard]] bool is_structured() const noexcept { return is_object() || is_array(); }

    /**
     * @brief Returns the type of the JSON value.
     *
     * This function returns the type of the JSON value as an enum value of the `json_type` enumeration.
     * The possible types are: null, boolean, number, string, object, and array.
     *
     * @return The type of the JSON value.
     */
    [[nodiscard]] json_type get_type() const noexcept
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

    /**
     * Returns the size of the JSON value.
     * For arrays, it returns the number of elements.
     * For objects, it returns the number of key-value pairs.
     * For other types, it returns 0.
     *
     * @return The size of the JSON value.
     */
    [[nodiscard]] size_t size() const
    {
      switch (get_type())
      {
      case json_type::array:
        return std::get<std::vector<json>>(value).size();
      case json_type::object:
        return std::get<std::map<std::string, json, std::less<>>>(value).size();
      default:
        return 0;
      }
    }

    /**
     * Checks if the JSON object contains a specific key.
     *
     * @param key The key to check for.
     * @return True if the key is present in the JSON object, false otherwise.
     */
    [[nodiscard]] bool contains(std::string_view key) const { return is_object() && std::get<std::map<std::string, json, std::less<>>>(value).count(key) > 0; }

    template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
    [[nodiscard]] bool operator==(T num) const noexcept
    {
      switch (value.index())
      {
      case 1:
        return std::get<bool>(value) == static_cast<bool>(num);
      case 2:
        return std::get<int64_t>(value) == static_cast<int64_t>(num);
      case 3:
        return std::get<uint64_t>(value) == static_cast<uint64_t>(num);
      case 4:
        return std::get<double>(value) == static_cast<double>(num);
      default:
        return false;
      }
    }

    template <typename T, std::enable_if_t<std::is_same_v<std::decay_t<T>, std::string> || std::is_same_v<std::decay_t<T>, std::string_view> || std::is_same_v<std::decay_t<T>, const char *>, int> = 0>
    [[nodiscard]] bool operator==(T &&str) const noexcept { return value.index() == 5 && std::get<std::string>(value) == std::string(str); }

    template <typename T, std::enable_if_t<std::is_arithmetic_v<T> && !std::is_same_v<std::decay_t<T>, bool>, int> = 0>
    [[nodiscard]] bool operator!=(T num) const noexcept
    {
      switch (value.index())
      {
      case 1:
        return std::get<bool>(value) != static_cast<bool>(num);
      case 2:
        return std::get<int64_t>(value) != static_cast<int64_t>(num);
      case 3:
        return std::get<uint64_t>(value) != static_cast<uint64_t>(num);
      case 4:
        return std::get<double>(value) != static_cast<double>(num);
      default:
        return true;
      }
    }

    template <typename T, std::enable_if_t<std::is_same_v<std::decay_t<T>, std::string> || std::is_same_v<std::decay_t<T>, std::string_view> || std::is_same_v<std::decay_t<T>, const char *>, int> = 0>
    [[nodiscard]] bool operator!=(T &&str) const noexcept { return value.index() != 5 || std::get<std::string>(value) != std::string(str); }

    /**
     * @brief Conversion operator to bool.
     *
     * This operator converts the JSON value to a boolean value.
     * The conversion is based on the type of the JSON value.
     *
     * @return true if the JSON value is considered true, false otherwise.
     */
    operator bool() const noexcept
    {
      switch (value.index())
      {
      case 1:
        return std::get<bool>(value);
      case 2:
        return std::get<int64_t>(value) != 0;
      case 3:
        return std::get<uint64_t>(value) != 0;
      case 4:
        return !std::get<double>(value);
      case 5:
        return !std::get<std::string>(value).empty();
      case 6:
        return !std::get<std::map<std::string, json, std::less<>>>(value).empty();
      case 7:
        return !std::get<std::vector<json>>(value).empty();
      default:
        return false;
      }
    }

    /**
     * @brief Conversion operator to int.
     *
     * This operator converts the JSON value to an integer value.
     * The conversion is based on the type of the JSON value.
     *
     * @return The integer value of the JSON object.
     */
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

    /**
     * @brief Conversion operator to unsigned 64-bit integer.
     *
     * This operator converts the JSON value to an unsigned 64-bit integer value.
     * The conversion is based on the type of the JSON value.
     *
     * @return The unsigned 64-bit integer value of the JSON object.
     */
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

    /**
     * @brief Conversion operator to double.
     *
     * This operator converts the JSON value to a double value.
     * The conversion is based on the type of the JSON value.
     *
     * @return The double value of the JSON object.
     */
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

    /**
     * @brief Conversion operator to string.
     *
     * This operator converts the JSON value to a string.
     * The conversion is based on the type of the JSON value.
     *
     * @return The string value of the JSON object.
     */
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

    /**
     * @brief Conversion operator to std::map<std::string, json, std::less<>>.
     *
     * This operator converts the JSON value to a std::map<std::string, json, std::less<>>.
     * The conversion is based on the type of the JSON value.
     *
     * @return The std::map<std::string, json, std::less<>> value of the JSON object.
     */
    operator std::map<std::string, json, std::less<>>() const noexcept { return value.index() == 6 ? std::get<std::map<std::string, json, std::less<>>>(value) : std::map<std::string, json, std::less<>>(); }
    /**
     * Returns a constant reference to the underlying map object if the JSON value is an object.
     *
     * @return A constant reference to the underlying map object.
     */
    [[nodiscard]] const std::map<std::string, json, std::less<>> &as_object() const noexcept { return std::get<std::map<std::string, json, std::less<>>>(value); }

    /**
     * @brief Conversion operator to std::vector<json>.
     *
     * This operator converts the JSON value to a std::vector<json>.
     * The conversion is based on the type of the JSON value.
     *
     * @return The std::vector<json> value of the JSON object.
     */
    operator std::vector<json>() const noexcept { return value.index() == 7 ? std::get<std::vector<json>>(value) : std::vector<json>(); }
    /**
     * Returns a const reference to the internal vector representation of the JSON value.
     *
     * @return const std::vector<json>& A const reference to the internal vector representation of the JSON value.
     */
    [[nodiscard]] const std::vector<json> &as_array() const noexcept { return std::get<std::vector<json>>(value); }

    /**
     * @brief Clears the JSON object.
     *
     * This function clears the JSON object by setting its value to null.
     */
    void clear() noexcept { value = nullptr; }

    /**
     * @brief Adds a new element to the end of the JSON array.
     *
     * This function appends a new element to the end of the JSON array. The element
     * to be added should be of the same type as the elements in the array.
     *
     * @param val The value to be added to the array.
     */
    void push_back(const json &val) { std::get<std::vector<json>>(value).push_back(val); }
    /**
     * @brief Adds a new element to the end of the JSON array.
     *
     * This function appends a new element to the end of the JSON array. The element
     * to be added should be of the same type as the elements in the array.
     *
     * @param val The value to be added to the array.
     */
    void push_back(json &&val) { std::get<std::vector<json>>(value).push_back(std::move(val)); }

    /**
     * @brief Erases the element with the specified key from the JSON object.
     *
     * This function removes the element with the specified key from the JSON object.
     * If the key does not exist, no changes are made.
     *
     * @param key The key of the element to be erased.
     */
    void erase(std::string_view key) { std::get<std::map<std::string, json, std::less<>>>(value).erase(key.data()); }

    /**
     * @brief Erases an element at the specified index from the JSON array.
     *
     * This function removes the element at the specified index from the JSON array.
     * The index must be a valid position within the array.
     *
     * @param index The index of the element to be erased.
     */
    void erase(size_t index) { std::get<std::vector<json>>(value).erase(std::get<std::vector<json>>(value).begin() + index); }

    /**
     * Returns a string representation of the JSON value.
     *
     * The `dump` function converts the JSON value into a string representation.
     * The resulting string represents the JSON value according to its type:
     * - For a null value, the string "null" is returned.
     * - For a boolean value, the string "true" or "false" is returned.
     * - For an integer value, the string representation of the integer is returned.
     * - For a floating-point value, the string representation of the floating-point number is returned.
     * - For a string value, the string is enclosed in double quotes and returned.
     * - For an object value, the string representation of the object is returned.
     * - For an array value, the string representation of the array is returned.
     *
     * @return A string representation of the JSON value.
     */
    [[nodiscard]] std::string dump() const
    {
      switch (value.index())
      {
      case 0: // null
        return "null";
      case 1: // boolean
        return std::get<bool>(value) ? "true" : "false";
      case 2: // int64_t
        return std::to_string(std::get<int64_t>(value));
      case 3: // uint64_t
        return std::to_string(std::get<uint64_t>(value));
      case 4: // double
        return std::to_string(std::get<double>(value));
      case 5: // string
      {
        std::string escaped;
        escaped.reserve(std::get<std::string>(value).size());
        for (char c : std::get<std::string>(value))
          switch (c)
          {
          case '"':
            escaped += "\\\"";
            break;
          case '\\':
            escaped += "\\\\";
            break;
          case '\b':
            escaped += "\\b";
            break;
          case '\f':
            escaped += "\\f";
            break;
          case '\n':
            escaped += "\\n";
            break;
          case '\r':
            escaped += "\\r";
            break;
          case '\t':
            escaped += "\\t";
            break;
          default:
            escaped += c;
            break;
          }
        return "\"" + escaped + "\"";
      }
      case 6: // object
      {
        std::string str = "{";
        const auto &m = std::get<std::map<std::string, json, std::less<>>>(value);
        for (auto it = m.begin(); it != m.end(); ++it)
        {
          if (it != m.begin())
            str += ",";
          str += "\"" + it->first + "\":" + it->second.dump();
        }
        return str + "}";
      }
      case 7: // array
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
      default: // should never happen
        return "";
      }
    }

    friend std::ostream &operator<<(std::ostream &os, const json &j) { return os << j.dump(); }

  private:
    std::variant<std::nullptr_t, bool, int64_t, uint64_t, double, std::string, std::map<std::string, json, std::less<>>, std::vector<json>> value;
  };

  /**
   * Loads a JSON object from the given input stream.
   *
   * @param is The input stream to read the JSON object from.
   * @return The loaded JSON object.
   */
  [[nodiscard]] json load(std::istream &is);

  /**
   * Loads a JSON object from a string.
   *
   * @param str The string containing the JSON object.
   * @return The loaded JSON object.
   */
  [[nodiscard]] json load(std::string_view str);

  /**
   * Validates a JSON value against a JSON schema.
   *
   * @param value The JSON value to be validated.
   * @param schema The JSON schema to validate against.
   * @param schema_refs The JSON schema references.
   * @return `true` if the value is valid according to the schema, `false` otherwise.
   */
  [[nodiscard]] bool validate(const json &value, const json &schema, const json &schema_refs);
} // namespace json
