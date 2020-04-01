#ifndef AST_H
# define AST_H

/**
** \file     ast.h
** \brief    AST structs
*/

# include <stdlib.h>
# include <stdbool.h>
# include "libft_mem.h"
# include "libft_util.h"

/**
** \brief            Separator type
** \param SEP_END    Regular command end `;`
** \param SEP_PIPE   Pipe output of left to right `|`
** \param SEP_AND    Execute right if left status == 0 `&&`
** \param SEP_OR     Execute right if left status != 0 `||`
*/

typedef enum
{
	SEP_END,
	SEP_PIPE,
	SEP_AND,
	SEP_OR,
}				t_sep;

struct s_ast;

/**
** \brief            Line struct
** \param left       AST to the left of separator
** \param right      AST to the right of separator
** \param sep        Type of separator
*/

typedef struct
{
	struct s_ast		*left;
	struct s_ast		*right;
	t_sep		sep;
}				t_line;

/**
** \brief            Command struct
** \param argv       Array of string, all arguments beginning with executable name
** \param in         STDIN redirection filename
** \param out        STDOUT redirection filename
** \param is_append  True if out redirection is append to file
*/

typedef struct
{
	char		**argv;
	char		*in;
	char		*out;
	bool		is_append;
}				t_cmd;

/**
** \brief            AST node tag (type)
** \param TAG_CMD    Command AST node
** \param TAG_LINE   Line AST node
*/

typedef enum
{
	TAG_CMD,
	TAG_LINE,
}   			t_ast_tag;

/**
** \brief             AST node struct
** \param tag         Node tag
** \param data        Union containning possible node data
** \param data::cmd   Command struct
** \param data::line  Line struct
*/

typedef struct	s_ast
{
	t_ast_tag	tag;
	union
	{
		t_line	line;
		t_cmd	cmd;
	}			data;
}				t_ast;

t_ast			*ast_new(t_ast_tag tag, void *data);
void			ast_destroy(t_ast *ast);

#endif
