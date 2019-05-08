/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_flags.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/26 20:31:04 by glegendr          #+#    #+#             */
/*   Updated: 2018/02/28 19:30:41 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		ft_long_conv(char c)
{
	if (c == 'U' || c == 'D' || c == 'S' || c == 'C' || c == 'O')
		return (1);
	return (0);
}

void	ft_conv_is_zj(t_st *t, int *cmpt, char c)
{
	if (c == 'z')
	{
		*cmpt += 1;
		t->mask |= Z;
	}
	else if (c == 'j')
	{
		*cmpt += 1;
		t->mask |= J;
	}
}

void	ft_ini_struct(t_st *t)
{
	t->precision = -1;
	t->zero = 0;
	t->size = 0;
	t->mask = 0;
	t->string_size = 0;
}

void	ft_flag_is_size(t_st *t, int string_size, t_vec *vec)
{
	if (t->size > 0 && v_size(vec) + string_size < t->size)
	{
		while (v_size(vec) + string_size < t->size)
			v_push_first(vec, " ");
	}
	else if (t->size < 0)
	{
		v_append_raw(vec, t->data, string_size);
		while (v_size(vec) < -t->size)
			v_push(vec, " ");
	}
}

void	ft_flag_is_sharp(t_st *t, int string_size, t_vec *vec)
{
	if ((t->mask & SHARP) == 0 || t->prin == 'u' ||
			(((char *)t->data)[0] == '0' && string_size != 0) ||
			(((char *)t->data)[0] == '0' && (t->prin == 'x' || t->prin == 'X')))
		return ;
	if (t->prin == 'x')
		v_append_raw(vec, "0x", 2);
	else if (t->prin == 'X')
		v_append_raw(vec, "0X", 2);
	else if (t->prin == 'b')
		v_append_raw(vec, "0b", 2);
	else if (((t->prin == 'o' || t->prin == 'O') &&
				t->precision <= string_size) ||
			((t->prin == 'o' || t->prin == 'O') && ((char *)t->data)[0] == '0'))
		v_push(vec, "0");
}
