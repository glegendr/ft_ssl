/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_cbc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/28 09:27:21 by glegendr          #+#    #+#             */
/*   Updated: 2019/08/28 09:30:01 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include <libft.h>
#include <des.h>

void		in_u8(uint8_t *cpy, uint8_t *in)
{
	in[0] = cpy[0];
	in[1] = cpy[1];
	in[2] = cpy[2];
	in[3] = cpy[3];
	in[4] = cpy[4];
	in[5] = cpy[5];
	in[6] = cpy[6];
	in[7] = cpy[7];
}

uint8_t		*des_cbc(t_hash *hash, bool print)
{
	return (core_des(hash, print, CBC, 0));
}
