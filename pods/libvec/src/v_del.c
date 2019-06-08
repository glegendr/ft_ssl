/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v_del.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/12 22:19:28 by glegendr          #+#    #+#             */
/*   Updated: 2019/06/08 13:26:53 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

void		v_del(t_vec *vec)
{
	if (vec != NULL && vec->private_content != NULL)
	{
		free(vec->private_content);
		vec->private_elem_size = 0;
		vec->private_elem_nb = 0;
		vec->private_elem_cap = 0;
		vec->private_content = NULL;
	}
}

void		v_del_all(t_vec *vec)
{
	int i;

	i = 0;
	while (i < v_size(vec))
	{
		v_del((t_vec *)v_get(vec, i));
		++i;
	}
	v_del(vec);
}

void		v_reset(t_vec *vec)
{
	vec->private_elem_nb = 0;
}
