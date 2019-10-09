/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_all.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 16:10:07 by glegendr          #+#    #+#             */
/*   Updated: 2019/10/09 21:19:32 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <ft_ssl.h>

char *const g_tab[NB_HASH] = {"md5", "sha256", "sha512", "sha384", "sha224",
	"base64", "base64url", "des", "des-ecb", "des-cbc", "des-pcbc"};
uint8_t *(*const g_hash_fct[NB_HASH])(t_hash *, bool) = {md5, sha256, sha512,
	sha384, sha224, base64, base64url, des_cbc, des_ecb, des_cbc, des_pcbc};
char const g_tab_flag[NB_FLAGS] = {'p', 'q', 'r', 's', 'd', 'e', 'i', 'o', 'k',
	'v', 'a', 'h'};
char *const g_tab_long_flag[NB_LONG_FLAGS] = {"password", "quiet", "reverse",
		"salt", "decript", "encrypt", "input", "output", "key", "initvector",
		"aliasbase", "help", "stdin", "string"};
void (*const g_flag_fct[NB_LONG_FLAGS])(t_hash *) = {p_fct, q_fct, r_fct, s_fct,
	d_fct, e_fct, i_fct, o_fct, k_fct, v_fct, a_fct, get_help, print_fct, string_fct};

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

void			(*get_long_flag_fct(char *flag))(t_hash *)
{
	int id;

	id = 0;
	while (id < NB_LONG_FLAGS)
	{
		if (!ft_strcmp(g_tab_long_flag[id], flag))
			break ;
		++id;
	}
	if (id == NB_LONG_FLAGS)
		print_usage(NULL);
	return (g_flag_fct[id]);
}

void			(*get_flag_fct(char flag))(t_hash *)
{
	int id;

	id = 0;
	while (id < NB_FLAGS)
	{
		if (g_tab_flag[id] == flag)
			break ;
		++id;
	}
	if (id == NB_FLAGS)
		print_usage(NULL);
	return (g_flag_fct[id]);
}

char			*get_all_hash(void)
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

char			*generic_usage(void)
{
	return ("Generics functions flags are:\n"
	" -p, --stdin          - echo STDIN to STDOUT and append the\n"
	"                        checksum to STDOUT\n"
	" -q, --quiet          - only print hash\n"
	" -r, --reverse        - reverse the output\n"
	" -s, --string         - the next argument is the string to hash\n"
	" -i, --input          - input file  -optional-\n"
	" -o, --output         - output file -default: stdout- -can be used once-\n"
	" -h, --help           - get help on this function\n\n"
	"./ft_ssl md5 [-pqrsioh]\n");
}

char			*des_usage(void)
{
	return ("Des flags are:\n"
	" -p, --password       - the next argument is your pasword\n"
	" -k, --key            - the next argument is your key in hexa\n"
	" -s, --salt           - the next argument is your salt in hexa\n"
	" -v, --initvector     - the next argument is your initialization\n"
	"                        vector -MANDATORY FOR DES, DES-CBC AND DES-PCBC-\n"
	" -a, --base64         - decode/encode the input/output in base64,\n"
	"                        depending on the encrypt mode\n"
	"     --string         - the next argument is the string to hash\n"
	" -d, --decrypt        - decode hash\n"
	" -e, --encrypt        - encode hash -optional-\n"
	" -i, --input          - input file  -optional-\n"
	" -o, --output         - output file -default: stdout- -can be used once-\n"
	" -h, --help           - get help on this function\n\n"
	"./ft_ssl des [-pksvadeioh]\n");
}

char			*base_usage()
{
	return ("base64 flags are:\n"
	" -p, --stdin          - echo STDIN to STDOUT and append the\n"
	"                        checksum to STDOUT\n"
	" -q, --quiet          - only print hash\n"
	" -r, --reverse        - reverse the output\n"
	" -s, --string         - the next argument is the string to hash\n"
	" -d, --decrypt        - decode hash\n"
	" -e, --encrypt        - encode hash -optional-\n"
	" -i, --input          - input file  -optional-\n"
	" -o, --output         - output file -default: stdout- -can be used once-\n"
	" -h, --help           - get help on this function\n\n"
	"./ft_ssl base64 [-pqrsiodeh]\n");
}


void			get_help(t_hash *tab)
{
	t_vec	vec;
	char	*s;

	vec = v_new(sizeof(char));
	if (tab->f == base64 || tab->f == base64url)
	{
		s = base_usage();
		v_append_raw(&vec, s, ft_strlen(s));
	}
	else if (tab->f == des_ecb || tab->f == des_cbc || tab->f == des_pcbc)
	{
		s = des_usage();
		v_append_raw(&vec, s, ft_strlen(s));
	}
	else
	{
		s = generic_usage();
		v_append_raw(&vec, s, ft_strlen(s));
	}
	write(2, (char *)v_raw(&vec), v_size(&vec));
	clean_des(tab, &vec);
	exit(1);
}
