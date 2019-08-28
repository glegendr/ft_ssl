/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/28 11:32:09 by glegendr          #+#    #+#             */
/*   Updated: 2019/08/28 11:40:58 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>

void		e_fct(t_hash *tab)
{
	if (tab->arg & D_FLAG)
		print_usage(NULL);
	tab->arg |= E_FLAG;
}

void		i_fct(t_hash *tab)
{
	tab->arg |= I_FLAG;
}

void		o_fct(t_hash *tab)
{
	if (tab->ops.fd != 1)
		print_usage(NULL);
	tab->arg |= O_FLAG;
}

void		k_fct(t_hash *tab)
{
	if (tab->ops.key)
		print_usage(NULL);
	tab->arg |= K_FLAG;
}

void		v_fct(t_hash *tab)
{
	if (tab->ops.init_vec)
		print_usage(NULL);
	tab->arg |= V_FLAG;
}
