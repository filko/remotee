#ifndef REMOTEE_ENGINE_JSON_HH
#define REMOTEE_ENGINE_JSON_HH 1

#include <jansson.h>
#include <memory>
#include <paludis/util/exception.hh>

class JsonException
    : public paludis::Exception
{
public:
    JsonException(const std::string & m)
        : paludis::Exception(m)
    {
    }
};

struct JsonDeleter
{
    constexpr JsonDeleter() = default;

    void operator()(json_t * j)
    {
        json_decref(j);
    }
};

typedef std::unique_ptr<json_t, JsonDeleter> UniqueJson;

inline
UniqueJson load_json_or_throw(const std::string & input)
{
    json_error_t error;
    UniqueJson ptr(json_loadb(input.c_str(), input.size(), 0, &error));

    if (! ptr)
        throw JsonException(std::string("Parsing json failed: ") + error.text);

    return ptr;
}

inline
json_t * json_object_get_or_throw(json_t * json, const char * key)
{
    if (! json_is_object(json))
        throw JsonException("Expected object, found " + std::to_string(json_typeof(json)));

    json_t * value(json_object_get(json, key));
    if (! value)
        throw JsonException(std::string("Key '") + key + "' not found in object");

    return value;
}

inline
std::size_t json_array_size_or_throw(const json_t * json)
{
    if (! json_is_array(json))
        throw JsonException("Expected array, got " + std::to_string(json_typeof(json)));

    return json_array_size(json);
}

inline
json_t * json_array_get_or_throw(const json_t * json, const std::size_t index)
{
    if (! json_is_array(json))
        throw JsonException("Expected array, got " + std::to_string(json_typeof(json)));

    json_t * element(json_array_get(json, index));
    if (! element)
        throw JsonException("Could not fetch " + std::to_string(index) + "th array element");

    return element;
}

inline
int json_get_integer_or_throw(json_t * json)
{
    if (! json_is_integer(json))
        throw JsonException("Expected integer, got " + std::to_string(json_typeof(json)));

    return json_integer_value(json);
}

inline
std::string json_get_string_or_throw(json_t * json)
{
    if (! json_is_string(json))
        throw JsonException("Expected string, got " + std::to_string(json_typeof(json)));

    return json_string_value(json);
}

#endif
