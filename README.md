# Nova Language

## About Nova Language

Nova is a simple, educational programming language designed to introduce children and beginners to the fundamental concepts of compilers and programming languages without the complexities often found in mainstream languages. Its syntax is straightforward and intuitive, making it an ideal tool for learning how code is parsed, interpreted, and executed.

Nova aims to demystify the compilation process, allowing learners to focus on core programming logic and the underlying mechanics of a language system. The Supernova compiler itself is built using C++11.

## Syntax Overview

Nova's syntax is designed for clarity and ease of learning. Here are some basic examples:

### Data Types

Nova supports the following fundamental data types:

*   `num`: Integer numbers (e.g., `10`, `-5`).
*   `string`: Sequences of characters (e.g., `"Hello World"`).
*   `bool`: Boolean values (either `true` or `false`).
*   `float`: Floating-point numbers (e.g., `3.14`, `0.5`).
*   `char`: Single characters (e.g., `'A'`, `'z'`).

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

git clone https://github.com/akram-dris/nova_lang.git
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

If your program has runtime errors, the `supernova` compiler will now crash and report the error message, for example:

```
Runtime Error: Division by zero.
```

### Example Output from `test.nv`

Here's the output when running the `test.nv` file, which demonstrates various features of the Nova language:

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

### Adding Supernova to your PATH

To use the `supernova` compiler from any directory without specifying its full path, you can add its `build` directory to your system's PATH environment variable.

#### Linux & macOS

Open your shell's configuration file (e.g., `~/.bashrc`, `~/.zshrc`, or `~/.profile`) and add the following line:

```bash
export PATH="/path/to/your/nova_lang/build:$PATH"
```

Replace `/path/to/your/nova_lang` with the actual absolute path to your cloned `nova_lang` directory. After saving the file, restart your terminal or run `source ~/.bashrc` (or your respective config file) for the changes to take effect.

#### Windows

1.  **Find the absolute path:** Navigate to your `nova_lang` directory and note down the full path (e.g., `C:\Users\YourUser\Documents\nova_lang`). The `supernova.exe` will be in the `build` subdirectory (e.g., `C:\Users\YourUser\Documents\nova_lang\build`).

2.  **Open Environment Variables:**
    *   Search for "Environment Variables" in the Windows search bar and select "Edit the system environment variables."
    *   Click the "Environment Variables..." button.

3.  **Edit Path Variable:**
    *   Under "User variables for <YourUser>" (or "System variables" if you want it for all users), find and select the `Path` variable.
    *   Click "Edit...".
    *   Click "New" and add the absolute path to your `nova_lang\build` directory (e.g., `C:\Users\YourUser\Documents\nova_lang\build`).
    *   Click "OK" on all open windows to save the changes.

4.  **Verify:** Open a new Command Prompt or PowerShell window and type `supernova`. If it shows an error about missing arguments (e.g., "Usage: supernova <source-file>"), it means it's successfully added to your PATH.
