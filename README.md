
# ez
experimental build sytem for C, built in C, and bootstrapped in C. around 1k LOC currently (sep 22, 2026)

builds with gcc manually

support for windows and linux :D

proudly **NOT vibecoded!**
# Dependencies
None!

# Example

to compile ez using ez, the following command can be run for linux;
`ez --include include --source-dir src --d LOGERROR --d DEBUG --d LOGTRACE --d LOGINFO --d LOGWARN --d LOGCOLORED
 --d LOGSUCCESS --d Linux --exclude-file src/io/windows11.c --o ez2`

in this example we define our source dir as `/src/` and our include directory as `/include/`. We also define multiple logging levels with the `--d` flag. Lastly, the `src/io/windows11.c` file is excluded through `--exclude-file` (even though header guards prevent conflicts anyways).

this outputs a file called `ez2`.

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
2. simply use your favorite C compiler:

gcc on linux: "`gcc src/error/state.c src/generator.c src/io/file.c src/io/windows11.c src/main.c src/tests.c src/io/linux.c -Iinclude -DLOGERROR -DDEBUG -DLOGTRACE -DLOGINFO -DLOGWARN -DLOGCOLORED -DLOGSUCCESS -DLOGDEBUG -DLinux -O3 -o ez`"

gcc on windows: "`gcc src/error/state.c src/generator.c src/io/file.c src/io/windows11.c src/main.c src/tests.c src/io/linux.c -Iinclude -DLOGERROR -DDEBUG -DLOGTRACE -DLOGINFO -DLOGWARN -DLOGCOLORED -DLOGSUCCESS -DLOGDEBUG -DWindows11 -O3 -o ez`"
