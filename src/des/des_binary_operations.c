/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_binary_operations.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/28 10:34:21 by glegendr          #+#    #+#             */
/*   Updated: 2019/08/30 14:09:05 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>
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
