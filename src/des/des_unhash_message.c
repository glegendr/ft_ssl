/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_unhash_message.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/28 10:28:43 by glegendr          #+#    #+#             */
/*   Updated: 2019/08/30 14:09:39 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>
#include <libft.h>
#include <des.h>
#define DEL_SALT() { print.private_content += 16; print.private_elem_nb -= 16; }
#define CNORM(x, y, z) t_norm ret_norm = {.mode = x, .str = y, .len = z}

uint64_t	xor_message64(uint64_t mess, uint8_t *xor, t_norm norm)
{
	uint64_t	ret;
	uint64_t	pcbc;
	int			i;

	pcbc = 0;
	ret = 0;
	i = 0;
	if (norm.mode == ECB)
		return (mess);
	while (i < 8)
	{
		ret = ret << 8;
		ret |= norm.str[i++];
	}
	if (norm.mode == PCBC && norm.len != 0)
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

void		launch_unhash(t_hash *hash, t_vec *print, uint8_t div_key[16][6],
							t_norm norm)
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
	while (y < norm.len / 8)
	{
		in_u8(norm.str + 8 * y, tmp);
		ip(tmp);
		mess = divide_message(tmp, div_key, true);
		CNORM(norm.mode, y == 0 ? xor : norm.str + 8 * (y - 1), y);
		mess = xor_message64(mess, xor, ret_norm);
		to_bytes64(mess, xor);
		v_append_raw(print, xor, 8);
		++y;
	}
}

uint8_t		*unhash_des_message(t_hash *hash, uint8_t div_key[16][6],
								bool bp, enum e_des_mode mode)
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
	CNORM(mode, str, len);
	launch_unhash(hash, &print, div_key, ret_norm);
	found_pad(&print);
	if (!hash->ops.key)
		DEL_SALT();
	if (!bp)
		return (print_des_message(hash, &print, true, bp));
	print_des_message(hash, &print, true, bp);
	if (!hash->ops.key)
		print.private_content -= 16;
	clean_des(hash, &print);
	return (NULL);
}
