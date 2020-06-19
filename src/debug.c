
#include <stdio.h>
#include "lexer.h"
#include "ast.h"

void token_debug(void *v)
{
	t_token *t;

	t= v;
	printf("[%4d %d] (%s)\n", t->tag, !!(t->tag & TAG_STICK), t->content);
}

void token_put(void *v)
{
	t_token *t;

	t= v;
	printf("%s ", t->content);
}

void print_level(int level)
{
	while (level-- > 0)
		printf("  ");
}

void ast_print(int level, t_ast *ast)
{
	if (ast->tag == AST_CMD)
	{
		print_level(level);
		printf("cmd: [ ");
		ft_lstiter(ast->cmd_argv, token_put);
		printf(" ] redirs: [");
		ft_lstiter(ast->redirs, token_put);
		printf(" ]");
	}
	else
	{
		/* printf("SEP: %d\n", ast->op.sep); */
		print_level(level);
		printf("redirs: [");
		ft_lstiter(ast->redirs, token_put);
		printf(" ] ");
		printf("{\n");

		print_level(level);
		printf(" left:\n");
		ast_print(level + 1, ast->op.left);

		printf("\n");
		print_level(level);
		printf(" right:\n");
		ast_print(level + 1, ast->op.right);

		printf("\n");
		print_level(level);
		printf("}\n");
	}
}
