/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_message.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/14 07:33:06 by glegendr          #+#    #+#             */
/*   Updated: 2019/08/26 14:36:07 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include <libft.h>
#include <des.h>

void		ip(uint8_t *str)
{
	int		i;
	uint8_t	ret[8];

	ft_bzero(ret, 8 * sizeof(uint8_t));
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
	uint8_t	str[4];

	ft_bzero(str, 4 * sizeof(uint8_t));
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
	uint8_t	tmp[6];
	uint8_t	six_bit[8];
	int		i;

	ft_bzero(tmp, 6 * sizeof(uint8_t));
	ft_bzero(six_bit, 8 * sizeof(uint8_t));
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

#define PUSH_SALT(p, h) {v_append_raw(p, "Salted__", 8); v_append_raw(p, h, 8);}

uint8_t		*hash_des_message(t_hash *hash, uint8_t div_key[16][6], bool bp,
								enum des_mode mode)
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

uint64_t	xor_message64(uint64_t mess, uint8_t *xor, enum des_mode mode,
							uint8_t *str, int y)
{
	uint64_t	ret;
	uint64_t	pcbc;
	int			i;

	pcbc = 0;
	ret = 0;
	i = 0;
	if (mode == ECB)
		return (mess);
	while (i < 8)
	{
		ret = ret << 8;
		ret |= str[i++];
	}
	if (mode == PCBC && y != 0)
	{
		i = 0;
		while (i < 8)
		{
			pcbc = pcbc << 8;
			pcbc |= xor[i++];
		}
		ret ^= pcbc;
	}
	return (ret ^ mess);
}

uint8_t		*unhash_base64(t_hash *hash, int *len)
{
	t_hash		tmp;
	t_vec		vec;
	uint8_t		*ret;

	vec = v_new(sizeof(t_vec));
	v_push(&vec, v_get(&hash->str, 0));
	tmp = *hash;
	tmp.str = vec;
	tmp.arg |= D_FLAG;
	ret = base64(&tmp, false);
	*len = tmp.ops.message_len;
	return (ret);
}

void		found_pad(t_vec *print)
{
	uint8_t tmp;

	tmp = *(uint8_t *)v_get(print, v_size(print) - 1);
	if (tmp > 8)
		return ;
	while (tmp > 0)
	{
		v_del_last(print);
		--tmp;
	}
}

# define DEL_SALT() print.private_content += 16;print.private_elem_nb -= 16
# define TEST       enum des_mode mode, uint8_t *str, t_vec *print

void		launch_unhash(t_hash *hash, int len, uint8_t div_key[16][6],
							enum des_mode mode, uint8_t *str, t_vec *print)
{
	uint8_t		xor[8];
	uint64_t	mess;
	uint8_t		tmp[8];
	int			y;

	ft_bzero(xor, 8 * sizeof(uint8_t));
	ft_bzero(tmp, 8 * sizeof(uint8_t));
	if (hash->ops.init_vec)
		in_u8(hash->ops.init_vec, xor);
	y = 0;
	while (y < len / 8)
	{
		in_u8(str + 8 * y, tmp);
		ip(tmp);
		mess = divide_message(tmp, div_key, true);
		mess = xor_message64(mess, xor, mode, y == 0 ? xor : str + 8 * (y - 1), y);
		to_bytes64(mess, xor);
		v_append_raw(print, xor, 8);
		++y;
	}
}

uint8_t		*unhash_des_message(t_hash *hash, uint8_t div_key[16][6],
								bool bp, enum des_mode mode)
{
	uint8_t		*str;
	t_vec		print;
	int			len;

	print = v_new(sizeof(uint8_t));
	if (hash->arg & A_FLAG)
		str = unhash_base64(hash, &len);
	else
	{
		str = v_raw(v_get(&hash->str, 0));
		len = v_size(v_get(&hash->str, 0));
	}
	launch_unhash(hash, len, div_key, mode, str, &print);
	found_pad(&print);
	if (!hash->ops.key)
	{
//		DEL_SALT();
	}
	if (!bp)
		return (print_des_message(hash, &print, true, bp));
	print_des_message(hash, &print, true, bp);
	if (!hash->ops.key)
		print.private_content -= 16;
	clean_des(hash, &print);
	return (NULL);
}
