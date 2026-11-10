# C++ Command-Line Interpreter

## Project overview

An interactive C++17 command interpreter implementing a small, deliberately restricted shell-like language. The project separates parsing, command construction, stream routing and command execution, and uses polymorphic command classes connected through standard C++ streams.

## Academic context

Developed as university coursework to practise object-oriented design, parsing, resource ownership, file I/O and composition of processing stages. It is not intended to be POSIX-compatible and does not start arbitrary operating-system processes.

## Main functionality

- Built-in commands: `echo`, `date`, `time`, `touch`, `wc`, `prompt`, `truncate`, `rm`, `tr`, `head` and `batch`
- Pipelines using `|`
- Input redirection and output redirection in overwrite or append mode
- Batch-file parsing and execution
- Validation and command-specific error handling
- Interactive prompt with end-of-file termination

## Technical implementation

The interpreter uses a staged design:

1. `Parser` converts a command line into `ParsedCommand` objects, including nested pipeline and batch representations.
2. `CommandBuilder` maps the parsed representation to concrete command objects.
3. `Interpreter` selects standard or file streams and invokes the command.
4. `PipeCommand` connects commands with in-memory string streams; `BatchCommand` parses and executes commands from a file.

`Command` is an abstract base class with a stream-oriented `execute(std::istream&, std::ostream&)` interface. Concrete commands can therefore read from the console, a redirected file or a preceding pipeline stage without changing their core logic. `std::unique_ptr` is used for ownership of commands and temporary streams.

## Build and usage

Requirements: a C++17 compiler and the standard library only.

```bash
g++ -std=c++17 *.cpp -o interpreter
./interpreter
```

On Windows, create a C++ project containing all `.cpp` and `.h` files and enable C++17. End interactive input with `Ctrl+D` on Linux/macOS or `Ctrl+Z`, then Enter, in a Windows console.

File-related commands operate in the current working directory. Test destructive commands such as `rm` and `truncate` only in a disposable directory.

## Repository structure

| Component | Purpose |
| --- | --- |
| `CommandLineInterpreter.cpp` | Program entry point |
| `Parser.*` | Grammar handling and parsed-command representation |
| `CommandBuilder.*` | Construction of concrete and composite commands |
| `Interpreter.*` | Read-evaluate loop and redirection setup |
| `Command.*` | Abstract command interface |
| `PipeCommand.*` | Pipeline composition and intermediate streams |
| `BatchCommand.*` | Batch-file execution |
| Individual `*Command.*` files | Built-in command implementations |
| `Error.*` | Input and command validation |

## Limitations and future improvements

- The grammar is custom and intentionally much smaller than a system shell; quoting, escaping, environment expansion, background jobs and external executables are outside scope.
- Pipelines are evaluated in-process using buffers rather than concurrent OS pipes.
- The repository has no build system, automated tests or usage transcript.
- Compilation could not be independently reproduced during this audit because a C++ compiler was not available in the review environment.
- Priority improvements are a CMake configuration, parser/unit tests, integration tests for pipelines and redirection, and a formal grammar with documented edge cases.


