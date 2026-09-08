#include "json_mini.hpp"

#include <cctype>
#include <sstream>

namespace jsonmini {
namespace {

class Parser {
public:
    explicit Parser(const std::string &text) : text_(text) {}

    Value parse_value() {
        skip_ws();
        if (pos_ >= text_.size()) {
            throw error("unexpected end of input");
        }
        char c = text_[pos_];
        if (c == 'n') return parse_null();
        if (c == 't' || c == 'f') return parse_bool();
        if (c == '"') return parse_string_value();
        if (c == '[') return parse_array();
        if (c == '{') return parse_object();
        if (c == '-' || std::isdigit(static_cast<unsigned char>(c))) return parse_number();
        throw error(std::string("unexpected character '") + c + "'");
    }

    void expect_end() {
        skip_ws();
        if (pos_ != text_.size()) {
            throw error("trailing characters after JSON value");
        }
    }

private:
    const std::string &text_;
    std::size_t pos_ = 0;
    std::size_t line_ = 1;
    std::size_t column_ = 1;

    ParseError error(const std::string &message) const {
        return ParseError(message, line_, column_);
    }

    void advance() {
        if (pos_ < text_.size() && text_[pos_] == '\n') {
            ++line_;
            column_ = 1;
        } else {
            ++column_;
        }
        ++pos_;
    }

    void skip_ws() {
        while (pos_ < text_.size() &&
               std::isspace(static_cast<unsigned char>(text_[pos_]))) {
            advance();
        }
    }

    void expect(char ch) {
        skip_ws();
        if (pos_ >= text_.size() || text_[pos_] != ch) {
            throw error(std::string("expected '") + ch + "'");
        }
        advance();
    }

    Value parse_null() {
        if (text_.compare(pos_, 4, "null") != 0) throw error("invalid null");
        for (int i = 0; i < 4; ++i) advance();
        return Value(nullptr);
    }

    Value parse_bool() {
        if (text_.compare(pos_, 4, "true") == 0) {
            for (int i = 0; i < 4; ++i) advance();
            return Value(true);
        }
        if (text_.compare(pos_, 5, "false") == 0) {
            for (int i = 0; i < 5; ++i) advance();
            return Value(false);
        }
        throw error("invalid boolean");
    }

    std::string parse_string() {
        expect('"');
        std::string out;
        while (pos_ < text_.size()) {
            char c = text_[pos_];
            if (c == '"') {
                advance();
                return out;
            }
            if (c == '\\') {
                advance();
                if (pos_ >= text_.size()) throw error("unterminated escape");
                char e = text_[pos_];
                advance();
                switch (e) {
                    case '"': out.push_back('"'); break;
                    case '\\': out.push_back('\\'); break;
                    case '/': out.push_back('/'); break;
                    case 'b': out.push_back('\b'); break;
                    case 'f': out.push_back('\f'); break;
                    case 'n': out.push_back('\n'); break;
                    case 'r': out.push_back('\r'); break;
                    case 't': out.push_back('\t'); break;
                    default: throw error("invalid escape sequence");
                }
                continue;
            }
            if (static_cast<unsigned char>(c) < 0x20) {
                throw error("unescaped control character in string");
            }
            out.push_back(c);
            advance();
        }
        throw error("unterminated string");
    }

    Value parse_string_value() { return Value(parse_string()); }

    Value parse_number() {
        std::size_t start = pos_;
        if (text_[pos_] == '-') advance();
        if (pos_ >= text_.size()) throw error("invalid number");
        if (text_[pos_] == '0') {
            advance();
        } else if (std::isdigit(static_cast<unsigned char>(text_[pos_]))) {
            while (pos_ < text_.size() &&
                   std::isdigit(static_cast<unsigned char>(text_[pos_]))) {
                advance();
            }
        } else {
            throw error("invalid number");
        }
        if (pos_ < text_.size() && text_[pos_] == '.') {
            advance();
            if (pos_ >= text_.size() ||
                !std::isdigit(static_cast<unsigned char>(text_[pos_]))) {
                throw error("invalid number fraction");
            }
            while (pos_ < text_.size() &&
                   std::isdigit(static_cast<unsigned char>(text_[pos_]))) {
                advance();
            }
        }
        if (pos_ < text_.size() && (text_[pos_] == 'e' || text_[pos_] == 'E')) {
            advance();
            if (pos_ < text_.size() && (text_[pos_] == '+' || text_[pos_] == '-')) {
                advance();
            }
            if (pos_ >= text_.size() ||
                !std::isdigit(static_cast<unsigned char>(text_[pos_]))) {
                throw error("invalid number exponent");
            }
            while (pos_ < text_.size() &&
                   std::isdigit(static_cast<unsigned char>(text_[pos_]))) {
                advance();
            }
        }
        try {
            return Value(std::stod(text_.substr(start, pos_ - start)));
        } catch (...) {
            throw error("number out of range");
        }
    }

