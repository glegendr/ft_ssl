/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 22:11:35 by glegendr          #+#    #+#             */
/*   Updated: 2019/10/09 21:24:06 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>

void		p_fct(t_hash *tab)
{
	if (tab->ops.pwd && IS_DES(tab->f))
		CLEAN_AND_PRINT(tab, NULL);
	tab->arg |= P_FLAG;
}

void		q_fct(t_hash *tab)
{
	if (IS_DES(tab->f))
		CLEAN_AND_PRINT(tab, NULL);
	tab->arg |= Q_FLAG;
}

void		r_fct(t_hash *tab)
{
	if (IS_DES(tab->f))
		CLEAN_AND_PRINT(tab, NULL);
	tab->arg |= R_FLAG;
}

void		s_fct(t_hash *tab)
{
	tab->arg |= S_FLAG;
}

void		d_fct(t_hash *tab)
{
	if (tab->arg & E_FLAG || (!IS_DES(tab->f) && tab->f != base64 && tab->f != base64url))
		CLEAN_AND_PRINT(tab, NULL);
	tab->arg |= D_FLAG;
}
