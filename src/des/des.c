/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/30 14:02:19 by glegendr          #+#    #+#             */
/*   Updated: 2019/08/30 14:02:24 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include <des.h>

uint8_t		*des_ecb(t_hash *hash, bool print)
{
	return (core_des(hash, print, ECB, 0));
}

uint8_t		*des_cbc(t_hash *hash, bool print)
{
	return (core_des(hash, print, CBC, 0));
}

uint8_t		*des_pcbc(t_hash *hash, bool print)
{
	return (core_des(hash, print, PCBC, 0));
}
