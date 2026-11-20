@echo off
setlocal
where g++ >nul 2>&1
if errorlevel 1 (
  echo g++ not found. Install LLVM-MinGW or MSYS2, then re-run build.bat
  exit /b 1
)
if not exist build mkdir build
g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 -Iinclude src\main.cpp src\json_value.cpp -o build\json-mini.exe
if errorlevel 1 exit /b 1
echo Built build\json-mini.exe

if /I "%~1"=="test" (
  g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 -Iinclude tests\test_json_mini.cpp src\json_value.cpp -o build\test_json_mini.exe
  if errorlevel 1 exit /b 1
  build\test_json_mini.exe
  if errorlevel 1 exit /b 1
  echo Tests passed.
  goto :eof
)

echo Example: build\json-mini.exe --pretty --file samples\example.json
echo Test:    build.bat test
endlocal
