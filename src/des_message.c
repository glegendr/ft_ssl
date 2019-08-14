/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_message.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/14 07:33:06 by glegendr          #+#    #+#             */
/*   Updated: 2019/08/14 08:41:17 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include <libft.h>
#include <des.h>

void		ip(uint8_t *str)
{
	int		i;
	uint8_t	ret[8] = {0};

	i = 0;
	while (i < 64)
	{
		bit(ret, i + 1, str, g_ip[i]);
		++i;
	}
	i = 0;
	while (i < 8)
	{
		str[i] = ret[i];
		++i;
	}
}

uint32_t	u8_to_b32(uint8_t *val)
{
	uint32_t	bytes;
	int			i;

	i = 0;
	while (i < 4)
	{
		bytes = bytes << 8;
		bytes = bytes | val[i];
		++i;
	}
	return (bytes);
}

void		e_bit(uint8_t *ret, uint32_t ln)
{
	int		i;
	uint8_t	str[4] = {0};

	to_bytes32(ln, str);
	i = 0;
	while (i < 48)
	{
		bit(ret, i + 1, str, g_e_bit[i]);
		++i;
	}
}

void		u8_to_u6(uint8_t *six, uint8_t *eight)
{
	int	i;
	int	y;
	int	x;

	six[0] = (eight[0] & 0xfc) >> 2;
	six[1] = ((eight[0] & 0x3) << 4) | ((eight[1] & 0xf0) >> 4);
	six[2] = ((eight[1] & 0xf) << 2) | ((eight[2] & 0xc0) >> 6);
	six[3] = (eight[2] & 0x3f);
	six[4] = (eight[3] & 0xfc) >> 2;
	six[5] = ((eight[3] & 0x3) << 4) | ((eight[4] & 0xf0) >> 4);
	six[6] = ((eight[4] & 0xf) << 2) | ((eight[5] & 0xc0) >> 6);
	six[7] = (eight[5] & 0x3f);
	i = 0;
	while (i < 8)
	{
		y = ((six[i] & 0x20) >> 4) | (six[i] & 0x1);
		x = (six[i] & 0x1e) >> 1;
		six[i] = g_sbox[i][y][x];
		++i;
	}
}

uint32_t	p(uint8_t *six)
{
	uint32_t	start;
	uint32_t	ret;
	int			i;

	start = 0;
	ret = 0;
	i = 0;
	while (i < 8)
	{
		start = start << 4;
		start = start | (six[i] & 0xf);
		++i;
	}
	i = 0;
	while (i < 32)
	{
		ret = ret << 1;
		ret = ret | ((start >> (32 - g_p[i])) & 0x1);
		++i;
	}
	return (ret);
}

uint32_t	f(uint32_t ln, uint8_t *key)
{
	uint8_t	tmp[6] = {0};
	uint8_t	six_bit[8] = {0};
	int		i;

	i = 0;
	e_bit(tmp, ln);
	while (i < 6)
	{
		tmp[i] = tmp[i] ^ key[i];
		++i;
	}
	u8_to_u6(six_bit, tmp);
	return (p(six_bit));
}

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

void		xor_message(uint8_t *str, uint8_t *xor, enum des_mode mode, int y)
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

void		clean_des(t_hash *hash, t_vec *print)
{
	t_ops ops;

	ops = hash->ops;
	if (ops.key)
		free(ops.key);
	if (ops.salt)
		free(ops.salt);
	if (ops.init_vec)
		free(ops.init_vec);
	v_del(print);
	v_del_all(&hash->folder);
	v_del_all(&hash->str);

}

uint8_t		*hash_des_message(t_hash *hash, uint8_t div_key[16][6], bool bp, enum des_mode mode)
{
	t_ops		ops;
	int			i;
	uint64_t	div_ret;
	uint8_t		*str;
	t_vec		print;
	uint8_t		xor[8] = {0};

	i = 0;
	print = v_new(sizeof(uint8_t));
	ops = hash->ops;
	if (mode != ECB)
		in_u8(ops.init_vec, xor);
	if (ops.salt)
	{
		v_append_raw(&print, "Salted__", 8);
		v_append_raw(&print, ops.salt, 8);
	}
	while (i < v_size(&hash->str))
	{
		pkcs5_pad(v_get(&hash->str, i));
		str = v_raw(v_get(&hash->str, i));
		for (int y = 0; y < v_size(v_get(&hash->str, i)) / 8; ++y)
		{
			xor_message(str, xor, mode, y);
			ip(xor);
			div_ret = divide_message(xor, div_key, false);
			to_bytes64(div_ret, xor);
			for (int z = 0; z < 8; ++z)
				v_push_int(&print, ((div_ret >> (64 - (8 * (z + 1)))) & 0xff));
		}
		if (!bp)
			return (print_des_message(hash, &print, false, bp));
		print_des_message(hash, &print, false, bp);
		v_reset(&print);
		++i;
	}
	clean_des(hash, &print);
	return (NULL);
}

uint64_t	xor_message64(uint64_t mess, uint8_t *xor, enum des_mode mode,
							uint8_t *str, int y)
{
	uint64_t	ret;
	uint64_t	pcbc;

	pcbc = 0;
	ret = 0;
	if (mode == ECB)
		return (mess);
	for (int i = 0; i < 8; ++i)
	{
		ret = ret << 8;
		ret |= str[i];
	}
	if (mode == PCBC && y != 0)
	{
		for (int i = 0; i < 8; ++i)
		{
			pcbc = pcbc << 8;
			pcbc |= xor[i];
		}
		ret ^= pcbc;
	}
	return (ret ^ mess);
}

uint8_t		*unhash_des_message(t_hash *hash, uint8_t div_key[16][6],
								bool bp, enum des_mode mode)
{
	uint64_t	mess;
	t_hash		tmp;
	t_vec		vec;
	uint8_t		*str;
	uint8_t		xor[8] = {0};
	t_vec		print;
	int			i;

	i = 0;
	if (hash->ops.init_vec)
		in_u8(hash->ops.init_vec, xor);
	print = v_new(sizeof(uint8_t));
	while (i < v_size(&hash->str))
	{
		if (hash->arg & A_FLAG)
		{
			vec = v_new(sizeof(t_vec));
			v_push(&vec, v_get(&hash->str, i));
			tmp = *hash;
			tmp.str = vec;
			tmp.arg |= D_FLAG;
			str = base64(&tmp, false);
		}
		else
			str = v_raw(v_get(&hash->str, i));
		if (!hash->ops.key)
			str += 16;
		for (unsigned long int y = 0; y < (ft_strlen((char *)str)) / 8; ++y)
		{
			uint8_t tmp[8] = {0};

			in_u8(str + 8 * y, tmp);
			ip(tmp);
			mess = divide_message(tmp, div_key, true);
			mess = xor_message64(mess, xor, mode, y == 0 ? xor : str + 8 * (y - 1), y);
			to_bytes64(mess, xor);
			for (int z = 0; z < 8; ++z)
				v_push_int(&print, ((mess >> (56 - (8 * z))) & 0xff));
		}
		if (!bp)
			return (print_des_message(hash, &print, true, bp));
		print_des_message(hash, &print, true, bp);
		v_reset(&print);
		++i;
	}
	clean_des(hash, &print);
	return (NULL);
}
