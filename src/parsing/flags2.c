/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/28 11:32:09 by glegendr          #+#    #+#             */
/*   Updated: 2019/08/29 13:42:20 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>

void		e_fct(t_hash *tab)
{
	if (tab->arg & D_FLAG)
		CLEAN_AND_PRINT(tab, NULL);
	tab->arg |= E_FLAG;
}

void		i_fct(t_hash *tab)
{
	tab->arg |= I_FLAG;
}

void		o_fct(t_hash *tab)
{
	if (tab->ops.fd != 1)
		CLEAN_AND_PRINT(tab, NULL);
	tab->arg |= O_FLAG;
}

void		k_fct(t_hash *tab)
{
	if (tab->ops.key)
		CLEAN_AND_PRINT(tab, NULL);
	tab->arg |= K_FLAG;
}

void		v_fct(t_hash *tab)
{
	if (tab->ops.init_vec)
		CLEAN_AND_PRINT(tab, NULL);
	tab->arg |= V_FLAG;
}
