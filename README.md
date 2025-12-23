# POSIX Shell Implementation  

## Overview   
The shell can execute system commands, manage foreground/background processes, and supports built-in commands like `cd`, `pwd`, `echo`, `ls`, and others.  

## Features Implemented  
- Custom shell prompt with username, system name, and current directory.  
- Built-in commands: `cd`, `pwd`, `echo`, `ls`.  
- Execution of system commands in both foreground and background.  
- Background process handling (prints PID of created process).  
- `pinfo` to display process information.  
- `search` command for recursive file/directory search.  
- Input/output redirection (`<`, `>`, `>>`).  
- Pipelines (`|`) and redirection with pipelines.  
- Signal handling (`CTRL-C`, `CTRL-Z`, `CTRL-D`).  
- Autocomplete for commands and files/directories.  
- Command history with support for up/down arrow keys.  

## Files  
- `main.cpp` – entry point of the shell.  
- `cd.cpp` – implementation of `cd`.  
- `pwd.cpp` – implementation of `pwd`.  
- `echo.cpp` – implementation of `echo`.  
- `ls.cpp` – implementation of `ls`.  
- `pinfo.cpp` – implementation of `pinfo`.  
- `search.cpp` – implementation of `search`.  
- `redirection.cpp` – input/output redirection handling.  
- `pipeline.cpp` – implementation of pipes.  
- `process.cpp` – process creation and background/foreground handling.  
- `history.cpp` – command history management.  
- `utils.cpp` – helper functions.  
- `allFunctions.h` – header file containing function declarations.  
- `makefile` – compiles the project.  
- `README.md` – project documentation.  

## How to Compile  
```bash
make 

