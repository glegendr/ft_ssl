/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v_get.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/12 22:03:59 by glegendr          #+#    #+#             */
/*   Updated: 2019/06/25 13:29:03 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

void		*v_get(t_vec *vec, int index)
{
	void *pos;

	pos = vec->private_content + index * vec->private_elem_size;
	return (pos);
}
