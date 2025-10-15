# Nova Language (`nova_lang`) & Supernova Compiler

## Overview

**Nova** is a simple, educational, **compiled programming language** designed to teach beginners and students the fundamentals of programming and compilers. Its **clean and intuitive syntax** allows learners to focus on the *logic of programming* and the *mechanics of compilation* without unnecessary complexity.

The official **Supernova Compiler** is a **C++11 implementation** that parses, compiles, and executes Nova source files (`.nv`), bridging the gap between beginner-friendly syntax and real-world compilation.

---

## Key Features

* **Compiled language** — Nova is executed via the Supernova Compiler.
* **Beginner-friendly syntax** — simple, intuitive, and readable.
* **Core data types:** `num`, `string`, `bool`, `float`, `char`.
* **Control structures:** `if`, `while`, and function support.
* **Optional parameters:** Functions can define parameters with default values.
* **Cross-platform support:** Linux, macOS, and Windows.
* **Educational focus:** teaches compiler concepts in a hands-on way.

---

## Syntax Examples (Nova Language)

### Variable Declaration

```nova
x:num = 10
name:string = "Alice"
is_active:bool = true
pi:float = 3.14
grade:char = 'A'
```

### Output

```nova
show "Hello, Nova"
show x
```

### Loops

```nova
counter:num = 0
while counter < 3 start
    show counter
    counter = counter + 1
end
```

### Functions

```nova
fun:num add a:num b:num start
    return a + b
end

result:num = add a:3 b:4
show result

// Function with optional parameter
fun:num multiply a:num b:num c:num = 1 start
    return a * b * c
end

show multiply a:2 b:3          // c defaults to 1 (result: 6)
show multiply a:2 b:3 c:4      // c is 4 (result: 24)
```

---

## Installation & Building Supernova

### Prerequisites

* **Git** — to clone the repository.
* **C++11 Compiler** — `g++` or `clang`.

---

### Linux & macOS

```bash
git clone https://github.com/akram-dris/nova_lang.git
cd nova_lang
chmod +x build.sh
./build.sh
```

The `supernova` executable will be available in the `build/` directory.

---

### Windows

1. Install MinGW-w64 (or another C++ compiler). Ensure `g++` is in your PATH.
2. Clone the repository:

```bash
git clone https://github.com/akram-dris/nova_lang.git
cd nova_lang
```

3. Build manually:

```bash
mkdir build
g++ src/main.cpp src/lexer.cpp src/parser.cpp -o build/supernova -std=c++11
```

Or run the build script via Git Bash:

```bash
./build.sh
```

---

## Usage

Compile and run Nova programs with the **Supernova Compiler**:

```bash
./build/supernova my_program.nv
```

If a runtime error occurs, Supernova reports it clearly:

```
Runtime Error: Division by zero.
```

---

## Example Output (`test.nv`)

```
3
10
7
10
14
4
9
Nova
Hello, Nova
hello world
y is 10
y is not 10
x is not greater than y
7
1
Hello, Nova
2 >= 2 is true
1 <= 2 is true
hello
true
is_active is true
false
3.14
6.28
A
J
12.8
6.5
10
4
5
Pi is greater than 3.0
Runtime Error: Division by zero.
```

---

## Add Supernova to PATH

### Linux & macOS

```bash
export PATH="/absolute/path/to/nova_lang/build:$PATH"
source ~/.bashrc
```

### Windows

1. Find the `nova_lang\build` folder path.
2. Add it to **Environment Variables → Path**.
3. Open a new terminal and type:

```bash
supernova
```

You should see the compiler usage instructions.

---

## Project Structure

```
nova_lang/
├── src/             # Source files for Supernova Compiler
│   ├── main.cpp
│   ├── lexer.cpp
│   ├── parser.cpp
│   └── ...
├── build/           # Compiled Supernova executable
├── examples/        # Sample Nova programs
├── build.sh         # Build script
└── README.md
```
