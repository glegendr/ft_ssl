/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 14:48:56 by glegendr          #+#    #+#             */
/*   Updated: 2019/06/07 15:22:05 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector.h>
#include <libft.h>
#include <fcntl.h>
#include "ft_ssl.h"

char *const g_tab[NB_HASH] = {"md5", "sha256", "sha512", "sha384", "sha224", "base64", "base64url"};
void (*const g_hash_fct[NB_HASH])(t_hash *) = {md5, sha256, sha512, sha384, sha224, base64, base64url};

void		(* get_hash_fct(char *name))(t_hash *)
{
	int id;

	id = 0;
	while (id < NB_HASH)
	{
		if (!ft_strcmp(g_tab[id], name))
			break ;
		++id;
	}
	if (id == NB_HASH)
		print_usage(name);
	return (g_hash_fct[id]);
}

char		*get_all_hash(void)
{
	t_vec vec;
	char *ret;
	int i;

	i = 0;
	vec = v_new(sizeof(char));
	while (i < NB_HASH)
	{
		v_append_raw(&vec, g_tab[i], ft_strlen(g_tab[i]));
		v_push_int(&vec, '\n');
		++i;
	}
	v_push_int(&vec, '\0');
	ret = ft_strdup(v_raw(&vec));
	v_del(&vec);
	return (ret);
}

void		print_usage(char *name)
{
	t_vec vec;
	char *s;

	if (!name)
		write(2,
		"usage: ft_ssl command [command opts] [command args]\n", 51);
	else
	{
		vec = v_new(sizeof(char));
		s = "ft_ssl: Error: '";
		v_append_raw(&vec, s, ft_strlen(s));
		v_append_raw(&vec, name, ft_strlen(name));
		s = "' is an invalid command\nallowed commands are:\n";
		v_append_raw(&vec, s, ft_strlen(s));
		s = get_all_hash();
		v_append_raw(&vec, s, ft_strlen(s));
		write(2, (char *)v_raw(&vec), v_size(&vec));
		v_del(&vec);
	}
	exit(1);
}

int		open_folder(char *flag, t_hash *tab)
{
	int		fd;
	t_vec	vec;
	char	ret[BUFF_SIZE];
	int		val;

	if ((fd = open(flag, O_RDONLY)) == -1)
	{
		printf("ft_ssl: %s: No such file or directory\n", flag);
		return (0);
	}
	tab->folder = tab_join(tab->folder, flag, tab_len(tab->folder));
	vec = v_new(sizeof(char));
	while ((val = read(fd, ret, BUFF_SIZE)) > 0)
		v_append_raw(&vec, ret, val);
	v_push(&tab->str, &vec);
	//tab->str = (uint8_t **)tab_join((char **)tab->str, (char *)v_raw(&vec), tab_len(tab->folder) - 1);
//	v_del(&vec);
	return (0);
}

int		match_flag(char *flag, t_hash *tab)
{
	int i;

	if (!flag || !flag[1])
		return (1);
	else if (flag[0] != '-')
		return (open_folder(flag, tab));
	i = 1;
	while (flag[i])
	{
		if (flag[i] == 'p')
				tab->arg |= P_FLAG;
			else if (flag[i] == 'q')
				tab->arg |= Q_FLAG;
			else if (flag[i] == 'r')
				tab->arg |= R_FLAG;
			else if (flag[i] == 's')
				tab->arg |= S_FLAG;
			else if (flag[i] == 'd')
				tab->arg |= D_FLAG;
			else if (flag[i] == 'e')
				tab->arg |= E_FLAG;
			else if (flag[i] == 'i')
				tab->arg |= I_FLAG;
			else if (flag[i] == 'o')
				tab->arg |= O_FLAG;
			else
				return (1);
			++i;
		}
		return (0);
}

void		parse_argv(int argc, char *argv[])
{
	t_hash tab;
	int i;

	if (argc < 2)
		print_usage(NULL);
	tab.f = get_hash_fct(argv[1]);
	tab.arg = 0;
	tab.folder = NULL;
	tab.str = v_new(sizeof(t_vec));
	i = 2;
	while (i < argc)
	{
		if (match_flag(argv[i], &tab))
			print_usage(NULL);
		if ((tab.arg & S_FLAG) != 0)
		{
			if (++i >= argc)
				print_usage(NULL);
			tab.folder = tab_join(tab.folder, argv[i], tab_len(tab.folder));
			t_vec tmp;
			tmp = v_new(sizeof(uint8_t));
			v_append_raw(&tmp, argv[i], ft_strlen(argv[i]));
			v_push(&tab.str, &tmp);
		//	tab.str = (uint8_t **)tab_join((char **)tab.str, argv[i], tab_len(tab.folder) - 1);
			tab.arg ^= S_FLAG;
		}
		++i;
	}
	tab.f(&tab);
}

#include <fcntl.h>

int		main(int argc, char *argv[])
{

	parse_argv(argc, argv);
	return (0);
}
