# json-mini

Small **C++17** JSON library and CLI: parse, validate, and pretty-print JSON with clear line/column errors.

No third-party dependencies.

## Status

Project scaffolding is in place. Parser, CLI, samples, and build scripts will land in follow-up commits.

## Goals

- Header + source library (`jsonmini::parse` / `stringify`)
- CLI for stdin or `--file`
- Pretty-print and validation modes
- Clear parse errors with line and column

## Requirements

- C++17 compiler (g++ / clang++ / MSVC)
- `make` (Unix) or `build.bat` (Windows)

## License

MIT
