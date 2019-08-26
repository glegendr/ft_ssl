/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 16:11:40 by glegendr          #+#    #+#             */
/*   Updated: 2019/08/26 14:11:18 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector.h>
#include <libft.h>
#include <fcntl.h>
#include "ft_ssl.h"

int				add_flag(char flag, t_hash *tab)
{
	if (flag == 'p')
	{
		if (tab->ops.pwd)
			print_usage(NULL);
		tab->arg |= P_FLAG;
	}
	else if (flag == 'q')
		tab->arg |= Q_FLAG;
	else if (flag == 'r')
		tab->arg |= R_FLAG;
	else if (flag == 's')
		tab->arg |= S_FLAG;
	else if (flag == 'd')
	{
		if (tab->arg & E_FLAG)
			print_usage(NULL);
		tab->arg |= D_FLAG;
	}
	else if (flag == 'e')
	{
		if (tab->arg & D_FLAG)
			print_usage(NULL);
		tab->arg |= E_FLAG;
	}
	else if (flag == 'i')
		tab->arg |= I_FLAG;
	else if (flag == 'o')
	{
		if (tab->ops.fd != 1)
			print_usage(NULL);
		tab->arg |= O_FLAG;
	}
	else if (flag == 'k')
	{
		if (tab->ops.key)
			print_usage(NULL);
		tab->arg |= K_FLAG;
	}
	else if (flag == 'v')
	{
		if (tab->ops.init_vec)
			print_usage(NULL);
		tab->arg |= V_FLAG;
	}
	else if (flag == 'a')
		tab->arg |= A_FLAG;
	else if (flag == 'h')
		get_help(tab);
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

static void		hex_flag(t_hash *hash, char *str, int flag)
{
	uint8_t	*key;
	int		len;
	int		i;
	char	in;

	if (!(key = malloc(sizeof(uint8_t) * 8)))
		return ;
	i = 0;
	len = ft_strlen(str);
	while (i < 16)
	{
		if (i < len)
		{
			in = str[i];
			if (in >= 'a' && in <= 'f')
				in -= 97 - 10;
			else if (in >= 'A' && in <= 'F')
				in -= 65 - 10;
			else if (in >= '0' && in <= '9')
				in -= 48;
			else
			{
				write(2, "non-hex digit\n", 14);
				exit(1);
			}
		}
		else
			in = 0;
		key[i / 2] = key[i / 2] << 4;
		key[i / 2] = key[i / 2] | in;
		++i;
	}
	if (flag == K_FLAG)
		hash->ops.key = key;
	else if (flag == V_FLAG)
		hash->ops.init_vec = key;
	else
		hash->ops.salt = key;
	hash->arg ^= flag;
}

static void		s_flag(t_hash *tab, char *str)
{
	if (tab->f != des_ecb)
	{
		into_vec(&tab->folder, str);
		into_vec(&tab->str, str);
		tab->arg ^= S_FLAG;
		return ;
	}
	hex_flag(tab, str, S_FLAG);
}

void			argument_flags(t_hash *tab, char **argv, int argc, int *i)
{
	char *str;

	*i += 1;
	if (*i >= argc)
		print_usage(NULL);
	str = argv[*i];
	if (tab->arg & S_FLAG)
		s_flag(tab, str);
	if (tab->arg & O_FLAG)
		o_flag(tab, str);
	if (tab->arg & P_FLAG)
	{
		tab->ops.pwd = (uint8_t *)str;
		tab->arg ^= P_FLAG;
	}
	if (tab->arg & I_FLAG)
	{
		match_flag(str, tab);
		tab->arg ^= I_FLAG;
	}
	if (tab->arg & K_FLAG)
		hex_flag(tab, str, K_FLAG);
	if (tab->arg & V_FLAG)
		hex_flag(tab, str, V_FLAG);
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
	tab.ops.salt = NULL;
	tab.ops.init_vec = NULL;
	tab.ops.key = NULL;
	i = 2;
	while (i < argc)
	{
		if (match_flag(argv[i], &tab))
			print_usage(NULL);
		if ((tab.arg & S_FLAG) || (tab.arg & O_FLAG) || (tab.arg & I_FLAG)
				|| ((tab.arg & P_FLAG) && (tab.f == des_ecb || tab.f == des_pcbc || tab.f == des_cbc))
				|| (tab.arg & K_FLAG) || (tab.arg & V_FLAG))
			argument_flags(&tab, argv, argc, &i);
		++i;
	}
	if (!v_size(&tab.folder) || ((tab.arg & P_FLAG) && (tab.f != des_ecb && tab.f != des_cbc && tab.f != des_pcbc)))
	{
		read_file(&tab, 0, (tab.arg & P_FLAG) ? true : false);
		into_vec(&tab.folder, NULL);
	}
	if ((tab.f == des_cbc || tab.f == des_pcbc) && !tab.ops.init_vec)
		print_usage(NULL);
	tab.f(&tab, true);
}
