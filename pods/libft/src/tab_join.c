/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tab_join.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/03 18:50:47 by glegendr          #+#    #+#             */
/*   Updated: 2019/05/15 19:35:27 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		del_tab(char **tab)
{
	int i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		++i;
	}
	free(tab);
}

int		tab_len(char **tab)
{
	int i;

	i = 0;
	if (!tab)
		return (0);
	while (tab[i])
		++i;
	return (i);
}

char		**tab_join(char **tab, char *s, int size)
{
	int		i;
	char	**tmp;

	i = 0;
	if (size == 0)
	{
		tab = (char **)malloc(sizeof(char *) * 2);
		tab[0] = ft_strdup(s);
		tab[1] = NULL;
		return (tab);
	}
	tmp = (char **)malloc(sizeof(char *) * (size + 2));
	while (i < size)
	{
		tmp[i] = ft_strdup(tab[i]);
		++i;
	}
	del_tab(tab);
	tmp[i] = ft_strdup(s);
	tmp[i + 1] = NULL;
	return (tmp);
}
