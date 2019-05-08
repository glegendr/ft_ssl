/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v_del.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/12 22:19:28 by glegendr          #+#    #+#             */
/*   Updated: 2018/03/13 22:38:57 by glegendr         ###   ########.fr       */
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
