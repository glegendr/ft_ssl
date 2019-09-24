/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 16:11:40 by glegendr          #+#    #+#             */
/*   Updated: 2019/09/24 18:19:55 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>

static int		match_flag(char *flag, t_hash *tab)
{
	int		i;
	void	(*f)(t_hash *);

	if (!flag)
		return (1);
	else if (flag[0] != '-' || !flag[1] || tab->arg & I_FLAG)
	{
		if (read_file(tab, open_file(flag, 0, 0), false) == 0)
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

void			argument_flags(t_hash *tab, char **argv, int argc, int *i)
{
	char *str;

	*i += 1;
	if (*i >= argc)
		CLEAN_AND_PRINT(tab, NULL);
	str = argv[*i];
	if (tab->arg & S_FLAG)
		s_flag(tab, str);
	if (tab->arg & O_FLAG)
		o_flag(tab, str);
	if ((tab->arg & P_FLAG) && IS_DES(tab->f))
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

#define IS_FLG(b) (tab.arg & b)

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
			CLEAN_AND_PRINT(&tab, NULL);
		if (IS_FLG(S_FLAG) || IS_FLG(O_FLAG) || IS_FLG(I_FLAG)
				|| (IS_FLG(P_FLAG) && IS_DES(tab.f))
				|| IS_FLG(K_FLAG) || IS_FLG(V_FLAG))
			argument_flags(&tab, argv, argc, &i);
		++i;
	}
	if (!v_size(&tab.folder) || (IS_FLG(P_FLAG) && !IS_DES(tab.f)))
	{
		read_file(&tab, 0, (IS_FLG(P_FLAG) && !IS_DES(tab.f)) ? true : false);
		into_vec(&tab.folder, NULL);
	}
	if ((tab.f == des_cbc || tab.f == des_pcbc) && !tab.ops.init_vec)
		CLEAN_AND_PRINT(&tab, NULL);
	tab.f(&tab, true);
}

uint64_t		init_s0(uint8_t *ck, int i)
{
	uint64_t	s0;
	int			y;

	y = 0;
	s0 = 0;
	while (y < 8)
	{
		s0 = s0 << 8;
		s0 = s0 + ck[i * 8 + y];
		++y;
	}
	return (s0);
}
