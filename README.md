# json-mini

Small **C++17** JSON library and CLI: parse, validate, and pretty-print JSON with clear line/column errors.

No third-party dependencies.

## Status

`Value` container types are in place (`Null`, `Bool`, `Number`, `String`, `Array`, `Object`).  
Parser, stringify, CLI, samples, and build scripts will land in follow-up commits.

## Library (so far)

```cpp
#include "json_mini.hpp"

jsonmini::Value n;                 // null
jsonmini::Value b(true);
jsonmini::Value x(3.14);
jsonmini::Value s(std::string{"ok"});
auto arr = jsonmini::Value::array();
auto obj = jsonmini::Value::object();
```

## Requirements

- C++17 compiler (g++ / clang++ / MSVC)

## License

MIT
