/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v_split.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/18 22:40:22 by glegendr          #+#    #+#             */
/*   Updated: 2019/05/28 16:51:10 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

t_vec		v_split(t_vec *vec, int index)
{
	t_vec	vec2;

	vec2 = v_new(vec->private_elem_size);
	v_append_raw(&vec2, vec->private_content, index + 1);
	ft_memcpy(vec->private_content, v_get(vec, index + 1),
			(vec->private_elem_nb - (index + 1)) * vec->private_elem_size);
	vec->private_elem_nb -= index + 1;
	return (vec2);
}

void		*v_split_raw(t_vec *vec, int index)
{
	void	*ret;

	ret = (void *)malloc(vec->private_elem_size * (index + 1));
	ft_memcpy(ret, vec->private_content, (index + 1) * vec->private_elem_size);
	ft_memcpy(vec->private_content, v_get(vec, index + 1),
			(vec->private_elem_nb - (index + 1)) * vec->private_elem_size);
	vec->private_elem_nb -= index + 1;
	return (ret);
}
