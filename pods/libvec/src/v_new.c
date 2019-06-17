/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v_new.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/12 21:04:30 by glegendr          #+#    #+#             */
/*   Updated: 2019/06/17 11:24:06 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

t_vec		v_new(int elem_size)
{
	t_vec vec;

	vec.private_content = malloc(elem_size * NB_ELEM_INI);
	vec.private_elem_nb = 0;
	vec.private_elem_size = elem_size;
	vec.private_elem_cap = NB_ELEM_INI;
	return (vec);
}

t_vec		v_new_null(int elem_size)
{
	t_vec vec;

	vec.private_content = NULL;
	vec.private_elem_nb = 0;
	vec.private_elem_size = elem_size;
	vec.private_elem_cap = 0;
	return (vec);
}
