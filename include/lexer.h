#include "minishell.h"

// just to make iterpolation compile
//////////////////////////////////////
enum e_token_tag
{
	LTAG_STR,
	LTAG_STR_SINGLE,
	LTAG_STR_DOUBLE,
};

typedef struct
{
	enum e_token_tag	tag;
	char				*content;
}						t_token;

t_token		*token_new(enum e_token_tag tag, char *content);
//////////////////////////////////////

char        **lexer(char *input);
int         lexer_sep(char input);
int         lexer_verif_entre_cote(char *input, int i);
int         lexer_verif_entre_cote22(char *input, int i);
int         lexer_count_nb_element(char *input);
int 		lexer_count_len_element(char *input, int i);
char 		**lexer_malloc_len_elem(char *input, int i, char **out);
