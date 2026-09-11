#include "json_mini.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace {

void usage(const char *prog) {
    std::cerr << "Usage: " << prog << " [--pretty] [--validate] [--file path]\n"
              << "Parse JSON from a file or stdin.\n"
              << "  --pretty    pretty-print output\n"
              << "  --validate  check only; print nothing on success\n";
}

std::string read_all(std::istream &in) {
    std::ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

}  // namespace

int main(int argc, char **argv) {
    bool pretty = false;
    bool validate_only = false;
    const char *path = nullptr;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            usage(argv[0]);
            return 0;
        }
        if (arg == "--pretty") {
            pretty = true;
            continue;
        }
        if (arg == "--validate") {
            validate_only = true;
            continue;
        }
        if (arg == "--file") {
            if (i + 1 >= argc) {
                usage(argv[0]);
                return 2;
            }
            path = argv[++i];
            continue;
        }
        std::cerr << "unknown argument: " << arg << '\n';
        usage(argv[0]);
        return 2;
    }

    try {
        std::string text;
        if (path != nullptr) {
            std::ifstream in(path);
            if (!in) {
                std::cerr << "cannot open file: " << path << '\n';
                return 2;
            }
            text = read_all(in);
        } else {
            text = read_all(std::cin);
        }

        jsonmini::Value value = jsonmini::parse(text);
        if (!validate_only) {
            std::cout << value.stringify(pretty) << '\n';
        }
        return 0;
    } catch (const jsonmini::ParseError &ex) {
        std::cerr << ex.what() << '\n';
        return 1;
    } catch (const std::exception &ex) {
        std::cerr << "error: " << ex.what() << '\n';
        return 1;
    }
}
