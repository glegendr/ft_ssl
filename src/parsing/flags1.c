/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/27 22:11:35 by glegendr          #+#    #+#             */
/*   Updated: 2019/08/27 22:29:09 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ssl.h>

void		p_fct(t_hash *tab)
{
	if (tab->ops.pwd)
		print_usage(NULL);
	tab->arg |= P_FLAG;
}

void		q_fct(t_hash *tab)
{
	tab->arg |= Q_FLAG;
}

void		r_fct(t_hash *tab)
{
	tab->arg |= R_FLAG;
}

void		s_fct(t_hash *tab)
{
	tab->arg |= S_FLAG;
}

void		d_fct(t_hash *tab)
{
	if (tab->arg & E_FLAG)
		print_usage(NULL);
	tab->arg |= D_FLAG;
}

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

void		a_fct(t_hash *tab)
{
	tab->arg |= A_FLAG;
}
