/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkhairou <mkhairou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 13:25:37 by mkhairou          #+#    #+#             */
/*   Updated: 2023/05/06 22:33:07 by mkhairou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	theres_is_red(char *a)
{
	if(!a)
		return(1);
	if(!strcmp(a, "<") || !strcmp(a, "<<") || !strcmp(a, ">") || !strcmp(a, ">>"))
		return(0);
	return(1);
}

void print_errors(char *a)
{
	write(2, a, ft_strlen(a));
	write(2, "\n", 1);
}

long checking_overwrite(t_mshel *shel, int cmd_index)
{
	int fd[2];
	char buffer[1000000];
	ssize_t n;

	pipe(fd);
	if (fork() == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		run_cmd(shel, cmd_index, shel->cmd[cmd_index]->cmd);
		sleep(1);
		exit(0);
	}
	else
	{
		close(fd[1]);
		n = read(fd[0], buffer, 1000000);
		buffer[n] = '\0';
	}
	int i = 0;
	while (shel->cmd[cmd_index]->redirect.out_file[i])
	{
		if (!shel->cmd[cmd_index]->redirect.out_file[i + 1])
			break;
		i++;
	}
	int tmp = open(shel->cmd[cmd_index]->redirect.out_file[i], O_RDONLY);
	ssize_t size = read(tmp, buffer, 1024);
	close(tmp);
	return ((long)n - (long)size);
}

int word_count(char *s)
{
	size_t i;
	int trgr;
	int count;

	i = 0;
	trgr = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] != ' ' && trgr == 0)
		{
			count++;
			trgr = 1;
		}
		if (s[i] == ' ' && trgr > 0)
			trgr = 0;
		i++;
	}
	return (count);
}

char *substr(char const *s, unsigned int start, size_t len)
{
	size_t i;
	size_t j;
	char *a;

	i = 0;
	if (!s)
		return (0);
	j = ft_strlen(s);
	a = (char *)malloc(len + 1);
	if (!a)
		return (NULL);
	while (s[i] && i < len && start < j)
	{
		a[i] = s[i + start];
		i++;
	}
	a[i] = '\0';
	return (a);
}

char **better_parsing(char *a, t_mshel *shel)
{
	int i;
	int j = 0;
	int checkpoint;
	char **new;
	char *tmp;
	i = 0;
	new = ft_calloc(1024,1024);
	int	herdoc_number = 0;

	while (a[i])
	{
		checkpoint = i;
		if (a[i] != 39 && a[i] != 34)
		{
			while (a[i] != '\0' && a[i] != 39 && a[i] != 34 && a[i] != ' ')
				i++;
			if (i > checkpoint)
			{
				if (ft_strchr(substr(a, checkpoint, i - checkpoint), '$'))
				{
					tmp = substr(a, checkpoint, i - checkpoint);
					if (ft_strchr(tmp, '<') ||
						ft_strchr(tmp, '>'))
					{
						int	s = 0;
						while ((tmp[s] != '>' || tmp[s] != '<') && tmp[s])
							s++;
						if(s > 0)
							new[j++] = ft_strtrim(substr(tmp, 0, s), " ");
						if(tmp[s] == '>' )
						{
							if (tmp[s + 1] == '>')
							{
								new[j++] = ">>";
								s++;
							}
							else
								new[j++] = ">";
						}
						else if(tmp[s] == '<' )
						{
							if (tmp[s + 1] == '<')
							{
								new[j++] = "<<";
								s++;
							}
							else
								new[j++] = "<";
						}
						if(tmp[s] && tmp[s + 1])
							new[j++] = ft_strtrim(substr(tmp, s + 1, ft_strlen(tmp) - (s + 1)), " ");
					}
					else
					{
						if (checkpoint != 0 && a[checkpoint - 1] != ' ')
						{
							char *tempo = check_expanding(shel, ft_strtrim(tmp, " "));
							if(tempo)
							{
								if(new[j - 1])
									new[j - 1] = ft_strjoin(new[j - 1], tempo);
								else
								{
									if(ft_strlen(tempo))
									{
										if(j != 0)
											new[j - 1] = ft_strdup(tempo);
										else
											new[j++] = ft_strdup(tempo);
									}
									else
										new[j -1] = NULL;
								}
							}
							if(j != 0 && new[j - 1] && !strcmp(new[j - 1],"<<"))
							{
								shel->exapnd_herdoc[herdoc_number] = 1;
								herdoc_number++;
							}
						}
						else
						{
							char *tempo = check_expanding(shel, ft_strtrim(tmp, " "));
							if(tempo && ft_strchr(tempo,' ') && tempo[ft_strlen(tempo) - 1] != ' ' && ft_strlen(tempo) > 1)
							{
								char **t = ft_split(tempo, ' ');
								int	index = 0;
								if(!t[index])
									new[j++] = ft_calloc(1,1);
								if(j != 0 && !strcmp(new[j - 1],"<<"))
									shel->exapnd_herdoc[herdoc_number] = 1;
								else
								{
									while (t[index])
									{
										new[j++] = ft_strtrim(t[index]," ");
										index++;
									}
								}
							}
							else if (tempo)
							{
								if(j != 0 && !strcmp(new[j - 1],"<<"))
								{
									shel->exapnd_herdoc[herdoc_number] = 1;
									herdoc_number++;
								}
								new[j++] = ft_strdup(tempo);
							}
							else
							{
								if(j != 0 && !strcmp(new[j - 1],"<<"))
								{
									shel->exapnd_herdoc[herdoc_number] = 1;
									herdoc_number++;
								}
								new[j++] = ft_calloc(1,1);
							}
						}
					}
				}
				else
				{
					tmp = substr(a, checkpoint, i - checkpoint);
					if (ft_strchr(tmp, '<') ||
						ft_strchr(tmp, '>'))
					{
						int	s = 0;
						while ((tmp[s] != '>' && tmp[s] != '<') && tmp[s])
							s++;
						if(s > 0)
							new[j++] = ft_strtrim(substr(tmp, 0, s)," ");
						if(tmp[s] == '>' )
						{
							if (tmp[s + 1] && tmp[s + 1] == '>')
							{
								new[j++] = ">>";
								s++;
							}
							else
								new[j++] = ">";
						}
						else if(tmp[s] == '<' )
						{
							if (tmp[s + 1] && tmp[s + 1] == '<')
							{
								new[j++] = "<<";
								s++;
							}
							else
								new[j++] = "<";
						}
						if(tmp[s + 1])
						{
							if(j != 0 && !strcmp(new[j - 1],"<<"))
							{
								shel->exapnd_herdoc[herdoc_number] = 1;
								herdoc_number++;
							}
							new[j++] = substr(tmp, s + 1, ft_strlen(tmp) - (s + 1));
						}
					}
					else
					{
						if(j != 0 && !strcmp(new[j - 1],"<<"))
						{
							shel->exapnd_herdoc[herdoc_number] = 1;
							herdoc_number++;
						}
						if (checkpoint != 0 && a[checkpoint - 1] != ' ')
							new[j - 1] = ft_strjoin(new[j - 1], substr(a, checkpoint, i - checkpoint));
						else
							new[j++] = ft_strtrim(substr(a, checkpoint, i - checkpoint)," ");
					}
				}
			}
		}
		checkpoint = i;
		if (a[i] == 39 || a[i] == 34)
		{
			if (a[i] == 39)
			{
				i++;
				while (a[i] != 39 && a[i])
					i++;
				if(j != 0 && new[j - 1] && !strcmp(new[j - 1],"<<"))
				{
					shel->exapnd_herdoc[herdoc_number] = 0;
					herdoc_number++;
				}
				if (checkpoint != 0 && a[checkpoint - 1] != ' ')
					new[j - 1] = ft_strjoin(new[j - 1], substr(a, checkpoint + 1, i - checkpoint - 1));
				else
				{
					if(ft_strlen(substr(a, checkpoint + 1, i - checkpoint - 1)) && j != 0 && theres_is_red(new[j-1]))
						new[j++] = substr(a, checkpoint + 1, i - checkpoint - 1);
					else if(!ft_strlen(substr(a, checkpoint + 1, i - checkpoint - 1)) && j != 0 && theres_is_red(new[j-1]))
						new[j++] = substr(a, checkpoint + 1, i - checkpoint - 1);
					else if(ft_strlen(substr(a, checkpoint + 1, i - checkpoint - 1)))
						new[j++] = substr(a, checkpoint + 1, i - checkpoint - 1);
					else if (j!= 0 && !theres_is_red(new[j-1]))
						new[j++] = NULL;
				}
			}
			else if (a[i] == 34)
			{
				i++;
				while (a[i] != 34 && a[i])
					i++;
				if(j != 0 && new[j - 1] && !strcmp(new[j - 1],"<<"))
				{
					shel->exapnd_herdoc[herdoc_number] = 5;
					herdoc_number++;
				}
				if (ft_strchr(substr(a, checkpoint, i - checkpoint), '$') && a[i - 1] != '$')
				{
					char *tet = check_expanding(shel, substr(a, checkpoint + 1, i - 1 - checkpoint));
					if (checkpoint != 0 && a[checkpoint - 1] != ' ')
					{
						if(tet)
							new[j - 1] = ft_strjoin(new[j - 1], tet);
					}
					else
					{
						if(tet)
							new[j++] = strdup(tet);
						else
							new[j++] = NULL;
					}
				}
				else
				{
					if (checkpoint != 0 && a[checkpoint - 1] != ' ')
						new[j - 1] = ft_strjoin(new[j - 1], substr(a, checkpoint + 1, i - checkpoint - 1));
					else
					{
						if(ft_strlen(substr(a, checkpoint + 1, i - checkpoint - 1)) && j != 0 && theres_is_red(new[j-1]))
							new[j++] = substr(a, checkpoint + 1, i - checkpoint - 1);
						else if(!ft_strlen(substr(a, checkpoint + 1, i - checkpoint - 1)) && j != 0 && theres_is_red(new[j-1]))
							new[j++] = substr(a, checkpoint + 1, i - checkpoint - 1);
						else if(ft_strlen(substr(a, checkpoint + 1, i - checkpoint - 1)))
							new[j++] = substr(a, checkpoint + 1, i - checkpoint - 1);
						else if (j!= 0 && !theres_is_red(new[j-1]))
						{
							new[j++] = NULL;
						}
					}
				// 	else
				// {
				// 	if (checkpoint != 0 && a[checkpoint - 1] != ' ')
				// 		new[j - 1] = ft_strjoin(new[j - 1], substr(a, checkpoint + 1, i - checkpoint - 1));
					// else
					// 	new[j++] = substr(a, checkpoint + 1, i - checkpoint - 1);
				// }
				}
			}
		}
		i++;
	}
	new[j] = NULL;
	return (new);
}
