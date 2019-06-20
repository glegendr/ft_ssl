/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64_url.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 15:48:29 by glegendr          #+#    #+#             */
/*   Updated: 2019/06/20 15:52:49 by glegendr         ###   ########.fr       */
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

void			encript_base(uint8_t *raw, char *base, int len, t_vec *print)
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

int				get_index(uint8_t *raw, char *base, uint8_t *index, int i)
{
	int y;

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
		return (1);
	return (0);
}

int				append_error(t_vec *print)
{
	v_reset(print);
	v_append_raw(print, "Invalid character in input stream.\n", 35);
	return (1);
}

void			base64url(t_hash *tab)
{
	char base[65];

	generate_base(base, true);
	tab->ops.name = "BASE64url";
	bases(tab, base, 0, 0);
}
