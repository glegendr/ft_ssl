/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha512.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 16:47:57 by glegendr          #+#    #+#             */
/*   Updated: 2019/06/19 17:36:12 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include <libft.h>

uint64_t g_k512[80] = {
	0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f,
	0xe9b5dba58189dbbc, 0x3956c25bf348b538, 0x59f111f1b605d019,
	0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 0xd807aa98a3030242,
	0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
	0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235,
	0xc19bf174cf692694, 0xe49b69c19ef14ad2, 0xefbe4786384f25e3,
	0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65, 0x2de92c6f592b0275,
	0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
	0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f,
	0xbf597fc7beef0ee4, 0xc6e00bf33da88fc2, 0xd5a79147930aa725,
	0x06ca6351e003826f, 0x142929670a0e6e70, 0x27b70a8546d22ffc,
	0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
	0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6,
	0x92722c851482353b, 0xa2bfe8a14cf10364, 0xa81a664bbc423001,
	0xc24b8b70d0f89791, 0xc76c51a30654be30, 0xd192e819d6ef5218,
	0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
	0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99,
	0x34b0bcb5e19b48a8, 0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb,
	0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3, 0x748f82ee5defb2fc,
	0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
	0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915,
	0xc67178f2e372532b, 0xca273eceea26619c, 0xd186b8c721c0c207,
	0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 0x06f067aa72176fba,
	0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
	0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc,
	0x431d67c49c100d4c, 0x4cc5d4becb3e42b6, 0x597f299cfc657e2a,
	0x5fcb6fab3ad6faec, 0x6c44198c4a475817};

void			to_bytes64(uint64_t val, uint8_t *bytes)
{
	bytes[7] = (uint8_t)val;
	bytes[6] = (uint8_t)(val >> 8);
	bytes[5] = (uint8_t)(val >> 16);
	bytes[4] = (uint8_t)(val >> 24);
	bytes[3] = (uint8_t)(val >> 32);
	bytes[2] = (uint8_t)(val >> 40);
	bytes[1] = (uint8_t)(val >> 48);
	bytes[0] = (uint8_t)(val >> 56);
}

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
	uint64_t	s1;
	uint64_t	ch;
	uint64_t	temp1;
	uint64_t	s0;
	uint64_t	maj;
	uint64_t	temp2;
	int			i;

	i = 0;
	while (i < 80)
	{
		s1 = ROTR(tmp[4], 14, 64) ^ ROTR(tmp[4], 18, 64) ^ ROTR(tmp[4], 41, 64);
		ch = (tmp[4] & tmp[5]) ^ ((~tmp[4]) & tmp[6]);
		temp1 = tmp[7] + s1 + ch + g_k512[i] + m[i];
		s0 = ROTR(tmp[0], 28, 64) ^ ROTR(tmp[0], 34, 64) ^ ROTR(tmp[0], 39, 64);
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
		++i;
	}
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
		s0 = 0;
		y = 0;
		if (i < 16)
		{
			while (y < 8)
			{
				s0 = s0 << 8;
				s0 = s0 + ck[i * 8 + y];
				++y;
			}
			m[i] = s0;
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

void			sha512(t_hash *tab)
{
	t_ops ops;

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
	launch_hash(tab);
}
