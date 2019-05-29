/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v_append_raw.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/18 22:32:59 by glegendr          #+#    #+#             */
/*   Updated: 2019/05/29 16:52:33 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

void		v_append_raw(t_vec *vec, void *elem, int nb)
{
	if (vec->private_elem_cap <=
			(vec->private_elem_nb + vec->private_elem_size * nb))
	{
		if ((vec->private_content = ft_realloc(vec->private_content,
			vec->private_elem_cap * vec->private_elem_size,
			nb + vec->private_elem_cap > 2 * vec->private_elem_cap ?
				((nb + vec->private_elem_cap)
				* vec->private_elem_size)
				:
				((2 * vec->private_elem_cap)
				* vec->private_elem_size))) == NULL)
			return ;
		if (nb + vec->private_elem_cap > 2 * vec->private_elem_cap)
			vec->private_elem_cap += nb;
		else
			vec->private_elem_cap *= 2;
	}
	ft_memcpy(v_get(vec, vec->private_elem_nb),
			elem, vec->private_elem_size * nb);
	vec->private_elem_nb += nb;
}
