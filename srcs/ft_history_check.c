/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_history_check.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhong <fhong@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/26 16:21:52 by fhong             #+#    #+#             */
/*   Updated: 2018/11/07 18:34:03 by mcarney          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static char		*get_history(char *cmd)
{
	int			i;
	int			index;
	t_history	*tmp;

	i = -1;
	if (cmd[0] == '!' && cmd[1] && cmd[1] != '\'' && cmd[1] != '\"' &&
		!ISSPACE(cmd[1]) && !ISSPACE2(cmd[1]))
	{
		index = (cmd[1] == '!') ? -1 : ft_atoi(&cmd[1]);
		(g_history && index < 0) && (index = g_history->index + index + 1);
		if (!g_history || index <= 0 || index > g_history->index)
		{
			if (index == 0)
				ft_printf("42sh: event not found: %s\n", &cmd[1]);
			else
				ft_printf("42sh: event not found: %d\n", ft_atoi(&cmd[1]));
			return (NULL);
		}
		tmp = g_history;
		while (tmp->index != index)
			tmp = tmp->next;
		return (tmp->command);
	}
	return ("!");
}

static int		ft_check_history_helper(char *str, int mark)
{
	if (!ft_strncmp(&str[mark], "!!", 2))
		mark += 2;
	else if (ft_numlen(&str[mark + 1]) != 0)
		mark = mark + ft_numlen(&str[mark + 1]) + 1;
	else
		mark += 1;
	return (mark);
}

static char		*get_history_command(char *str, char *cmd, int mark, int tmp_mark)
{
	char	*tmp;

	tmp = ft_strsub(str, tmp_mark, mark - tmp_mark);
	cmd = (cmd) ? free_join(cmd, tmp) : ft_strdup(tmp);
	(tmp) ? ft_strdel(&tmp) : 0;
	tmp = (str[mark] == '!') ? get_history((str + mark)) : ft_strdup("");
	cmd = (cmd) ? free_join(cmd, tmp) : ft_strdup(tmp);
	return (cmd);
}

static int		move_mark_in_siglequote(char *str, int mark)
{
	while (str[++mark] && (str[mark] != '\'' ||
				(str[mark] == '\'' && count_backslashes(mark, str))))
		;
	mark++;
	return (mark);
}

char	*ft_check_history(char *str)
{
	int		mark;
	int		tmp_mark;
	char	*cmd;

	if (!ft_strchr(str, '!'))
		return (str);
	cmd = NULL;
	mark = 0;
	tmp_mark = mark;
	while (str && str[mark])
	{
		if (str[mark] == '\'' && !(count_backslashes(mark, str)))
			mark = move_mark_in_siglequote(str, mark);
		else if (str[mark] == '!' && !(count_backslashes(mark, str)))
		{
			cmd = get_history_command(str, cmd, mark, tmp_mark);
			mark = ft_check_history_helper(str, mark);
			tmp_mark = mark;
		}
		else
			mark++;
	}
	cmd = (cmd) ? free_join(cmd, &str[tmp_mark]) : ft_strdup(str);
	(ft_strcmp(cmd, str) != 0) ? (ft_putendl(cmd)) : 0;
	return (cmd);
}
