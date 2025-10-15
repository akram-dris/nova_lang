# Nova Language

## About Nova Language

Nova is a simple, educational programming language designed to introduce children and beginners to the fundamental concepts of compilers and programming languages without the complexities often found in mainstream languages. Its syntax is straightforward and intuitive, making it an ideal tool for learning how code is parsed, interpreted, and executed.

Nova aims to demystify the compilation process, allowing learners to focus on core programming logic and the underlying mechanics of a language system.

## Syntax Overview

Nova's syntax is designed for clarity and ease of learning. Here are some basic examples:

### Variable Declaration and Assignment

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

### Arithmetic Operations

```nova
result:num = 5 + 3 * 2
area:float = pi * radius * radius
```

### Conditional Statements

```nova
if x > 5 start
    show "x is greater than 5"
end else
    show "x is not greater than 5"
end
```

### Loops

```nova
akram:num = 0
while akram < 3 start
    show akram
    akram = akram + 1
end
```

### Functions

```nova
fun:num add a:num b:num start
    return a + b
end

result:num = add(3, 4)
show result
```

## Installation

To get started with the Nova language, you'll need a C++ compiler (like g++) installed on your system. Follow the instructions below for your specific operating system.

### Prerequisites

*   **Git:** To clone the repository.
*   **C++ Compiler:** A C++11 compatible compiler (e.g., g++, Clang).

### Linux & macOS

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/your-username/nova_lang.git # Replace with actual repo URL
    cd nova_lang
    ```

2.  **Build the compiler:**
    Use the provided `build.sh` script to compile the Supernova compiler:
    ```bash
    chmod +x build.sh
    ./build.sh
    ```
    This will create an executable named `supernova` in the `build/` directory.

### Windows

1.  **Install MinGW-w64 (or similar C++ toolchain):**
    If you don't have a C++ compiler, MinGW-w64 is a popular choice for Windows. Download and install it, ensuring `g++` is added to your system's PATH.

2.  **Clone the repository:**
    Open Git Bash or your preferred terminal and clone the repository:
    ```bash
    git clone https://github.com/your-username/nova_lang.git # Replace with actual repo URL
    cd nova_lang
    ```

3.  **Build the compiler:**
    You can manually run the compilation command in your terminal (e.g., Git Bash, PowerShell, or Command Prompt):
    ```bash
    mkdir build
    g++ src/main.cpp src/lexer.cpp src/parser.cpp -o build/supernova -std=c++11
    ```
    Alternatively, you can execute the `build.sh` script using a Unix-like environment (like Git Bash):
    ```bash
    ./build.sh
    ```

## Usage

Once the `supernova` compiler is built, you can use it to compile and run your Nova source files.

To compile and run a Nova source file (e.g., `my_program.nv`):

```bash
./build/supernova my_program.nv
```

If your program has runtime errors, the compiler will now crash and report the error message, for example:

```
Runtime Error: Division by zero.
```
