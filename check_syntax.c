#include "minishell.h"

int find_file(int i, char *a)
{
	int state = 0;
	while (i)
	{
		if (ft_isalnum(a[i]) || a[i] == '\'' || a[i] == '\"')
			state = 1;
		i--;
	}
	return (state);
}

int print_error(int cho)
{
	if (cho == 0)
		print_errors("minishell: syntax error ");
	else if (cho == 1)
		print_errors("minishell: unexpected EOF while looking for matching");
	return 0;
}
 
int check_syntax(char *a)
{
	int i = 0;
	int quotes = 0;
	int prev_symbol = 0;

	while (a[i])
	{
		if (a[i] == ' ' && !a[i + 1] && !quotes && prev_symbol)
			return (print_error(0));
		if (a[i] == '"' || a[i] == '\'')
		{
			if (!quotes)
				quotes = a[i];
			else if (quotes == a[i])
				quotes = 0;
		}
		else if (!quotes && (a[i] == '<' || a[i] == '>' || a[i] == '|'))
		{
			if (i == 0 || a[i + 1] == '\0' || a[i - 1] == '|')
			{
				if (i == 0 && a[i] != '<' && a[i] != '>')
					return(print_error(0));
				else if (!a[i + 1])
					return (print_error(0));
			}
			else if (a[i] == '<' && a[i + 1] == '>')
				return (print_error(0));
			else if (a[i] == '>' && a[i + 1] == '<')
				return (print_error(0));
			else if (a[i] == '|' && !find_file(i, a))
				return (print_error(0));
			else
				prev_symbol = 1;
		}
		else
			prev_symbol = 0;
		i++;
	}
	if (quotes != 0)
		return (print_error(1));
	return 1;
}
