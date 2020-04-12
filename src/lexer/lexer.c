
#include "lexer.h"


char        **lexer(char *input)
{
	char **out;
	int i;

	if (!input)
		return (0);
	i = lexer_count_nb_element(input);
	if (!(out = malloc(sizeof(char *) * i + 1)))
        return (0);
	out[i + 1] = NULL;
	out = lexer_malloc_len_elem(input,i, out);
	return(out);

}
