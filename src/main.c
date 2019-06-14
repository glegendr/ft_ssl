/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 14:48:56 by glegendr          #+#    #+#             */
/*   Updated: 2019/06/14 15:45:40 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector.h>
#include <libft.h>
#include <fcntl.h>
#include "ft_ssl.h"

char *const g_tab[NB_HASH] = {"md5", "sha256", "sha512", "sha384", "sha224", "base64", "base64url"};
void (*const g_hash_fct[NB_HASH])(t_hash *) = {md5, sha256, sha512, sha384, sha224, base64, base64url};

static void		(* get_hash_fct(char *name))(t_hash *)
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

static char		*get_all_hash(void)
{
	t_vec vec;
	char *ret;
	int i;

	i = 0;
	vec = v_new(sizeof(char));
	while (i < NB_HASH)
	{
		v_append_raw(&vec, "   ", 3);
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
		"usage: ft_ssl command [command opts] [command args]\nft_ssl -h for more help\n", 76);
	else
	{
		vec = v_new(sizeof(char));
		s = "ft_ssl: Error: '";
		v_append_raw(&vec, s, ft_strlen(s));
		v_append_raw(&vec, name, ft_strlen(name));
		s = "' is an invalid command\nUsage: ft_ssl hash_fct [-pqrde] [-s string] [-i in_file] [-o out_file]\nAllowed hash_fct are:\n";
		v_append_raw(&vec, s, ft_strlen(s));
		s = get_all_hash();
		v_append_raw(&vec, s, ft_strlen(s));
		s = "Allowed flags are:\n   -p print string before hash\n   -q only print hash\n   -r reverse the output\n   -d decode hash -base64 only-\n   -e encode hash -base64 only-\n   -s hash the string\n   -i input file  -optional-\n   -o output file -default: stdout- -can only be used once-\n";
		v_append_raw(&vec, s, ft_strlen(s));
		write(2, (char *)v_raw(&vec), v_size(&vec));
		v_del(&vec);
	}
	exit(1);
}

static void	read_file(t_hash *tab, int fd, bool print)
{
	t_vec	vec;
	char	ret[BUFF_SIZE];
	int		val;

	if (fd == -1)
		print_usage(NULL);
	vec = v_new(sizeof(char));
	while ((val = read(fd, ret, BUFF_SIZE)) > 0)
	{
		v_append_raw(&vec, ret, val);
		if (print == true)
			write(1, ret, BUFF_SIZE);
	}
	v_push(&tab->str, &vec);
}

static int		open_file(char *argv, int flag, int perm)
{
	int		fd;
	int		o_flag;
	int		s_perm;

	o_flag = O_RDONLY | flag;
	s_perm = 0 | perm;
	if ((fd = open(argv, o_flag, s_perm)) == -1)
	{
		printf("ft_ssl: %s: No such file or directory\n", argv);
		return (fd);
	}
	return (fd);
}

static void		into_vec(t_vec *to_push, char *str)
{
	t_vec vec;

	vec = v_new(sizeof(char));
	v_append_raw(&vec, str, ft_strlen(str));
	v_push(to_push, &vec);
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
			{
				if (tab->ops.fd != 1)
					print_usage(NULL);
				tab->arg |= O_FLAG;
			}
			else
				return (1);
			++i;
		}
		return (0);
}

static void		o_flag(t_hash *tab, char *argv)
{
	int fd;

	fd = open_file(argv, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IWGRP |  S_IROTH);
	if (fd == -1)
		print_usage(NULL);
	tab->ops.fd = fd;
	tab->arg ^= O_FLAG;
}

static void		s_flag(t_hash *tab, char *argv)
{
	t_vec tmp;

	into_vec(&tab->folder, argv);
	tmp = v_new(sizeof(uint8_t));
	v_append_raw(&tmp, argv, ft_strlen(argv));
	v_push(&tab->str, &tmp);
	tab->arg ^= S_FLAG;
}

static void		parse_argv(int argc, char *argv[])
{
	t_hash tab;
	int i;

	if (argc < 2)
		print_usage(NULL);
	tab.f = get_hash_fct(argv[1]);
	tab.arg = 0;
	tab.folder = v_new(sizeof(t_vec));
	tab.str = v_new(sizeof(t_vec));
	tab.ops.fd = 1;
	i = 2;
	while (i < argc)
	{
		if (match_flag(argv[i], &tab))
			print_usage(NULL);
		if ((tab.arg & S_FLAG) || (tab.arg & O_FLAG) || (tab.arg & I_FLAG))
		{
			if (++i >= argc)
				print_usage(NULL);
			else if (tab.arg & S_FLAG)
				s_flag(&tab, argv[i]);
			else if (tab.arg & O_FLAG)
				o_flag(&tab, argv[i]);
			else if (tab.arg & I_FLAG)
			{
				match_flag(argv[i], &tab);
				tab.arg ^= I_FLAG;
			}
		}
		++i;
	}
	if (!v_size(&tab.folder) || (tab.arg & P_FLAG))
	{
		read_file(&tab, 0, true);
		v_push(&tab.folder, NULL);
	}
	tab.f(&tab);
}

int		main(int argc, char *argv[])
{
	parse_argv(argc, argv);
	return (0);
}
