# Data Fitting Utilities with d-Spline

[English](README.md) | [日本語](README-ja.md)

These utilities fit the input data appropriately using the d-Spline function.  
These utilities are a pair of libraries and tools.

README will be updated as appropriate.

## Preparing

### Creating Files

Run the `make` command in the root directory.  
The necessary files will be created automatically.

``` bash
make
```

### Setting Environment Variable

Add the necessary path to your PATH environment variable.  
You can set them automatically using setup_env.sh, which is created automatically in the "Creating Files" section.

``` bash
source setup_env.sh
```

## File structure

- dspline
  - bin  
  Executable files
  - include  
  Header files for C codes
  - lib  
  Shared library files
  - src  
  Source files for building the libraries
  - tool  
  The C code `main.c` for building the tools (DO NOT CHANGE)
  - work  
  Working directory for building
  - Makefile  
  `Makefile` for building the tools (called automatically)

## What is "Data fitting"

Experimental data generally contain measurement errors.  
Data fitting separates measurement errors from the input data and retrieves the original function structure.

## Latest Release

Refer to the [Release Note](CHANGELOG.md) for past updates.

### Version 0.1.2+docs (2025-03-09)

- Other Updates
  - Added the English version of the README
- Known Issues
  - Using the two-phase model, there are some cases that the utilities output results that seems a bit off to the human eye (2025-03-09: under investigation)
