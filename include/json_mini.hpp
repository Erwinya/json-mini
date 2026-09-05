#pragma once

#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace jsonmini {

enum class Type { Null, Bool, Number, String, Array, Object };

/// In-memory JSON value container (parse / stringify arrive in later commits).
class Value {
public:
    Value();
    explicit Value(std::nullptr_t);
    explicit Value(bool b);
    explicit Value(double n);
    explicit Value(std::string s);
    static Value array();
    static Value object();

    Type type() const { return type_; }

    bool is_null() const { return type_ == Type::Null; }
    bool as_bool() const;
    double as_number() const;
    const std::string &as_string() const;
    const std::vector<Value> &as_array() const;
    std::vector<Value> &as_array();
    const std::map<std::string, Value> &as_object() const;
    std::map<std::string, Value> &as_object();

private:
    Type type_;
    bool bool_ = false;
    double number_ = 0.0;
    std::string string_;
    std::vector<Value> array_;
    std::map<std::string, Value> object_;
};

}  // namespace jsonmini
