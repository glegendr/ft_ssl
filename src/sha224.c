/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha224.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/30 20:49:14 by glegendr          #+#    #+#             */
/*   Updated: 2019/06/20 15:34:19 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

uint32_t g_k256[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
	0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
	0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
	0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
	0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
	0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 };

void			to_bytes32(uint32_t val, uint8_t *bytes)
{
	bytes[3] = (uint8_t)val;
	bytes[2] = (uint8_t)(val >> 8);
	bytes[1] = (uint8_t)(val >> 16);
	bytes[0] = (uint8_t)(val >> 24);
}

void			make_256_pseudo_random(uint32_t *tmp, uint64_t *m,
									int i, uint32_t s0)
{
	uint32_t	s1;
	uint32_t	ch;
	uint32_t	temp1;
	uint32_t	temp2;
	uint32_t	maj;

	s1 = ROTR(tmp[4], 6, 32) ^ ROTR(tmp[4], 11, 32) ^ ROTR(tmp[4], 25, 32);
	ch = (tmp[4] & tmp[5]) ^ ((~tmp[4]) & tmp[6]);
	temp1 = tmp[7] + s1 + ch + g_k256[i] + ((uint32_t)m[i]);
	s0 = ROTR(tmp[0], 2, 32) ^ ROTR(tmp[0], 13, 32) ^ ROTR(tmp[0], 22, 32);
	maj = (tmp[0] & tmp[1]) ^ (tmp[0] & tmp[2]) ^ (tmp[1] & tmp[2]);
	temp2 = s0 + maj;
	tmp[7] = tmp[6];
	tmp[6] = tmp[5];
	tmp[5] = tmp[4];
	tmp[4] = tmp[3] + temp1;
	tmp[3] = tmp[2];
	tmp[2] = tmp[1];
	tmp[1] = tmp[0];
	tmp[0] = temp1 + temp2;
}

static void		digest(uint64_t *h, uint8_t *ret)
{
	to_bytes32(h[0], ret);
	to_bytes32(h[1], ret + 4);
	to_bytes32(h[2], ret + 8);
	to_bytes32(h[3], ret + 12);
	to_bytes32(h[4], ret + 16);
	to_bytes32(h[5], ret + 20);
	to_bytes32(h[6], ret + 24);
}

static void		init_h(uint64_t *h)
{
	h[0] = 0xc1059ed8;
	h[1] = 0x367cd507;
	h[2] = 0x3070dd17;
	h[3] = 0xf70e5939;
	h[4] = 0xffc00b31;
	h[5] = 0x68581511;
	h[6] = 0x64f98fa7;
	h[7] = 0xbefa4fa4;
}

void			sha224(t_hash *tab)
{
	t_ops ops;

	ops.name = "SHA224";
	ops.endian = false;
	ops.loop = 64;
	ops.message_len = 28;
	ops.encodage_len = 64;
	ops.init_h = init_h;
	ops.declare_chunk = declare_chunk256;
	ops.binary_operation = binop256;
	ops.digest = digest;
	ops.fd = tab->ops.fd;
	tab->ops = ops;
	launch_hash(tab);
}
