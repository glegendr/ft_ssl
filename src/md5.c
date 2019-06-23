/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/16 18:27:36 by glegendr          #+#    #+#             */
/*   Updated: 2019/06/23 15:49:24 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdint.h>
#include <libft.h>
#include "ft_ssl.h"
#include "md5.h"

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
	uint32_t	tab[4];
	int			i;

	i = 0;
	instance_tab(tab, lul);
	while (i < 64)
	{
		g = make_md5_pseudo_random(i, tab, &f);
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
		++i;
	}
}

static void		declare_chunk(t_vec *ck_init, int y, uint64_t *m)
{
	uint8_t		*ck;
	int			i;

	i = 0;
	ck = v_raw(ck_init) + 64 * y;
	while (i < 16)
	{
		m[i] = (ck[i * 4 + 3] << 24) + (ck[i * 4 + 2] << 16)
				+ (ck[i * 4 + 1] << 8) + ck[i * 4];
		++i;
	}
}

static void		init_h(uint64_t *h)
{
	h[0] = 0x67452301;
	h[1] = 0xefcdab89;
	h[2] = 0x98badcfe;
	h[3] = 0x10325476;
}

void			md5(t_hash *tab)
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
	ops.fd = tab->ops.fd;
	tab->ops = ops;
	launch_hash(tab);
}
