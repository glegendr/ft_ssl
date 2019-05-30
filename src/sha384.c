/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha384.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/30 20:49:23 by glegendr          #+#    #+#             */
/*   Updated: 2019/05/30 20:49:34 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

static void	digest(uint64_t *h, uint8_t *ret)
{
	to_bytes64(h[0], ret);
	to_bytes64(h[1], ret + 8);
	to_bytes64(h[2], ret + 16);
	to_bytes64(h[3], ret + 24);
	to_bytes64(h[4], ret + 32);
	to_bytes64(h[5], ret + 40);
}

static void	init_h(uint64_t *h)
{
	h[0] = 0xcbbb9d5dc1059ed8;
	h[1] = 0x629a292a367cd507;
	h[2] = 0x9159015a3070dd17;
	h[3] = 0x152fecd8f70e5939;
	h[4] = 0x67332667ffc00b31;
	h[5] = 0x8eb44a8768581511;
	h[6] = 0xdb0c2e0d64f98fa7;
	h[7] = 0x47b5481dbefa4fa4;
}

void		sha384(t_hash *tab)
{
	t_ops ops;

	ops.name = "SHA384";
	ops.endian = false;
	ops.loop = 80;
	ops.message_len = 48;
	ops.encodage_len = 128;
	ops.init_h = init_h;
	ops.declare_chunk = declare_chunk512;
	ops.binary_operation = binop512;
	ops.digest = digest;
	tab->ops = ops;
	launch_hash(tab);
}
