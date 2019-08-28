/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_cbc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/28 09:27:21 by glegendr          #+#    #+#             */
/*   Updated: 2019/08/28 11:10:37 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include <libft.h>
#include <des.h>

void		in_u8(uint8_t *cpy, uint8_t *in)
{
	in[0] = cpy[0];
	in[1] = cpy[1];
	in[2] = cpy[2];
	in[3] = cpy[3];
	in[4] = cpy[4];
	in[5] = cpy[5];
	in[6] = cpy[6];
	in[7] = cpy[7];
}

void		u8_to_u6(uint8_t *six, uint8_t *eight)
{
	int	i;
	int	y;
	int	x;

	six[0] = (eight[0] & 0xfc) >> 2;
	six[1] = ((eight[0] & 0x3) << 4) | ((eight[1] & 0xf0) >> 4);
	six[2] = ((eight[1] & 0xf) << 2) | ((eight[2] & 0xc0) >> 6);
	six[3] = (eight[2] & 0x3f);
	six[4] = (eight[3] & 0xfc) >> 2;
	six[5] = ((eight[3] & 0x3) << 4) | ((eight[4] & 0xf0) >> 4);
	six[6] = ((eight[4] & 0xf) << 2) | ((eight[5] & 0xc0) >> 6);
	six[7] = (eight[5] & 0x3f);
	i = 0;
	while (i < 8)
	{
		y = ((six[i] & 0x20) >> 4) | (six[i] & 0x1);
		x = (six[i] & 0x1e) >> 1;
		six[i] = g_sbox[i][y][x];
		++i;
	}
}

void		clean_des(t_hash *hash, t_vec *print)
{
	t_ops ops;

	ops = hash->ops;
	if (ops.key)
		free(ops.key);
	if (ops.salt)
		free(ops.salt);
	if (ops.init_vec)
		free(ops.init_vec);
	v_del(print);
	v_del_all(&hash->folder);
	v_del_all(&hash->str);
}

uint8_t		*des_cbc(t_hash *hash, bool print)
{
	return (core_des(hash, print, CBC, 0));
}

uint8_t		*des_pcbc(t_hash *hash, bool print)
{
	return (core_des(hash, print, PCBC, 0));
}
