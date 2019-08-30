/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 16:44:12 by glegendr          #+#    #+#             */
/*   Updated: 2019/08/30 14:15:01 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>

static void		digest(uint64_t *h, uint8_t *ret)
{
	to_bytes32(h[0], ret);
	to_bytes32(h[1], ret + 4);
	to_bytes32(h[2], ret + 8);
	to_bytes32(h[3], ret + 12);
	to_bytes32(h[4], ret + 16);
	to_bytes32(h[5], ret + 20);
	to_bytes32(h[6], ret + 24);
	to_bytes32(h[7], ret + 28);
}

void			binop256(uint64_t *init, uint64_t *m)
{
	uint32_t	tmp[8];
	int			i;

	i = 0;
	tmp[0] = init[0];
	tmp[1] = init[1];
	tmp[2] = init[2];
	tmp[3] = init[3];
	tmp[4] = init[4];
	tmp[5] = init[5];
	tmp[6] = init[6];
	tmp[7] = init[7];
	while (i < 64)
		make_256_pseudo_random(tmp, m, i++, 0);
	init[0] = tmp[0];
	init[1] = tmp[1];
	init[2] = tmp[2];
	init[3] = tmp[3];
	init[4] = tmp[4];
	init[5] = tmp[5];
	init[6] = tmp[6];
	init[7] = tmp[7];
}

void			declare_chunk256(t_vec *ck_init, int y, uint64_t *m)
{
	uint32_t		s0;
	uint32_t		s1;
	uint32_t		x;
	uint8_t			*ck;
	int				i;

	i = 0;
	ck = v_raw(ck_init) + 64 * y;
	while (i < 64)
	{
		if (i < 16)
		{
			s0 = (ck[i * 4] << 24) + (ck[i * 4 + 1] << 16)
				+ (ck[i * 4 + 2] << 8) + ck[i * 4 + 3];
			m[i] = s0;
			++i;
			continue ;
		}
		x = m[i - 15];
		s0 = (ROTR(x, 7, 32)) ^ (ROTR(x, 18, 32)) ^ (SHR(x, 3));
		x = m[i - 2];
		s1 = (ROTR(x, 17, 32)) ^ (ROTR(x, 19, 32)) ^ (SHR(x, 10));
		m[i] = m[i - 16] + s0 + m[i - 7] + s1;
		++i;
	}
}

static void		init_h(uint64_t *h)
{
	h[0] = 0x6a09e667;
	h[1] = 0xbb67ae85;
	h[2] = 0x3c6ef372;
	h[3] = 0xa54ff53a;
	h[4] = 0x510e527f;
	h[5] = 0x9b05688c;
	h[6] = 0x1f83d9ab;
	h[7] = 0x5be0cd19;
}

uint8_t			*sha256(t_hash *tab, bool print)
{
	t_ops ops;

	ops = tab->ops;
	ops.name = "SHA256";
	ops.endian = false;
	ops.message_len = 32;
	ops.encodage_len = 64;
	ops.init_h = init_h;
	ops.declare_chunk = declare_chunk256;
	ops.binary_operation = binop256;
	ops.digest = digest;
	ops.fd = tab->ops.fd;
	tab->ops = ops;
	return (launch_hash(tab, print));
}
