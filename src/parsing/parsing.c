/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 16:11:40 by glegendr          #+#    #+#             */
/*   Updated: 2019/08/28 08:07:13 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector.h>
#include <libft.h>
#include <fcntl.h>
#include "ft_ssl.h"
#define IS_DES(f) (f == des_ecb || f == des_cbc || f == des_pcbc)

static int		match_flag(char *flag, t_hash *tab)
{
	int		i;
	void	(*f)(t_hash *);

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
		f = get_flag_fct(flag[i]);
		f(tab);
		++i;
	}
	return (0);
}

void			hex_to_u8(char *str, int len, uint8_t *key, int i)
{
	int in;

	while (i < 16)
	{
		if (i < len)
		{
			in = str[i];
			if (in >= 'a' && in <= 'f')
				in -= 'a' - 10;
			else if (in >= 'A' && in <= 'F')
				in -= 'A' - 10;
			else if (in >= '0' && in <= '9')
				in -= '0';
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
}

static void		hex_flag(t_hash *hash, char *str, int flag)
{
	uint8_t	*key;
	int		len;
	int		i;

	if (!(key = malloc(sizeof(uint8_t) * 8)))
		return ;
	i = 0;
	len = ft_strlen(str);
	hex_to_u8(str, len, key, 0);
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

void			init_hash(t_hash *tab, char *fct)
{
	tab->f = get_hash_fct(fct);
	tab->arg = 0;
	tab->folder = v_new(sizeof(t_vec));
	tab->str = v_new(sizeof(t_vec));
	tab->ops.fd = 1;
	tab->ops.pwd = NULL;
	tab->ops.salt = NULL;
	tab->ops.init_vec = NULL;
	tab->ops.key = NULL;
}

void			parse_argv(int argc, char *argv[])
{
	t_hash	tab;
	int		i;

	if (argc < 2)
		print_usage(NULL);
	init_hash(&tab, argv[1]);
	i = 2;
	while (i < argc)
	{
		if (match_flag(argv[i], &tab))
			print_usage(NULL);
		if ((tab.arg & S_FLAG) || (tab.arg & O_FLAG) || (tab.arg & I_FLAG)
				|| ((tab.arg & P_FLAG) && IS_DES(tab.f))
				|| (tab.arg & K_FLAG) || (tab.arg & V_FLAG))
			argument_flags(&tab, argv, argc, &i);
		++i;
	}
	if (!v_size(&tab.folder) || ((tab.arg & P_FLAG) && !IS_DES(tab.f)))
	{
		read_file(&tab, 0, (tab.arg & P_FLAG) ? true : false);
		into_vec(&tab.folder, NULL);
	}
	if ((tab.f == des_cbc || tab.f == des_pcbc) && !tab.ops.init_vec)
		print_usage(NULL);
	tab.f(&tab, true);
}
