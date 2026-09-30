# json-mini

Small **C++17** JSON library and CLI: parse, validate, and pretty-print JSON with clear line/column errors.

No third-party dependencies.

## Build

```bash
make
```

Windows (MinGW/LLVM):

```bat
build.bat
```

## Usage

```bash
./json-mini --pretty --file samples/example.json
./json-mini --validate --file samples/example.json
```

```bash
echo "{\"a\":[1,true,null]}" | ./json-mini --pretty
```

Windows:

```bat
build\json-mini.exe --pretty --file samples\example.json
build\json-mini.exe --validate --file samples\example.json
```

Windows PowerShell:

```powershell
.\build\json-mini.exe --pretty --file samples\example.json
.\build\json-mini.exe --validate --file samples\example.json
```

```powershell
'{"a":[1,true,null]}' | .\build\json-mini.exe --pretty
Get-Content samples\example.json -Raw | .\build\json-mini.exe --pretty
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

- `0` â€” success (`--validate` prints nothing)
- `1` â€” parse / runtime error
- `2` â€” usage or I/O error

## License

MIT
