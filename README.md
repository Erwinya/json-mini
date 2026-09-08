# json-mini

Small **C++17** JSON library and CLI: parse, validate, and pretty-print JSON with clear line/column errors.

No third-party dependencies.

## Status

Full value parsing is available: scalars, arrays, and objects, plus stringify with optional pretty-print.  
CLI, samples, and build scripts will land in follow-up commits.

## Library (so far)

```cpp
#include "json_mini.hpp"

auto v = jsonmini::parse(R"({"ok":true,"n":[1,2]})");
std::cout << v.stringify(true);
```

`ParseError` reports line and column for invalid input.

## Requirements

- C++17 compiler (g++ / clang++ / MSVC)

## License

MIT
