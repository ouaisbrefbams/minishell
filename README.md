# minishell [![Build Status](https://travis-ci.com/HappyTramp/minishell.svg?token=ZspKVUuPUQ73ZVD6J2w4&branch=master)](https://travis-ci.com/HappyTramp/minishell)

minishell project of school 42

## Testing

```
>make test
```

## Documentation

Generate with `make doc` (clean with `make doc_clean`).
You can then read the man pages in ./doc/man or open ./doc/html/index.html in your browser.

## TODO

### Mandatory

- [ ] Show a prompt when waiting for a new command
- [x] Search and launch the right executable (based on the *PATH* variable or by using relative or absolute path) like in bash
- [ ] It must implement the builtins like in bash:
	- [ ] `echo` with option `-n`
	- [ ] `cd` without `-` option
	- [ ] `pwd` without any options
	- [ ] `export` without any options
	- [ ] `unset` without any options
	- [ ] `env` without any options and any arguments
	- [ ] `exit` without any options
- [ ] `;` in the command should separate commands like in bash
- [ ] `'` and `"` should work like in bash except for multiline commands
- [ ] Redirections `<` `>` `>>` should work like in bash except for file descriptor aggregation
- [ ] Pipes | should work like in bash
- [ ] Environment variables (`$` followed by characters) should work like in bash
- [ ] `$?` should work like in bash
- [ ] `ctrl-C`, `ctrl-D` and `ctrl-\` should have the same result as in bash

### Bonus

- [ ] Redirection `<<` like in bash
- [ ] History and line editing with Termcaps (`man tgetent` for examples)
	- [ ] Edit the line where the cursor is located.
	- [ ] Move the cursor left and right to be able to edit the line at a specific location. Obviously new characters have to be inserted between the existing ones similarly to a classic shell.
	- [ ] Use up and down arrows to navigate through the command history which we will then be able to edit if we feel like it (the line, not the history).
	- [ ] Cut, copy, and/or paste all or part of a line using the key sequence you prefer.
	- [ ] Move directly by word towards the left or the right using ctrl+LEFT and ctrl+RIGHT.
	- [ ] Go directly to the beginning or the end of a line by pressing `home` and `end`.
	- [ ] Write AND edit a command over a few lines. In that case, we would love that ctrl+UP and ctrl+DOWN allow to go from one line to another in the command while remaining in the same column or otherwise the most appropriate column.
- [ ] &&, || with parenthesis for priorities, like in bash
- [ ] wilcard * like in bash
