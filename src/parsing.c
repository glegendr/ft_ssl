/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 16:11:40 by glegendr          #+#    #+#             */
/*   Updated: 2019/06/25 13:35:07 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector.h>
#include <libft.h>
#include <fcntl.h>
#include "ft_ssl.h"

int				add_flag(char flag, t_hash *tab)
{
	if (flag == 'p')
		tab->arg |= P_FLAG;
	else if (flag == 'q')
		tab->arg |= Q_FLAG;
	else if (flag == 'r')
		tab->arg |= R_FLAG;
	else if (flag == 's')
		tab->arg |= S_FLAG;
	else if (flag == 'd')
		tab->arg |= D_FLAG;
	else if (flag == 'e')
		tab->arg |= E_FLAG;
	else if (flag == 'i')
		tab->arg |= I_FLAG;
	else if (flag == 'o')
	{
		if (tab->ops.fd != 1)
			print_usage(NULL);
		tab->arg |= O_FLAG;
	}
	else
		return (1);
	return (0);
}

static int		match_flag(char *flag, t_hash *tab)
{
	int i;

	if (!flag || !flag[1])
		return (1);
	else if (flag[0] != '-')
	{
		read_file(tab, open_file(flag, 0, 0), false);
		into_vec(&tab->folder, flag);
		return (0);
	}
	i = 1;
	while (flag[i])
	{
		if (add_flag(flag[i++], tab))
			return (1);
	}
	return (0);
}

static void		s_flag(t_hash *tab, char *argv)
{
	into_vec(&tab->folder, argv);
	into_vec(&tab->str, argv);
	tab->arg ^= S_FLAG;
}

void			argument_flags(t_hash *tab, char **argv, int argc, int *i)
{
	char *str;

	*i += 1;
	str = argv[*i];
	if (*i >= argc)
		print_usage(NULL);
	else if (tab->arg & S_FLAG)
		s_flag(tab, str);
	else if (tab->arg & O_FLAG)
		o_flag(tab, str);
	else if (tab->arg & P_FLAG)
		tab->ops.pwd = str;
	else if (tab->arg & I_FLAG)
	{
		match_flag(str, tab);
		tab->arg ^= I_FLAG;
	}
}

void			parse_argv(int argc, char *argv[])
{
	t_hash	tab;
	int		i;

	if (argc < 2)
		print_usage(NULL);
	tab.f = get_hash_fct(argv[1]);
	tab.arg = 0;
	tab.folder = v_new(sizeof(t_vec));
	tab.str = v_new(sizeof(t_vec));
	tab.ops.fd = 1;
	tab.ops.pwd = NULL;
	i = 2;
	while (i < argc)
	{
		if (match_flag(argv[i], &tab))
			print_usage(NULL);
		if ((tab.arg & S_FLAG) || (tab.arg & O_FLAG) || (tab.arg & I_FLAG)
				|| ((tab.arg & P_FLAG) && tab.f == des_ecb))
			argument_flags(&tab, argv, argc, &i);
		++i;
	}
	if (!v_size(&tab.folder) || (tab.arg & P_FLAG))
	{
		read_file(&tab, 0, (tab.arg & P_FLAG) ? true : false);
		into_vec(&tab.folder, NULL);
	}
	tab.f(&tab);
}
