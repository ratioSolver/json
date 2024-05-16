#pragma once

#include <string>
#include <variant>
#include <map>
#include <vector>
#include <cstdint>

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
        value = static_cast<int64_t>(0);
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
    /**
     * @brief Copy constructor for the json class.
     *
     * This constructor creates a new json object by copying the contents of another json object.
     *
     * @param other The json object to be copied.
     */
    json(const json &other) noexcept : value(other.value) {}
    /**
     * @brief Move constructor for the json class.
     *
     * This constructor moves the contents of the `other` object into the newly created `json` object.
     * The `other` object is left in a valid but unspecified state.
     *
     * @param other The `json` object to be moved from.
     */
    json(json &&other) noexcept : value(std::move(other.value)) { other.value = nullptr; }
    /**
     * @brief Constructs a `json` object from a `nullptr_t`.
     *
     * This constructor creates a `json` object with a null value.
     *
     * @param[in] nullptr_t A null pointer constant.
     */
    json(std::nullptr_t) noexcept : value(nullptr) {}
    /**
     * @brief Constructs a `json` object from a boolean value.
     *
     * This constructor initializes a `json` object with the given boolean value.
     *
     * @param b The boolean value to initialize the `json` object with.
     */
    json(bool b) noexcept : value(b) {}
    /**
     * @brief Constructs a `json` object with an integer value.
     *
     * This constructor initializes a `json` object with the given integer value.
     *
     * @param l The integer value to initialize the `json` object with.
     */
    json(int l) noexcept : value(static_cast<int64_t>(l)) {}
    /**
     * @brief Constructs a `json` object from an int64_t value.
     *
     * This constructor initializes a `json` object with the given int64_t value.
     *
     * @param l The int64_t value to initialize the `json` object with.
     */
    json(int64_t l) noexcept : value(l) {}
    /**
     * @brief Constructs a `json` object with an unsigned 64-bit integer value.
     *
     * This constructor initializes a `json` object with the provided unsigned 64-bit integer value.
     *
     * @param l The unsigned 64-bit integer value to initialize the `json` object with.
     */
    json(uint64_t l) noexcept : value(l) {}
    /**
     * @brief Constructs a `json` object from a double value.
     *
     * This constructor initializes a `json` object with the given double value.
     *
     * @param d The double value to initialize the `json` object with.
     */
    json(double d) noexcept : value(d) {}
    /**
     * @brief Constructs a `json` object from a string.
     *
     * This constructor initializes a `json` object with the given string.
     *
     * @param str The string to initialize the `json` object with.
     */
    json(const std::string &str) noexcept : value(str) {}
    /**
     * @brief Constructs a `json` object from a C-style string.
     *
     * This constructor creates a `json` object by converting the given C-style string into a `std::string`.
     *
     * @param str The C-style string to be converted into a `json` object.
     */
    json(const char *str) noexcept : value(std::string(str)) {}
    /**
     * @brief Constructs a `json` object from a vector of `json` objects.
     *
     * This constructor initializes a `json` object with the given vector of `json` objects.
     *
     * @param arr The vector of `json` objects to initialize the `json` object with.
     */
    json(std::vector<json> &&arr) noexcept : value(std::move(arr)) {}

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
     * @brief Move assignment operator for the json class.
     *
     * This operator allows the json object to be assigned the contents of another json object using move semantics.
     *
     * @param other The json object to be moved from.
     * @return A reference to the modified json object.
     */
    json &operator=(json &&other) noexcept
    {
      value = std::move(other.value);
      return *this;
    }

    /**
     * @brief Assignment operator overload for assigning a nullptr to a json object.
     *
     * This operator allows assigning a nullptr to a json object. It sets the value of the json object to nullptr.
     *
     * @param[in] nullptr_t The nullptr to assign to the json object.
     * @return json& A reference to the modified json object.
     */
    json &operator=(std::nullptr_t) noexcept
    {
      value = nullptr;
      return *this;
    }

    /**
     * @brief Assigns a string value to the JSON object.
     *
     * This assignment operator allows you to assign a string value to a JSON object.
     *
     * @param str The string value to assign.
     * @return A reference to the modified JSON object.
     */
    json &operator=(const std::string &str) noexcept
    {
      value = str;
      return *this;
    }

    /**
     * @brief Assignment operator overload for assigning a C-style string to a json object.
     *
     * This operator allows you to assign a C-style string to a json object. The C-style string
     * is converted to a std::string and assigned to the 'value' member variable of the json object.
     *
     * @param str The C-style string to assign to the json object.
     * @return A reference to the modified json object.
     */
    json &operator=(const char *str) noexcept
    {
      value = std::string(str);
      return *this;
    }

    /**
     * @brief Assignment operator for assigning a boolean value to a JSON object.
     *
     * This operator assigns a boolean value to a JSON object and returns a reference to the modified object.
     *
     * @param b The boolean value to be assigned.
     * @return A reference to the modified JSON object.
     */
    json &operator=(bool b) noexcept
    {
      value = b;
      return *this;
    }

    /**
     * @brief Assignment operator for assigning an integer value to a JSON object.
     *
     * This operator assigns the given integer value to the JSON object and returns a reference to the modified object.
     *
     * @param l The integer value to assign.
     * @return A reference to the modified JSON object.
     */
    json &operator=(int l) noexcept
    {
      value = static_cast<int64_t>(l);
      return *this;
    }

    /**
     * @brief Assigns an int64_t value to the JSON object.
     *
     * This assignment operator allows you to assign an int64_t value to a JSON object.
     *
     * @param l The int64_t value to assign.
     * @return A reference to the modified JSON object.
     */
    json &operator=(int64_t l) noexcept
    {
      value = l;
      return *this;
    }

    /**
     * @brief Assigns an unsigned 64-bit integer value to the JSON object.
     *
     * This assignment operator allows you to assign an unsigned 64-bit integer value
     * to a JSON object. The value is stored in the `value` member variable of the JSON object.
     *
     * @param l The unsigned 64-bit integer value to assign.
     * @return A reference to the modified JSON object.
     */
    json &operator=(uint64_t l) noexcept
    {
      value = l;
      return *this;
    }

    /**
     * @brief Assigns a double value to the JSON object.
     *
     * This assignment operator allows you to assign a double value to a JSON object.
     *
     * @param d The double value to assign.
     * @return A reference to the modified JSON object.
     */
    json &operator=(double d) noexcept
    {
      value = d;
      return *this;
    }

    /**
     * @brief Accesses the value associated with the specified key in the JSON object.
     *
     * This operator allows accessing the value associated with the specified key in the JSON object.
     * The key is provided as a C-style string.
     *
     * @param str The key to access the value.
     * @return A reference to the value associated with the key.
     */
    json &operator[](const char *str) { return operator[](std::string(str)); }
    /**
     * @brief Accesses the value associated with the specified key in the JSON object.
     *
     * This operator allows accessing the value associated with the specified key in the JSON object.
     * The key is provided as a C-style string.
     *
     * @param str The key to access the value.
     * @return A reference to the value associated with the key.
     */
    const json &operator[](const char *str) const { return operator[](std::string(str)); }

    /**
     * @brief Accesses the value associated with the specified key in the JSON object.
     *
     * This operator allows you to access the value associated with the specified key in the JSON object.
     * If the key does not exist, it will be created and associated with a default-constructed JSON value.
     *
     * @param key The key to access the value.
     * @return A reference to the value associated with the key.
     */
    json &operator[](const std::string &key) { return std::get<std::map<std::string, json>>(value)[key]; }
    /**
     * @brief Accesses the value associated with the specified key in the JSON object.
     *
     * This operator allows you to access the value associated with the specified key in the JSON object.
     * If the key does not exist, it will be created and associated with a default-constructed JSON value.
     *
     * @param key The key to access the value.
     * @return A reference to the value associated with the key.
     */
    const json &operator[](const std::string &key) const { return std::get<std::map<std::string, json>>(value).at(key); }

    /**
     * @brief Accesses the element at the specified index in the JSON object.
     *
     * This operator allows accessing the element at the specified index in the JSON object.
     * The index can be either an integer or a size_t value.
     *
     * @param index The index of the element to access.
     * @return A reference to the element at the specified index.
     */
    json &operator[](int index) { return operator[](static_cast<size_t>(index)); }
    /**
     * @brief Accesses the element at the specified index in the JSON object.
     *
     * This operator allows accessing the element at the specified index in the JSON object.
     * The index can be either an integer or a size_t value.
     *
     * @param index The index of the element to access.
     * @return A reference to the element at the specified index.
     */
    const json &operator[](int index) const { return operator[](static_cast<size_t>(index)); }

    /**
     * @brief Accesses the element at the specified index in the JSON object.
     *
     * This operator allows you to access the element at the specified index in the JSON object.
     * It returns a reference to the element, allowing you to modify its value if needed.
     *
     * @param index The index of the element to access.
     * @return A reference to the element at the specified index.
     */
    json &operator[](size_t index) { return std::get<std::vector<json>>(value)[index]; }
    /**
     * @brief Accesses the element at the specified index in the JSON object.
     *
     * This operator allows you to access the element at the specified index in the JSON object.
     * It returns a reference to the element, allowing you to modify its value if needed.
     *
     * @param index The index of the element to access.
     * @return A reference to the element at the specified index.
     */
    const json &operator[](size_t index) const { return std::get<std::vector<json>>(value).at(index); }

    /**
     * @brief Returns the type of the JSON value.
     *
     * This function returns the type of the JSON value as an enum value of the `json_type` enumeration.
     * The possible types are: null, boolean, number, string, object, and array.
     *
     * @return The type of the JSON value.
     */
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

    /**
     * Returns the size of the JSON value.
     * For arrays, it returns the number of elements.
     * For objects, it returns the number of key-value pairs.
     * For other types, it returns 0.
     *
     * @return The size of the JSON value.
     */
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

    /**
     * Checks if the JSON object contains a specific key.
     *
     * @param key The key to check for.
     * @return True if the key is present in the JSON object, false otherwise.
     */
    bool contains(const std::string &key) const { return get_type() == json_type::object && std::get<std::map<std::string, json>>(value).count(key) > 0; }

    /**
     * @brief Overloads the equality operator for comparing two json objects.
     *
     * This function compares the value of two json objects and returns true if they are equal, and false otherwise.
     *
     * @param other The json object to compare with.
     * @return true if the two json objects are equal, false otherwise.
     */
    bool operator==(const json &other) const noexcept { return value == other.value; }
    /**
     * @brief Overloads the equality operator for comparing a json object with a nullptr.
     *
     * This function compares the value of the json object with a nullptr and returns true if the value is null, and false otherwise.
     *
     * @param nullptr_t A null pointer constant.
     * @return true if the json object is null, false otherwise.
     */
    bool operator==(std::nullptr_t) const noexcept { return value.index() == 0; }
    /**
     * @brief Overloads the equality operator for comparing a json object with a boolean value.
     *
     * This function compares the value of the json object with a boolean value and returns true if they are equal, and false otherwise.
     *
     * @param b The boolean value to compare with.
     * @return true if the json object is equal to the boolean value, false otherwise.
     */
    bool operator==(bool b) const noexcept { return value.index() == 1 && std::get<bool>(value) == b; }
    /**
     * @brief Overloads the equality operator for comparing a json object with an integer value.
     *
     * This function compares the value of the json object with an integer value and returns true if they are equal, and false otherwise.
     *
     * @param l The integer value to compare with.
     * @return true if the json object is equal to the integer value, false otherwise.
     */
    bool operator==(int l) const noexcept
    {
      switch (value.index())
      {
      case 1:
        return std::get<bool>(value) == static_cast<bool>(l);
      case 2:
        return std::get<int64_t>(value) == static_cast<int64_t>(l);
      case 3:
        return std::get<uint64_t>(value) == static_cast<uint64_t>(l);
      case 4:
        return std::get<double>(value) == static_cast<double>(l);
      default:
        return false;
      }
    }
    /**
     * @brief Overloads the equality operator for comparing a json object with an int64_t value.
     *
     * This function compares the value of the json object with an int64_t value and returns true if they are equal, and false otherwise.
     *
     * @param l The int64_t value to compare with.
     * @return true if the json object is equal to the int64_t value, false otherwise.
     */
    bool operator==(int64_t l) const noexcept { return value.index() == 2 && std::get<int64_t>(value) == l; }
    /**
     * @brief Overloads the equality operator for comparing a json object with an unsigned 64-bit integer value.
     *
     * This function compares the value of the json object with an unsigned 64-bit integer value and returns true if they are equal, and false otherwise.
     *
     * @param l The unsigned 64-bit integer value to compare with.
     * @return true if the json object is equal to the unsigned 64-bit integer value, false otherwise.
     */
    bool operator==(uint64_t l) const noexcept { return value.index() == 3 && std::get<uint64_t>(value) == l; }
    /**
     * @brief Overloads the equality operator for comparing a json object with a double value.
     *
     * This function compares the value of the json object with a double value and returns true if they are equal, and false otherwise.
     *
     * @param d The double value to compare with.
     * @return true if the json object is equal to the double value, false otherwise.
     */
    bool operator==(double d) const noexcept { return value.index() == 4 && std::get<double>(value) == d; }
    /**
     * @brief Overloads the equality operator for comparing a json object with a string.
     *
     * This function compares the value of the json object with a string and returns true if they are equal, and false otherwise.
     *
     * @param str The string to compare with.
     * @return true if the json object is equal to the string, false otherwise.
     */
    bool operator==(const std::string &str) const noexcept { return value.index() == 5 && std::get<std::string>(value) == str; }
    /**
     * @brief Overloads the equality operator for comparing a json object with a C-style string.
     *
     * This function compares the value of the json object with a C-style string and returns true if they are equal, and false otherwise.
     *
     * @param str The C-style string to compare with.
     * @return true if the json object is equal to the C-style string, false otherwise.
     */
    bool operator==(const char *str) const noexcept { return value.index() == 5 && std::get<std::string>(value) == str; }

    /**
     * @brief Overloads the inequality operator for comparing two json objects.
     *
     * This function compares the value of two json objects and returns true if they are not equal, and false otherwise.
     *
     * @param other The json object to compare with.
     * @return true if the two json objects are not equal, false otherwise.
     */
    bool operator!=(const json &other) const noexcept { return value != other.value; }
    /**
     * @brief Overloads the inequality operator for comparing a json object with a nullptr.
     *
     * This function compares the value of the json object with a nullptr and returns true if the value is not null, and false otherwise.
     *
     * @param nullptr_t A null pointer constant.
     * @return true if the json object is not null, false otherwise.
     */
    bool operator!=(std::nullptr_t) const noexcept { return value.index() != 0; }
    /**
     * @brief Overloads the inequality operator for comparing a json object with a boolean value.
     *
     * This function compares the value of the json object with a boolean value and returns true if they are not equal, and false otherwise.
     *
     * @param b The boolean value to compare with.
     * @return true if the json object is not equal to the boolean value, false otherwise.
     */
    bool operator!=(bool b) const noexcept { return value.index() != 1 || std::get<bool>(value) != b; }
    /**
     * @brief Overloads the inequality operator for comparing a json object with an integer value.
     *
     * This function compares the value of the json object with an integer value and returns true if they are not equal, and false otherwise.
     *
     * @param l The integer value to compare with.
     * @return true if the json object is not equal to the integer value, false otherwise.
     */
    bool operator!=(int l) const noexcept { return !(*this == l); }
    /**
     * @brief Overloads the inequality operator for comparing a json object with an int64_t value.
     *
     * This function compares the value of the json object with an int64_t value and returns true if they are not equal, and false otherwise.
     *
     * @param l The int64_t value to compare with.
     * @return true if the json object is not equal to the int64_t value, false otherwise.
     */
    bool operator!=(int64_t l) const noexcept { return value.index() != 2 || std::get<int64_t>(value) != l; }
    /**
     * @brief Overloads the inequality operator for comparing a json object with an unsigned 64-bit integer value.
     *
     * This function compares the value of the json object with an unsigned 64-bit integer value and returns true if they are not equal, and false otherwise.
     *
     * @param l The unsigned 64-bit integer value to compare with.
     * @return true if the json object is not equal to the unsigned 64-bit integer value, false otherwise.
     */
    bool operator!=(uint64_t l) const noexcept { return value.index() != 3 || std::get<uint64_t>(value) != l; }
    /**
     * @brief Overloads the inequality operator for comparing a json object with a double value.
     *
     * This function compares the value of the json object with a double value and returns true if they are not equal, and false otherwise.
     *
     * @param d The double value to compare with.
     * @return true if the json object is not equal to the double value, false otherwise.
     */
    bool operator!=(double d) const noexcept { return value.index() != 4 || std::get<double>(value) != d; }
    /**
     * @brief Overloads the inequality operator for comparing a json object with a string.
     *
     * This function compares the value of the json object with a string and returns true if they are not equal, and false otherwise.
     *
     * @param str The string to compare with.
     * @return true if the json object is not equal to the string, false otherwise.
     */
    bool operator!=(const std::string &str) const noexcept { return value.index() != 5 || std::get<std::string>(value) != str; }
    /**
     * @brief Overloads the inequality operator for comparing a json object with a C-style string.
     *
     * This function compares the value of the json object with a C-style string and returns true if they are not equal, and false otherwise.
     *
     * @param str The C-style string to compare with.
     * @return true if the json object is not equal to the C-style string, false otherwise.
     */
    bool operator!=(const char *str) const noexcept { return value.index() != 5 || std::get<std::string>(value) != str; }

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
        return !std::get<std::map<std::string, json>>(value).empty();
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
     * @brief Conversion operator to std::map<std::string, json>.
     *
     * This operator converts the JSON value to a std::map<std::string, json>.
     * The conversion is based on the type of the JSON value.
     *
     * @return The std::map<std::string, json> value of the JSON object.
     */
    operator std::map<std::string, json>() const noexcept { return value.index() == 6 ? std::get<std::map<std::string, json>>(value) : std::map<std::string, json>(); }
    /**
     * Returns a constant reference to the underlying map object if the JSON value is an object.
     *
     * @return A constant reference to the underlying map object.
     */
    const std::map<std::string, json> &as_object() const noexcept { return std::get<std::map<std::string, json>>(value); }

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
    const std::vector<json> &as_array() const noexcept { return std::get<std::vector<json>>(value); }

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
    void erase(const std::string &key) { std::get<std::map<std::string, json>>(value).erase(key); }

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

  /**
   * Loads a JSON object from the given input stream.
   *
   * @param is The input stream to read the JSON object from.
   * @return The loaded JSON object.
   */
  json load(std::istream &is);

  /**
   * Loads a JSON object from a string.
   *
   * @param str The input string containing the JSON data.
   * @return A JSON object representing the parsed data.
   */
  json load(const char *str);

  /**
   * Loads a JSON object from a string.
   *
   * @param str The string containing the JSON object.
   * @return The loaded JSON object.
   */
  inline json load(const std::string &str) { return load(str.c_str()); }
} // namespace json
