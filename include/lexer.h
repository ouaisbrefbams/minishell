
#include "minishell.h"

char        **lexer(char *input);
int         lexer_sep(char input);
int         lexer_verif_entre_cote(char *input, int i);
int         lexer_verif_entre_cote22(char *input, int i);
int         lexer_count_nb_element(char *input);
int 		lexer_count_len_element(char *input, int i);
char 		**lexer_malloc_len_elem(char *input, int i, char **out);
