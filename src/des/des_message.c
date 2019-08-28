/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_message.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/14 07:33:06 by glegendr          #+#    #+#             */
/*   Updated: 2019/08/28 12:17:39 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include <libft.h>
#include <des.h>
#define DEL_SALT() { print.private_content += 16; print.private_elem_nb -= 16; }
#define PUSH_SALT(p, h) {v_append_raw(p, "Salted__", 8); v_append_raw(p, h, 8);}

uint64_t	divide_message(uint8_t *str, uint8_t fin_keys[16][6], bool rev)
{
	uint32_t	ln;
	uint32_t	rn;
	uint64_t	tmp;
	uint64_t	end;
	int			i;

	ln = u8_to_b32(str);
	rn = u8_to_b32(str + 4);
	i = 0;
	while (i < 16)
	{
		tmp = ln;
		ln = rn;
		rn = tmp ^ f(ln, fin_keys[rev ? 15 - i : i]);
		++i;
	}
	tmp = rn;
	tmp = (tmp << 32) | ln;
	end = 0;
	i = 0;
	while (i < 64)
		end = (end << 1) | ((tmp >> (64 - g_ip1[i++])) & 0x1);
	return (end);
}

void		pkcs5_pad(t_vec *vec)
{
	int	len;
	int	i;

	i = 0;
	len = 8 - (v_size(vec) % 8);
	while (i < len)
	{
		v_push_int(vec, len);
		++i;
	}
}

uint8_t		*print_des_message(t_hash *hash, t_vec *print, bool rev, bool bp)
{
	t_hash	tmp;
	t_vec	vec;

	if (hash->arg & A_FLAG && !rev)
	{
		vec = v_new(sizeof(t_vec));
		v_push(&vec, print);
		tmp = *hash;
		tmp.str = vec;
		tmp.arg |= Q_FLAG;
		return (base64(&tmp, bp));
	}
	else if (!bp)
		return (v_raw(print));
	else
		write(hash->ops.fd, v_raw(print), v_size(print));
	return (NULL);
}

void		xor_message(uint8_t *str, uint8_t *xor, enum e_des_mode mode, int y)
{
	uint8_t	*tmp;
	int		i;

	i = 0;
	if (mode == PCBC && y != 0)
	{
		tmp = str + 8 * (y - 1);
		while (i < 8)
		{
			xor[i] ^= tmp[i];
			++i;
		}
	}
	tmp = str + 8 * y;
	i = 0;
	if (mode == ECB)
		in_u8(tmp, xor);
	else
		while (i < 8)
		{
			xor[i] ^= tmp[i];
			++i;
		}
}

uint8_t		*hash_des_message(t_hash *hash, uint8_t div_key[16][6], bool bp,
								enum e_des_mode mode)
{
	uint8_t		*str;
	t_vec		print;
	uint8_t		xor[8];
	int			y;

	y = 0;
	print = v_new(sizeof(uint8_t));
	if (mode != ECB)
		in_u8(hash->ops.init_vec, xor);
	if (hash->ops.salt)
		PUSH_SALT(&print, hash->ops.salt);
	pkcs5_pad(v_get(&hash->str, 0));
	str = v_raw(v_get(&hash->str, 0));
	while (y < v_size(v_get(&hash->str, 0)) / 8)
	{
		xor_message(str, xor, mode, y++);
		ip(xor);
		to_bytes64(divide_message(xor, div_key, false), xor);
		v_append_raw(&print, xor, 8);
	}
	if (!bp)
		return (print_des_message(hash, &print, false, bp));
	print_des_message(hash, &print, false, bp);
	clean_des(hash, &print);
	return (NULL);
}
