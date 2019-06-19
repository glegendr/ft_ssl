/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 16:23:20 by glegendr          #+#    #+#             */
/*   Updated: 2019/06/19 17:36:54 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <libft.h>
#include "ft_ssl.h"

void			generate_base(char *base, bool url)
{
	int y;
	int i;

	y = 0;
	i = 0;
	while ('A' + i <= 'Z')
		base[y++] = 'A' + i++;
	i = 0;
	while ('a' + i <= 'z')
		base[y++] = 'a' + i++;
	i = 0;
	while ('0' + i <= '9')
		base[y++] = '0' + i++;
	if (url)
	{
		base[y] = '-';
		base[y + 1] = '_';
	}
	else
	{
		base[y] = '+';
		base[y + 1] = '/';
	}
	base[y + 2] = '=';
}

static void		encript_base(uint8_t *raw, char *base, int len, t_vec *print)
{
	uint8_t	ret[4];
	uint8_t x;

	x = len == 1 ? 0 : ((raw[1] & 0xf0) >> 2);
	ret[0] = (raw[0] & 0xfc) >> 2;
	ret[1] = (((raw[0] & 0x3) << 6) | x) >> 2;
	if (len > 2)
	{
		ret[2] = (((raw[1] & 0xf) << 4) | ((raw[2] & 0xc0) >> 4)) >> 2;
		ret[3] = (raw[2] & 0x3f);
	}
	else
	{
		ret[2] = len == 2 ? (((raw[1] & 0xf) << 4) | 0) >> 2 : 64;
		ret[3] = 64;
	}
	v_push_int(print, base[ret[0]]);
	v_push_int(print, base[ret[1]]);
	v_push_int(print, base[ret[2]]);
	v_push_int(print, base[ret[3]]);
}

int				decript_base(uint8_t *raw, char *base, int len,
								int *z, t_vec *print)
{
	uint8_t	ret[3] = {0, 0, 0};
	uint8_t	index[4] = {255, 255, 255, 255};
	size_t	y;
	int		i;

	i = 0;
	while (i < 4 && *z + i < len)
	{
		while (ft_isspace(raw[i]) && *z < len)
		{
			++raw;
			++(*z);
		}
		if (*z >= len)
			break ;
		y = 0;
		while (y < 65)
		{
			if (raw[i] == base[y])
			{
				index[i] = y;
				break ;
			}
			++y;
		}
		if (index[i] == 255 && raw[i] != '\n' && raw[i] != '\0')
		{
			v_reset(print);
			v_append_raw(print, "Invalid character in input stream.\n", 35);
			return (1);
		}
		++i;
	}
	if (i > 0)
		ret[0] = (index[0] << 2) | ((index[1] & 0x30) >> 4);
	if (i > 1)
		ret[1] = ((index[1] & 0xf) << 4) | ((index[2] & 0x3c) >> 2);
	if (i > 2)
		ret[2] = ((index[2] & 0x3) << 6) | index[3];
	if (ret[2] == '@')
		ret[2] = '\0';
	v_append_raw(print, ret, 3);
	return (0);
}

void			bases(t_hash *tab, char *base)
{
	int		i;
	t_vec	print;
	t_vec	*vec;
	uint8_t	*tmp;
	int		z;

	z = 0;
	i = 0;
	print = v_new(sizeof(char));
	while (i < v_size(&tab->str))
	{
		vec = v_get(&tab->str, i);
		tmp = (uint8_t *)v_raw(vec);
		while (z < v_size(vec))
		{
			if (tab->arg & D_FLAG)
			{
				if (decript_base(tmp + z, base, v_size(vec), &z, &print))
					break ;
				++z;
			}
			else
				encript_base(tmp + z, base, v_size(vec) - z, &print);
			z += 3;
		}
		tab->ops.message_len = v_size(&print);
		print_hash(&print, tab, i, tab->ops);
		v_reset(&print);
		++i;
	}
	v_del(&print);
}

void			base64(t_hash *tab)
{
	char base[65];

	generate_base(base, false);
	tab->ops.name = "BASE64";
	bases(tab, base);
}

void			base64url(t_hash *tab)
{
	char base[65];

	generate_base(base, true);
	tab->ops.name = "BASE64url";
	bases(tab, base);
}
