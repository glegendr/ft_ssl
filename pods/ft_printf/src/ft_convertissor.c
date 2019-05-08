/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aa.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/15 19:08:23 by glegendr          #+#    #+#             */
/*   Updated: 2018/02/28 19:24:24 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "ft_printf.h"

char	*ft_print_pc(t_st *t)
{
	t_vec vec;

	vec = v_new(sizeof(char));
	while (t->size-- > 1)
		v_push_int(&vec, ' ');
	while (t->zero-- > 1 && (t->size >= -1))
		v_push_int(&vec, '0');
	v_push_int(&vec, '%');
	while (++t->size < -1)
		v_push_int(&vec, ' ');
	v_push_int(&vec, '\0');
	char *tmp;
	tmp = ft_strdup((char *)v_raw(&vec));
	v_del(&vec);
	return (tmp);
	return ((char *)v_raw(&vec));
}

char	*ft_pre_is_null(t_st *t, char **s, t_vec *vec)
{
	if (s != NULL && *s != NULL)
		free(*s);
	while (t->zero-- > 0 && t->size == 0)
		v_push(vec, " ");
	while (t->size-- > 0)
		v_push(vec, " ");
	while (++t->size < 0)
		v_push(vec, " ");
	if ((t->mask & PLUS) == PLUS)
		v_push(vec, "+");
	if ((t->mask & SPACE) == SPACE)
		v_push(vec, " ");
	ft_flag_is_sharp(t, 0, vec);
//	v_print(vec);
//	*cmpt += v_size(vec);
//	v_del(vec);
	return ((char *)v_raw(vec));
}

long	ft_puis(long nb, int pui)
{
	long i;

	i = nb;
	if (pui == 0)
		return (1);
	pui--;
	while (pui > 0)
	{
		nb *= i;
		--pui;
	}
	return (nb);
}

char	*bin_to_dec_next(long long ret, int neg, char **s1)
{
	char		*s;
	long long	tmp;
	int			i;

	free(*s1);
	i = 0;
	tmp = ret;
	while ((tmp = (tmp / 10)) > 0)
		++i;
	i += neg;
	s = ft_strnew(i + 1);
	if (neg == 1)
		s[0] = '-';
	while (i >= neg)
	{
		s[i] = (ret % 10) + 48;
		ret /= 10;
		--i;
	}
	return (s);
}

char	*bin_to_dec(char *s)
{
	long long i;
	long long ret;
	long long y;

	if (ft_strcmp(s, "-") == 0)
	{
		free(s);
		return (ft_strdup("-9223372036854775808"));
	}
	y = 0;
	ret = 0;
	i = ft_strlen(s) - 1;
	while (s[i])
	{
		if (s[i] == '1')
		{
			ret += ft_puis(2, y);
		}
		++y;
		--i;
	}
	if (s[0] == '-')
		return (bin_to_dec_next(ret, 1, &s));
	return (bin_to_dec_next(ret, 0, &s));
}
