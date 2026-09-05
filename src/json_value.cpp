#include "json_mini.hpp"

namespace jsonmini {

Value::Value() : type_(Type::Null) {}
Value::Value(std::nullptr_t) : type_(Type::Null) {}
Value::Value(bool b) : type_(Type::Bool), bool_(b) {}
Value::Value(double n) : type_(Type::Number), number_(n) {}
Value::Value(std::string s) : type_(Type::String), string_(std::move(s)) {}

Value Value::array() {
    Value v;
    v.type_ = Type::Array;
    return v;
}

Value Value::object() {
    Value v;
    v.type_ = Type::Object;
    return v;
}

bool Value::as_bool() const {
    if (type_ != Type::Bool) throw std::logic_error("not a bool");
    return bool_;
}

double Value::as_number() const {
    if (type_ != Type::Number) throw std::logic_error("not a number");
    return number_;
}

const std::string &Value::as_string() const {
    if (type_ != Type::String) throw std::logic_error("not a string");
    return string_;
}

const std::vector<Value> &Value::as_array() const {
    if (type_ != Type::Array) throw std::logic_error("not an array");
    return array_;
}

std::vector<Value> &Value::as_array() {
    if (type_ != Type::Array) throw std::logic_error("not an array");
    return array_;
}

const std::map<std::string, Value> &Value::as_object() const {
    if (type_ != Type::Object) throw std::logic_error("not an object");
    return object_;
}

std::map<std::string, Value> &Value::as_object() {
    if (type_ != Type::Object) throw std::logic_error("not an object");
    return object_;
}

}  // namespace jsonmini
