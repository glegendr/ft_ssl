/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 16:23:20 by glegendr          #+#    #+#             */
/*   Updated: 2019/08/26 14:07:14 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdint.h>
#include <libft.h>
#include "ft_ssl.h"

void			append_ret(uint8_t *ret, uint8_t *index, t_vec *print, int i)
{
	if (i == 5)
		i = 3;
	if (i > 0)
		ret[0] = (index[0] << 2) | ((index[1] & 0x30) >> 4);
	if (i > 1)
		ret[1] = ((index[1] & 0xf) << 4) | ((index[2] & 0x3c) >> 2);
	if (i > 2)
		ret[2] = ((index[2] & 0x3) << 6) | index[3];
	v_append_raw(print, ret, i - 1);
}

void			init_tabs(uint8_t *ret, uint8_t *index)
{
	ret[0] = 0;
	ret[1] = 0;
	ret[2] = 0;
	index[0] = 255;
	index[1] = 255;
	index[2] = 255;
	index[3] = 255;
}

int				decript_base(t_vec *vec, char *base, int *z, t_vec *print)
{
	uint8_t	ret[3];
	uint8_t	index[4];
	int		i;
	uint8_t	*raw;

	raw = (uint8_t *)v_raw(vec) + *z;
	init_tabs(ret, index);
	i = 0;
	while (i < 4 && *z + i < v_size(vec))
	{
		while (ft_isspace(raw[i]) && *z < v_size(vec))
		{
			++raw;
			++(*z);
		}
		if (*z >= v_size(vec))
			break ;
		if (get_index(raw, base, index, &i))
			return (append_error(print));
		++i;
	}
	if (*z < v_size(vec))
		append_ret(ret, index, print, i);
	return (0);
}

uint8_t			*bases(t_hash *tab, char *base, int z, bool print_b)
{
	t_vec	print;
	t_vec	*vec;
	int		i;

	i = 0;
	print = v_new(sizeof(char));
	while (i < v_size(&tab->str))
	{
		vec = v_get(&tab->str, i);
		while (z < v_size(vec))
		{
			if (tab->arg & D_FLAG)
			{
				if (decript_base(vec, base, &z, &print))
					break ;
				++z;
			}
			else
				encript_base(v_raw(vec) + z, base, v_size(vec) - z, &print);
			z += 3;
		}
		tab->ops.message_len = v_size(&print);
		if (!print_b)
			return ((uint8_t *)v_raw(&print));
		print_hash(&print, tab, i++, tab->ops);
		v_reset(&print);
	}
	v_del(&print);
	return (NULL);
}

uint8_t			*base64(t_hash *tab, bool print)
{
	char base[65];

	generate_base(base, false);
	tab->ops.name = "BASE64";
	return (bases(tab, base, 0, print));
}
