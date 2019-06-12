/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha224.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/30 20:49:14 by glegendr          #+#    #+#             */
/*   Updated: 2019/06/12 11:59:57 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

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
