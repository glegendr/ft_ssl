/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/28 11:32:09 by glegendr          #+#    #+#             */
/*   Updated: 2019/10/09 21:22:55 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>

void		e_fct(t_hash *tab)
{
	if (tab->arg & E_FLAG || (!IS_DES(tab->f) && tab->f != base64 && tab->f != base64url))
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
	if (tab->ops.key || !IS_DES(tab->f))
		CLEAN_AND_PRINT(tab, NULL);
	tab->arg |= K_FLAG;
}

void		v_fct(t_hash *tab)
{
	if (tab->ops.init_vec || !IS_DES(tab->f))
		CLEAN_AND_PRINT(tab, NULL);
	tab->arg |= V_FLAG;
}
