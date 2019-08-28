/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_key.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/14 07:17:26 by glegendr          #+#    #+#             */
/*   Updated: 2019/08/28 09:45:41 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include <libft.h>
#include <des.h>

void		bit(uint8_t *ret, int i, uint8_t *str, int pos)
{
	uint8_t	x;
	int		posit;
	int		bit;
	int		in_ret;

	posit = pos - pos / 8 * 8;
	in_ret = ((posit == 0 ? pos - 1 : pos) / 8);
	if (posit == 0)
		posit = 8;
	x = 1 << (8 - posit);
	bit = (str[in_ret] & x) >> (8 - posit);
	if ((i % 8) == 0 && i != 0)
		ret[(i - 1) / 8] |= bit;
	else
		ret[i / 8] |= bit << (8 - (i % 8));
}

void		pc1(uint8_t *str)
{
	uint8_t	ret[7];
	int		i;

	i = 0;
	ft_bzero(ret, 7 * sizeof(uint8_t));
	while (i < 56)
	{
		bit(ret, i + 1, str, g_pc1[i]);
		++i;
	}
	in_u8(ret, str);
}

void		pc2(uint32_t *str, uint8_t ret[16][6])
{
	int		i;
	int		y;
	uint8_t	in[7];

	y = 0;
	while (y < 16)
	{
		in[0] = (str[y] & 0xff00000) >> 20;
		in[1] = (str[y] & 0xff000) >> 12;
		in[2] = (str[y] & 0xff0) >> 4;
		in[3] = ((str[y] & 0xf) << 4) | ((str[y + 16] & 0xf000000) >> 24);
		in[4] = (str[y + 16] & 0xff0000) >> 16;
		in[5] = (str[y + 16] & 0xff00) >> 8;
		in[6] = str[y + 16] & 0xff;
		i = 0;
		while (i < 48)
		{
			bit(ret[y], i + 1, in, g_pc2[i]);
			++i;
		}
		++y;
	}
}

uint32_t	u8_to_b28(uint8_t *val, bool first)
{
	int			i;
	uint32_t	bytes;
	uint8_t		*str;

	i = 0;
	str = val;
	bytes = 0;
	if (!first)
	{
		bytes = bytes | (str[0] & 0xf);
		bytes = bytes << 8;
		str += 1;
	}
	while (i < 3)
	{
		if (!(!first && i == 0))
			bytes = bytes << 8;
		bytes = bytes | str[i++];
	}
	if (first)
	{
		bytes = bytes << 4;
		bytes = bytes | ((str[i] & 0xf0) >> 4);
	}
	return (bytes);
}

void		rotate_key(uint8_t *pwd, uint32_t *div_key)
{
	uint32_t	c0;
	uint32_t	d0;
	int			x;
	int			i;

	c0 = u8_to_b28(pwd, true);
	d0 = u8_to_b28(pwd + 3, false);
	i = 0;
	x = 1;
	while (i < 32)
	{
		if (i == 2 || i == 9 || i == 16 + 2 || i == 16 + 9)
			x = 2;
		else if (i == 8 || i == 15 || i == 16 + 8 || i == 16 + 15)
			x = 1;
		div_key[i] = DEFKEY(i < 16 ? c0 : d0);
		++i;
	}
}
