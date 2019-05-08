/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rawtoi.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/11 21:27:06 by glegendr          #+#    #+#             */
/*   Updated: 2018/01/31 23:13:53 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int				ft_conv(char c)
{
	if (c == 's' || c == 'S' || c == 'p' || c == 'd' || c == 'D' ||
			c == 'i' || c == 'o' || c == 'O' || c == 'u' ||
			c == 'U' || c == 'x' || c == 'X' || c == 'c' ||
			c == 'C' || c == '%' || c == 'b')
		return (1);
	return (0);
}

int				ft_flags(char c)
{
	if (c == 'h' || c == 'l' || c == 'j' || c == 'z' || c == '.' ||
			c == '+' || c == '-' || c == ' ' || c == '#' ||
			(c >= '0' && c <= '9'))
		return (1);
	return (0);
}

int				ft_rawtoi(char const *restrict c,
		int *i, t_st *t, int *cmpt)
{
	int		y;

	if (c[*i] == '#' && ft_isdigit(c[*i + 1]))
	{
		*cmpt += 1;
		*i += 1;
		t->mask |= SHARP;
	}
	y = ft_atoi(&c[*i]);
	while (ft_isdigit(c[*i]))
	{
		*i += 1;
		*cmpt += 1;
	}
	*i -= 1;
	return (y);
}

int				ft_no_flag_founded(char c, t_st *t, int cmpt, va_list *v)
{
	if (c != '%' && ft_conv(c))
		t->data = va_arg(*v, char *);
	else if (c == '%')
		t->mask |= PC;
	else if (c != '\0')
	{
		t->prin = c;
		return (cmpt);
	}
	else
	{
		return (cmpt);
	}
	t->prin = c;
	return (cmpt);
}
