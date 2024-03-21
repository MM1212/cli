# CLI

Command line interface to parse flags, switches and arguments.

It has an OOP-like interface to describe flags and switches (options) and arguments.

---

## Dependencies/Submodules

- [Libft](https://github.com/MM1212/42-libft)

## Installation

*It is better to use this as a submodule of your project.*

```bash
git submodule add git@github.com:MM1212/42-cli.git libs/cli
git submodule update --init --recursive
```

---

## Usage

### Bash
```bash
# Compile
make
# Cleanup
make clean/fclean
# Recompile
make re
```

---

### Makefile
```Makefile
CLI_PATH = libs/cli
CLI_BIN = $(addprefix $(CLI_PATH)/,bin)
CLI_INCLUDES = $(addprefix $(CLI_PATH)/,includes/)
CLI_ARCH = $(addprefix $(CLI_BIN)/,cli.a)

$(CLI_ARCH):
  @make -C $(CLI_PATH)
```
