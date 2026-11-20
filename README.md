# json-mini

Small **C++17** JSON library and CLI: parse, validate, and pretty-print JSON with clear line/column errors.

No third-party dependencies.

## Status

Library, CLI, unit tests, and CI. Parser follows a practical RFC 8259 subset: strict whitespace, `\uXXXX` / surrogate pairs, duplicate-key rejection, and finite number serialization.

## Build

```bash
make
make test
```

Windows (MinGW/LLVM):

```bat
build.bat
build.bat test
```

## Usage

```bash
./json-mini --pretty --file samples/example.json
./json-mini --validate --file samples/example.json
```

Windows PowerShell:

```powershell
.\build\json-mini.exe --pretty --file samples\example.json
.\build\json-mini.exe --validate --file samples\example.json
'{"a":[1,true,null]}' | .\build\json-mini.exe --pretty
```

`--validate` parses the input and exits `0` on success without printing the value.

## Library

```cpp
#include "json_mini.hpp"
auto v = jsonmini::parse(text);
std::cout << v.stringify(true);
```

`ParseError` reports line and column for invalid input.

## Exit codes

- `0` — success (`--validate` prints nothing)
- `1` — parse / runtime error
- `2` — usage or I/O error

## License

MIT
