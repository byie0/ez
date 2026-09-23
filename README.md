
# ez
experimental build sytem for C, built in C, and bootstrapped in C. around 1k LOC currently (sep 22, 2026)

builds with gcc manually

only works on Windows, soon will work on linux too :D

proudly **NOT vibecoded!**
# Dependencies
None!

## Current Features
- Recursive Source Discovery
	- Traverses source directories through the operating system's native API
- LSP (clangd) support
	- automatically generates `compile_commands.json`
- Command-Line configuration
	- can  build without the need for makefiles through flags (see usage below)
- File exclusion
	- explicit file exclusion using the `--exclude-file`flag
- Compilation as a subprocess
	- Directly spawns the compiler process as a child and monitors its execution
- Custom logging system
	- multi-level logging macros with (optional) color coding 
- Thread-safe error state
	- uses an atomic global error tracking system (sometimes, otherwise just prints errors) // to fix!

## Usage
either run the executable directly or add it to PATH. call `ez` from the command-line with the possible flags:
- `--include <DIRECTORY>` adds an include directory
-  `--source-dir <DIRECTORY>` adds source directory; recursively scanning all subfolders and adding every source file to the compilation targets
-  `--source-file <FILE_PATH>` adds a file to the compilation targets
 - `--exclude-file <FILE_PATH>` removes a file from the compilation targets if it is present. Does nothing otherwise.
- `--d <TERM>`, `--define <TERM>`,  defines a term globally (equivalent to a global `#define <TERM>`. 
- `--o <OUTPUT_FILE_PATH>`, `--out <OUTPUT_FILE_PATH>`,  sets the output file path/name. 


## Future Plans
1. Add support for a configuration file to remove the need for flags every run and configuration (like color-coded logging)
2. Implement explicit error handling and fix memory leaks here and there...


## Building
1. git clone this repo
2. simply use your favorite C compiler! if you already have gcc installed: "`gcc src/error/state.c src/generator.c src/io/file.c src/io/windows11.c src/main.c src/tests.c src/io/linux.c -Iinclude -DLOGERROR -DDEBUG -DLOGTRACE -DLOGINFO -DLOGWARN -DLOGCOLORED -DLOGSUCCESS -DLOGDEBUG -o build/ez`"
