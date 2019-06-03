/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/16 18:27:36 by glegendr          #+#    #+#             */
/*   Updated: 2019/06/03 17:38:27 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <ft_printf.h>
#include <libft.h>
#include "ft_ssl.h"
#include "md5.h"

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

int			pad_message(t_vec *tab, bool endian, int mod)
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

static void to_bytes32_endian(uint32_t val, uint8_t *bytes)
{
	bytes[0] = (uint8_t) val;
	bytes[1] = (uint8_t) (val >> 8);
	bytes[2] = (uint8_t) (val >> 16);
	bytes[3] = (uint8_t) (val >> 24);
}

static void		digest(uint64_t *h, uint8_t *ret)
{
	to_bytes32_endian(h[0], ret);
	to_bytes32_endian(h[1], ret + 4);
	to_bytes32_endian(h[2], ret + 8);
	to_bytes32_endian(h[3], ret + 12);
}

static void		binop(uint64_t *lul, uint64_t *m)
{
	uint32_t	f;
	uint32_t	g;
	uint32_t	temp;
	uint32_t	tab[4] = {lul[0], lul[1], lul[2], lul[3]};

	for (int i = 0; i < 64; i++) {
		if (i < 16) {
			f = (tab[1] & tab[2]) | ((~tab[1]) & tab[3]);
			g = i;
		} else if (i < 32) {
			f = (tab[3] & tab[1]) | ((~tab[3]) & tab[2]);
			g = (5 * i + 1) % 16;
		} else if (i < 48) {
			f = tab[1] ^ tab[2] ^ tab[3];
			g = (3 * i + 5) % 16;
		} else {
			f = tab[2] ^ (tab[1] | (~tab[3]));
			g = (7 * i) % 16;
		}
		temp = tab[3];
		tab[3] = tab[2];
		tab[2] = tab[1];
		tab[1] = tab[1] +
				 ROTL((tab[0] + f + g_k[i] + ((uint32_t)m[g])), g_s[i], 32);
		tab[0] = temp;
		lul[0] = tab[0];
		lul[1] = tab[1];
		lul[2] = tab[2];
		lul[3] = tab[3];
	}
}

static void		declare_chunk(t_vec *ck_init, int y, uint64_t *m)
{
	uint8_t		*ck;

	ck = v_raw(ck_init) + 64 * y;
	for (int i = 0; i < 16; ++i)
		m[i] = (ck[i * 4 + 3] << 24) + (ck[i * 4 + 2] << 16)
				+ (ck[i * 4 + 1] << 8) + ck[i * 4];

}

static void		init_h(uint64_t *h)
{
	h[0] = 0x67452301;
	h[1] = 0xefcdab89;
	h[2] = 0x98badcfe;
	h[3] = 0x10325476;
}

void		md5(t_hash *tab)
{
	t_ops ops;

	ops.name = "MD5";
	ops.endian = true;
	ops.loop = 0;
	ops.message_len = 16;
	ops.encodage_len = 64;
	ops.init_h = init_h;
	ops.declare_chunk = declare_chunk;
	ops.binary_operation = binop;
	ops.digest = digest;
	tab->ops = ops;
	launch_hash(tab);
}
