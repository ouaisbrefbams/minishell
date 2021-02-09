# minishell ![norminette](https://github.com/ouaisbrefbams/minishell/workflows/norminette/badge.svg)

minishell project of school 42.

## Authors

* [nahaddac](https://github.com/nass1pro)
* [cacharle](https://github.com/cacharle)

## Usage

```sh
$ make
$ ./minishell
```

## Testing

Done with [minishell\_test](https://github.com/cacharle/minishell_test).

```sh
$ git submodule update minishell_test
$ cd minishell_test
$ ./run --help
$ ./run
```

## Documentation

Install [Doxygen](https://www.doxygen.nl/index.html)
Generate with `make doc` (clean with `make doc_clean`).  
You can then read the man pages in `./doc/man` or open `./doc/html/index.html` in your browser.
