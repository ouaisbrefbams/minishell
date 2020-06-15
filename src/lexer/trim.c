
#include "lexer.h"

char                *del_space(char *str)
{
    int             i;
    char            *s;

    i = 0;
    while(str[++i] != '\0')
    {
        if(str[i] == '\\' && str[i + 1] == ' ')
            i += 2;
        if(str[i] == ' ')
            break;
    }
    s = ft_strsubf(str, 0, i);
    return(s);
}

char                *del_quote(char *str)
{
    int             i;
    char            *s;

    i = 1;
    while(str[++i] != '\0')
        if (str[i] == '\'' || str[i] == '"')
            break;
    s = ft_strsubf(str, 1, i - 1);
    return (s);
}

t_ftlst             *lexe_trim_out(t_ftlst *lst)
{
    t_ftlst        *first;
    t_token        *tk;

    first = lst;
    while(lst != NULL)
    {
        tk = lst->data;
        if (tk->tag >= TAG_STR_DOUBLE || tk->tag >= TAG_STR_SINGLE)
        {
            tk->content = del_quote(tk->content);
            if(lst->next == NULL)
                if (tk->tag & TAG_STICK)
                    tk->tag -= TAG_STICK;
        }
        else
        {
            tk->content = del_space(tk->content);
            if(lst->next == NULL)
                if (tk->tag & TAG_STICK)
                    tk->tag -= TAG_STICK;
        }
        lst = lst->next;
    }
    return (first);
}
