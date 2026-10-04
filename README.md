# JSON Parser (C++)

A command-line C++ application for parsing, validating, modifying, and searching within JSON documents. Built using an object-oriented architecture and an extensible tree-based type hierarchy (`JsonValue`).

---

## Project Structure

* **`Tokenizer/`**: Reads the raw file text and breaks it down into basic tokens (brackets, strings, numbers, etc.).
* **`Json/`**:
  * `JsonParser`: Reads the tokens and builds the JSON tree in memory.
  * `JsonValue`: Base class representing any JSON element.
  * Derived Types: Concrete JSON types — `JsonObject`, `JsonArray`, `JsonString`, `JsonNumber`, `JsonBool`, and `JsonNull`.
* **`Commands/`**: Handles user input and executes CLI commands (open, save, edit, search).
* **`SplitString/`**: Helper utilities for string splitting and path navigation (e.g. `user/name`).
---

## Path Syntax (`<path>`)

* Hierarchy levels are separated by forward slashes (`/`), e.g., `academic/courses/0`.
* Array elements are indexed using integers (`0`, `1`, `2`, ...).

---

## Commands

| Command | Description | Example |
| :--- | :--- | :--- |
| `open <file>` | Loads and parses a JSON file into memory | `open test.json` |
| `close` | Closes the open file and frees allocated memory | `close` |
| `print` | Displays formatted JSON structure on the screen | `print` |
| `save [<path>]` | Saves the document or subpath to the current file | `save academic/grades` |
| `saveAs <file> [<path>]` | Saves the document or subpath to a new JSON file | `saveAs backup.json` |
| `search <key>` | Recursively finds all values matching the given key | `search faculty_number` |
| `set <path> <value>` | Updates an existing value at the specified path | `set student "Ivan Ivanov"` |
| `create <path> <value>` | Creates a new value along with any missing parent path | `create student/grades/0 6` |
| `deletePath <path>` | Removes an element or key-value pair at the path | `deletePath profile/is_first_year` |
| `help` | Shows available commands and syntax rules | `help` |
| `exit` | Frees all resources and terminates the application | `exit` |

---

## Build & Run

### CMake:
```bash
cmake -B build -S .
cmake --build build
./build/JSONParcer
