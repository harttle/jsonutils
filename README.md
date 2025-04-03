# JsonUtils

A command-line JSON processing utility that provides various JSON manipulation tools.

## Features

- Format JSON with proper indentation
- Query JSON using JSONPath syntax
- Validate JSON syntax
- Transform JSON structure

## Installation

### From Source

```bash
mkdir build && cd build
cmake ..
make
sudo make install
```

### Homebrew

```bash
brew install jsonutils
```

### AUR (Arch User Repository)

```bash
yay -S jsonutils
```

## Usage

JsonUtils reads from stdin and writes to stdout, making it easy to use in command pipelines.

### Format JSON

```bash
echo '{"name":"John","age":30}' | jsonutils format
```

### Query JSON

```bash
echo '{"user":{"name":"John","age":30}}' | jsonutils query '$.user.name'
```

### Validate JSON

```bash
echo '{"valid": true}' | jsonutils validate
```

### Transform JSON

```bash
echo '{"old_key": "value"}' | jsonutils transform '{"new_key": "$.old_key"}'
```

## Building from Source

### Prerequisites

- CMake 3.15 or higher
- C++17 compliant compiler
- Git (for fetching dependencies)
- jq development files (libjq-dev on Ubuntu/Debian, jq on macOS via Homebrew)

### Build Steps

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/jsonutils.git
   cd jsonutils
   ```

2. Create build directory:
   ```bash
   mkdir build && cd build
   ```

3. Configure and build:
   ```bash
   cmake ..
   make
   ```

4. Run tests:
   ```bash
   ctest
   ```

5. Install:
   ```bash
   sudo make install
   ```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details. 