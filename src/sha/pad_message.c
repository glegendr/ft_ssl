/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pad_message.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/20 15:59:30 by glegendr          #+#    #+#             */
/*   Updated: 2019/08/30 14:11:39 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>

static void		push_original_len(t_vec *tab,
								uint64_t original_len, bool endian, int mult)
{
	int loop;

	loop = 0;
	while (mult > 8)
	{
		v_push_int(tab, 0);
		--mult;
	}
	while (loop < 8)
	{
		if (endian)
			v_push_int(tab, ((original_len >> (8 * loop)) & 0xff));
		else
			v_push_int(tab, (original_len >> (56 - (8 * loop)) & 0xff));
		++loop;
	}
}

int				pad_message(t_vec *tab, bool endian, int mod)
{
	int		len;
	int		original_len;

	original_len = v_size(tab);
	len = original_len + 1;
	while (len % mod)
		++len;
	if (original_len + 8 >= len)
		len += mod;
	len -= mod / 8;
	v_push_int(tab, (char)128);
	while (v_size(tab) < len)
		v_push_int(tab, 0);
	push_original_len(tab, (uint64_t)original_len * 8, endian, mod / 8);
	return (v_size(tab));
}

void			to_bytes32_endian(uint32_t val, uint8_t *bytes)
{
	bytes[0] = (uint8_t)val;
	bytes[1] = (uint8_t)(val >> 8);
	bytes[2] = (uint8_t)(val >> 16);
	bytes[3] = (uint8_t)(val >> 24);
}

uint32_t		make_md5_pseudo_random(int i, uint32_t *tab, uint32_t *f)
{
	if (i < 16)
	{
		*f = (tab[1] & tab[2]) | ((~tab[1]) & tab[3]);
		return (i);
	}
	else if (i < 32)
	{
		*f = (tab[3] & tab[1]) | ((~tab[3]) & tab[2]);
		return ((5 * i + 1) % 16);
	}
	else if (i < 48)
	{
		*f = tab[1] ^ tab[2] ^ tab[3];
		return ((3 * i + 5) % 16);
	}
	else
	{
		*f = tab[2] ^ (tab[1] | (~tab[3]));
		return ((7 * i) % 16);
	}
}

void			instance_tab(uint32_t *tab, uint64_t *lul)
{
	tab[0] = lul[0];
	tab[1] = lul[1];
	tab[2] = lul[2];
	tab[3] = lul[3];
}
