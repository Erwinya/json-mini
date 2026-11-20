#include "json_mini.hpp"

#include <iostream>
#include <string>

namespace {

int fails = 0;

void expect(bool cond, const std::string &msg) {
    if (!cond) {
        std::cerr << "FAIL: " << msg << '\n';
        ++fails;
    }
}

}  // namespace

int main() {
    {
        auto v = jsonmini::parse(R"({"a":[1,true,null],"s":"hi"})");
        expect(v.type() == jsonmini::Type::Object, "object root");
        expect(v.as_object().at("a").as_array().size() == 3, "array size");
        expect(v.as_object().at("s").as_string() == "hi", "string value");
    }
    {
        auto v = jsonmini::parse(R"("\u0041\n\t")");
        expect(v.as_string() == "A\n\t", "unicode and escapes");
    }
    {
        auto v = jsonmini::parse(R"("\uD83D\uDE00")");
        expect(v.as_string().size() == 4, "surrogate pair to utf8");
    }
    {
        bool threw = false;
        try {
            jsonmini::parse("{\n\"a\":1,\n\"a\":2\n}");
        } catch (const jsonmini::ParseError &) {
            threw = true;
        }
        expect(threw, "duplicate keys rejected");
    }
    {
        auto v = jsonmini::parse("{\t\"a\":1\n}");
        expect(v.as_object().at("a").as_number() == 1.0, "tab/newline whitespace allowed");
    }
    {
        bool threw = false;
        try {
            jsonmini::parse("true\v");
        } catch (const jsonmini::ParseError &) {
            threw = true;
        }
        expect(threw, "vertical tab is not JSON whitespace");
    }
    {
        auto v = jsonmini::parse("1.25");
        expect(v.stringify() == "1.25", "number stringify");
    }
    {
        bool threw = false;
        try {
            jsonmini::parse("[1,");
        } catch (const jsonmini::ParseError &ex) {
            threw = true;
            expect(ex.line() >= 1, "error has line");
        }
        expect(threw, "truncated array errors");
    }

    if (fails) {
        std::cerr << fails << " assertion(s) failed\n";
        return 1;
    }
    std::cout << "test_json_mini: ok\n";
    return 0;
}
