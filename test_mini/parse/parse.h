

# include <stdlib.h>
# include <stdlib.h>
# include <stdbool.h>


typedef enum		e_sep
{
	SEP_END,
	SEP_PIPE,
	SEP_AND,
	SEP_OR,
}					t_sep;

struct s_ast;

/*
** \brief            Line struct
** \param left       AST to the left of separator
** \param right      AST to the right of separator
** \param sep        Type of separator
*/

typedef struct		s_line
{
	struct s_ast	*left;
	struct s_ast	*right;
	struct s_ast	*parent;
	t_sep			sep;
}					t_line;

/*
** \brief            Command struct
** \param argv       Array of string,
**                   all arguments beginning with executable name
** \param in         STDIN redirection filename
** \param out        STDOUT redirection filename
** \param is_append  True if out redirection is append to file
*/

typedef struct		s_cmd
{
	char			*str;
	char			*in;
	char			*out;
	bool			is_append;
}					t_cmd;

/*
** \brief            AST node tag (type)
** \param TAG_CMD    Command AST node
** \param TAG_LINE   Line AST node
*/

typedef enum		e_ast_tag
{
	TAG_CMD,
	TAG_LINE,
}					t_ast_tag;

/*
** \brief             AST node struct
** \param tag         Node tag
** \param cmd         Command struct
** \param line        Line struct
*/

typedef struct		s_ast
{
	t_ast_tag		tag;
	union
	{
		t_line		line;
		t_cmd		cmd;
	}				;
}					t_ast;

typedef struct s_return
{
    t_ast  *result;
    char   **rest;
}   		   t_return;

t_return				*parse_nw(char **input);
t_return         		*parse(t_return *nw, char **input);

/* utils*/
int          verif_part(char **input, char chr);
