/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generic.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 16:13:33 by glegendr          #+#    #+#             */
/*   Updated: 2019/09/04 14:12:54 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>
#include <libft.h>

static void	transform_hash(uint8_t *ret, t_hash *hash, int i, t_ops ops)
{
	t_vec	vec;
	char	*hex;
	int		y;

	y = 0;
	vec = v_new(sizeof(char));
	while (y < ops.message_len)
	{
		hex = ft_itoa_base(ret[y], 16, 'x');
		if (ft_strlen(hex) == 1)
			v_push_int(&vec, '0');
		v_append_raw(&vec, hex, ft_strlen(hex));
		free(hex);
		++y;
	}
	print_hash(&vec, hash, i, ops);
	v_del(&vec);
}

static void	attribute_value(uint64_t *dst, uint64_t *src, bool addition)
{
	if (!addition)
	{
		dst[0] = src[0];
		dst[1] = src[1];
		dst[2] = src[2];
		dst[3] = src[3];
		dst[4] = src[4];
		dst[5] = src[5];
		dst[6] = src[6];
		dst[7] = src[7];
		return ;
	}
	dst[0] += src[0];
	dst[1] += src[1];
	dst[2] += src[2];
	dst[3] += src[3];
	dst[4] += src[4];
	dst[5] += src[5];
	dst[6] += src[6];
	dst[7] += src[7];
}

static void	encript(t_vec *ck, t_ops ops, uint8_t *ret, int loop)
{
	uint64_t	m[80];
	uint64_t	h[8];
	uint64_t	tmp[8];
	int			y;

	ft_bzero(m, sizeof(uint64_t) * 80);
	ft_bzero(h, sizeof(uint64_t) * 8);
	ft_bzero(tmp, sizeof(uint64_t) * 8);
	y = 0;
	ops.init_h(h);
	while (y < loop)
	{
		ops.declare_chunk(ck, y, m);
		attribute_value(tmp, h, false);
		ops.binary_operation(tmp, m);
		attribute_value(h, tmp, true);
		++y;
	}
	ops.digest(h, ret);
}

uint8_t		*copy_ret(uint8_t *ret, int len)
{
	uint8_t	*cpy;
	int		i;

	i = 0;
	if ((cpy = (uint8_t *)malloc(len * sizeof(uint8_t))) == NULL)
		return (NULL);
	while (i < len)
	{
		cpy[i] = ret[i];
		++i;
	}
	return (cpy);
}

uint8_t		*launch_hash(t_hash *hash, bool print)
{
	int		i;
	int		final_len;
	uint8_t	ret[64];
	t_ops	ops;

	ops = hash->ops;
	i = 0;
	if (!v_get(&hash->str, 0))
		CLEAN_AND_PRINT(hash, NULL);
	while (i < v_size(&hash->str))
	{
		final_len = pad_message(v_get(&hash->str, i), ops.endian,
								ops.encodage_len);
		encript(v_get(&hash->str, i), ops, ret, final_len / ops.encodage_len);
		if (!print)
			return (copy_ret(ret, ops.message_len));
		transform_hash(ret, hash, i, ops);
		++i;
	}
	clean_des(hash, NULL);
	return (NULL);
}
