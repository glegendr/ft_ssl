/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v_print.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/21 16:28:18 by glegendr          #+#    #+#             */
/*   Updated: 2019/06/23 15:49:02 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"
#include <unistd.h>

void		v_print(t_vec *vec)
{
	int i;

	i = 0;
	ft_putchar('{');
	if (v_get_size(vec) == sizeof(char))
		write(1, v_get(vec, 0), vec->private_elem_nb);
	else if (v_get_size(vec) == sizeof(int))
		while (i < v_size(vec))
		{
			ft_putnbr(*(int *)v_get(vec, i));
			++i;
			if (i < v_size(vec))
				ft_putchar(' ');
		}
	else if (v_get_size(vec) == sizeof(t_vec))
		while (i < v_size(vec))
		{
			v_print(v_get(vec, i));
			++i;
		}
	ft_putchar('}');
}
