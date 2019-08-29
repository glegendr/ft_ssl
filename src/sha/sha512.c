/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha512.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 16:47:57 by glegendr          #+#    #+#             */
/*   Updated: 2019/08/29 14:08:32 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include <libft.h>

static void		digest(uint64_t *h, uint8_t *ret)
{
	to_bytes64(h[0], ret);
	to_bytes64(h[1], ret + 8);
	to_bytes64(h[2], ret + 16);
	to_bytes64(h[3], ret + 24);
	to_bytes64(h[4], ret + 32);
	to_bytes64(h[5], ret + 40);
	to_bytes64(h[6], ret + 48);
	to_bytes64(h[7], ret + 56);
}

void			binop512(uint64_t *tmp, uint64_t *m)
{
	int			i;

	i = 0;
	while (i < 80)
		make_512_pseudo_random(tmp, m, i++, 0);
}

void			declare_chunk512(t_vec *ck_init, int y, uint64_t *m)
{
	uint64_t		s0;
	uint64_t		s1;
	uint64_t		x;
	uint8_t			*ck;
	int				i;

	i = 0;
	ck = v_raw(ck_init) + 128 * y;
	while (i < 80)
	{
		if (i < 16)
		{
			m[i] = init_s0(ck, i);
			++i;
			continue ;
		}
		x = m[i - 15];
		s0 = (ROTR(x, 1, 64)) ^ (ROTR(x, 8, 64)) ^ (SHR(x, 7));
		x = m[i - 2];
		s1 = (ROTR(x, 19, 64)) ^ (ROTR(x, 61, 64)) ^ (SHR(x, 6));
		m[i] = m[i - 16] + s0 + m[i - 7] + s1;
		++i;
	}
}

static void		init_h(uint64_t *h)
{
	h[0] = 0x6a09e667f3bcc908;
	h[1] = 0xbb67ae8584caa73b;
	h[2] = 0x3c6ef372fe94f82b;
	h[3] = 0xa54ff53a5f1d36f1;
	h[4] = 0x510e527fade682d1;
	h[5] = 0x9b05688c2b3e6c1f;
	h[6] = 0x1f83d9abfb41bd6b;
	h[7] = 0x5be0cd19137e2179;
}

uint8_t			*sha512(t_hash *tab, bool print)
{
	t_ops ops;

	ops = tab->ops;
	ops.name = "SHA512";
	ops.endian = false;
	ops.loop = 80;
	ops.message_len = 64;
	ops.encodage_len = 128;
	ops.init_h = init_h;
	ops.declare_chunk = declare_chunk512;
	ops.binary_operation = binop512;
	ops.digest = digest;
	ops.fd = tab->ops.fd;
	tab->ops = ops;
	return (launch_hash(tab, print));
}