    Value parse_array() {
        expect('[');
        Value arr = Value::array();
        skip_ws();
        if (pos_ < text_.size() && text_[pos_] == ']') {
            advance();
            return arr;
        }
        while (true) {
            arr.as_array().push_back(parse_value());
            skip_ws();
            if (pos_ < text_.size() && text_[pos_] == ']') {
                advance();
                break;
            }
            expect(',');
        }
        return arr;
    }

    Value parse_object() {
        expect('{');
        Value obj = Value::object();
        skip_ws();
        if (pos_ < text_.size() && text_[pos_] == '}') {
            advance();
            return obj;
        }
        while (true) {
            skip_ws();
            if (pos_ >= text_.size() || text_[pos_] != '"') {
                throw error("expected object key string");
            }
            std::string key = parse_string();
            expect(':');
            obj.as_object().emplace(std::move(key), parse_value());
            skip_ws();
            if (pos_ < text_.size() && text_[pos_] == '}') {
                advance();
                break;
            }
            expect(',');
        }
        return obj;
    }
};

std::string escape_string(const std::string &s) {
    std::ostringstream out;
    out << '"';
    for (unsigned char c : s) {
        switch (c) {
            case '"': out << "\\\""; break;
            case '\\': out << "\\\\"; break;
            case '\b': out << "\\b"; break;
            case '\f': out << "\\f"; break;
            case '\n': out << "\\n"; break;
            case '\r': out << "\\r"; break;
            case '\t': out << "\\t"; break;
            default:
                if (c < 0x20) {
                    out << "\\u00" << "0123456789abcdef"[c >> 4]
                        << "0123456789abcdef"[c & 0xF];
                } else {
                    out << static_cast<char>(c);
                }
        }
    }
    out << '"';
    return out.str();
}

}  // namespace

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

std::string Value::stringify(bool pretty, int indent) const {
    switch (type_) {
        case Type::Null:
            return "null";
        case Type::Bool:
            return bool_ ? "true" : "false";
        case Type::Number: {
            std::ostringstream out;
            out.precision(15);
            out << number_;
            return out.str();
        }
        case Type::String:
            return escape_string(string_);
        case Type::Array: {
            if (array_.empty()) return "[]";
            std::ostringstream out;
            out << '[';
            if (pretty) out << '\n';
            for (std::size_t i = 0; i < array_.size(); ++i) {
                if (pretty) out << std::string(static_cast<std::size_t>(indent + 2), ' ');
                out << array_[i].stringify(pretty, indent + 2);
                if (i + 1 < array_.size()) out << ',';
                if (pretty) out << '\n';
            }
            if (pretty) out << std::string(static_cast<std::size_t>(indent), ' ');
            out << ']';
            return out.str();
        }
        case Type::Object: {
            if (object_.empty()) return "{}";
            std::ostringstream out;
            out << '{';
            if (pretty) out << '\n';
            std::size_t i = 0;
            for (const auto &kv : object_) {
                if (pretty) out << std::string(static_cast<std::size_t>(indent + 2), ' ');
                out << escape_string(kv.first) << (pretty ? ": " : ":")
                    << kv.second.stringify(pretty, indent + 2);
                if (i + 1 < object_.size()) out << ',';
                if (pretty) out << '\n';
                ++i;
            }
            if (pretty) out << std::string(static_cast<std::size_t>(indent), ' ');
            out << '}';
            return out.str();
        }
    }
    return "null";
}

ParseError::ParseError(const std::string &message, std::size_t line, std::size_t column)
    : std::runtime_error("JSON parse error at " + std::to_string(line) + ":" +
                         std::to_string(column) + ": " + message),
      line_(line),
      column_(column) {}

Value parse(const std::string &text) {
    Parser parser(text);
    Value value = parser.parse_value();
    parser.expect_end();
    return value;
}

}  // namespace jsonmini
