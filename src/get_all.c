/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_all.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 16:10:07 by glegendr          #+#    #+#             */
/*   Updated: 2019/07/11 16:58:51 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector.h>
#include <libft.h>
#include <fcntl.h>
#include "ft_ssl.h"

char *const g_tab[NB_HASH] = {"md5", "sha256", "sha512", "sha384", "sha224",
	"base64", "base64url", "des", "des-ecb", "des-cbc"};
uint8_t *(*const g_hash_fct[NB_HASH])(t_hash *, bool) = {md5, sha256, sha512, sha384, sha224, base64, base64url, des_cbc, des_ecb, des_cbc};

uint8_t			*(*get_hash_fct(char *name))(t_hash *, bool)
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
	t_vec	vec;
	char	*ret;
	int		i;

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

char			*get_usage(void)
{
	return ("Generics flags are:\n"
	" -p echo STDIN to STDOUT and append the checksum to STDOUT -don't work with des, des-ecb-\n"
	" -q only print hash\n"
	" -r reverse the output\n"
	" -s hash the string -don't work with des, des-ecb-\n"
	" -i input file  -optional-\n"
	" -o output file -default: stdout- -can only be used once-\n"
	" Try hash_fct -h to get help for the function\n");
}

void			print_usage(char *name)
{
	t_vec	vec;
	char	*s;

	if (!name)
		write(2,
		"usage: ft_ssl command [command opts] [command args]\n"
		"ft_ssl -h for more help\n", 76);
	else
	{
		vec = v_new(sizeof(char));
		s = "ft_ssl: Error: '";
		v_append_raw(&vec, s, ft_strlen(s));
		v_append_raw(&vec, name, ft_strlen(name));
		s = "' is an invalid command\n"
			"Usage: ft_ssl hash_fct [-pqrde] [-s string] [-i in_file]"
			" [-o out_file]\nAllowed hash_fct are:\n";
		v_append_raw(&vec, s, ft_strlen(s));
		s = get_all_hash();
		v_append_raw(&vec, s, ft_strlen(s));
		s = get_usage();
		v_append_raw(&vec, s, ft_strlen(s));
		write(2, (char *)v_raw(&vec), v_size(&vec));
		v_del(&vec);
	}
	exit(1);
}

void			get_help(t_hash *tab)
{
	t_vec	vec;
	char	*s;

	vec = v_new(sizeof(char));
	s = get_usage();
	v_append_raw(&vec, s, ft_strlen(s));
	s = "Specific flags for your function are:\n";
	v_append_raw(&vec, s, ft_strlen(s));
	if (tab->f == base64 || tab->f == base64url || tab->f == des_ecb)
	{
		s = " -d decode hash\n -e encode hash -optional-\n";
		v_append_raw(&vec, s, ft_strlen(s));
	}
	if (tab->f == des_ecb)
	{
		s = " -k key in hex\n -s salt in hex\n -v initialization vector in hex\n -a decode/encode the input/output in base64, depending on the encrypt mode\n -p next arg is your password\n";
		v_append_raw(&vec, s, ft_strlen(s));
	}
	write(2, (char *)v_raw(&vec), v_size(&vec));
	v_del(&vec);
	exit(1);
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
