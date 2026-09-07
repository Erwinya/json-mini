# json-mini

Small **C++17** JSON library and CLI: parse, validate, and pretty-print JSON with clear line/column errors.

No third-party dependencies.

## Status

Scalar parse and stringify are available (`null`, `boolean`, `number`, `string`).  
Array/object parsing, CLI, samples, and build scripts will land in follow-up commits.

## Library (so far)

```cpp
#include "json_mini.hpp"

auto v = jsonmini::parse("\"hello\"");
std::cout << v.stringify();           // "hello"
std::cout << jsonmini::parse("3.14").as_number();
```

`ParseError` reports line and column for invalid input. Arrays/objects are rejected by `parse` for now (constructed `Value::array()` / `Value::object()` can still `stringify`).

## Requirements

- C++17 compiler (g++ / clang++ / MSVC)

## License

MIT
