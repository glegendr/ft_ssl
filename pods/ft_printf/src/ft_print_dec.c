/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_dec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/16 14:54:12 by glegendr          #+#    #+#             */
/*   Updated: 2018/02/28 19:18:13 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "ft_restrictions.h"

void		ft_flag_is_pre(int *size, int string_size, t_vec *vec)
{
	int y;

	y = 0;
	if (*size == -1)
		return ;
	if (string_size < *size)
	{
		while (y++ < *size - string_size)
			v_push(vec, "0");
	}
}

int			ft_flag_is_space(t_st *t, t_vec *vec, void *data)
{
	if ((t->mask & SPACE) == 0 && ((char *)data)[0] != '-'
			&& (t->mask & PLUS) == 0)
		return (0);
	if (((char *)data)[0] == '-')
	{
		v_push(vec, "-");
		t->data = t->data + 1;
		return (-1);
	}
	if ((t->mask & PLUS) == PLUS)
	{
		v_push(vec, "+");
		return (0);
	}
	v_push(vec, " ");
	return (0);
}

void		ft_flag_is_null(t_st *t, int string_size, t_vec *vec)
{
	if (t->precision != -1)
		while (v_size(vec) + string_size < t->zero)
			v_push_first(vec, " ");
	else
		while (v_size(vec) + string_size < t->zero)
			v_push(vec, "0");
}

char		*ft_ini_int(t_st *t)
{
	if ((t->mask & CONV) == 0)
		return (bin_to_dec(ft_itoa_base((int)t->data, 2, 0)));
	else if ((t->mask & J) == J)
		return (bin_to_dec(ft_itoa_base((intmax_t)t->data, 2, 0)));
	else if ((t->mask & Z) == Z)
		return (bin_to_dec(ft_itoa_base((size_t)t->data, 2, 0)));
	else if ((t->mask & LL) == LL)
		return (bin_to_dec(ft_itoa_base((long long int)t->data, 2, 0)));
	else if ((t->mask & L) == L)
		return (bin_to_dec(ft_itoa_base((long int)t->data, 2, 0)));
	else if ((t->mask & HH) == HH)
		return (bin_to_dec(ft_itoa_base((signed char)t->data, 2, 0)));
	else if ((t->mask & H) == H)
		return (bin_to_dec(ft_itoa_base((short int)t->data, 2, 0)));
	return ("Pourquoi tu regarde mon code au juste ?");
}

char		*ft_print_dec(t_st *t)
{
	char	*s;
	t_vec	vec;
	int	r;

	vec = v_new(sizeof(char));
	s = ft_ini_int(t);
	if (s[0] == '0' && t->precision == 0)
		return (ft_pre_is_null(t, &s, &vec));
	t->data = s;
	r = ft_flag_is_space(t, &vec, s);
	ft_flag_is_pre(&t->precision, ft_strlen(s) + r, &vec);
	if (t->size >= 0)
		ft_flag_is_null(t, ft_strlen(s) + r, &vec);
	ft_flag_is_size(t, ft_strlen(s) + r, &vec);
	if (t->size >= 0)
		v_append_raw(&vec, t->data, ft_strlen(s) + r);
	v_push_int(&vec, '\0');
//	v_print(&vec);
//	*cmpt += v_size(&vec);
//	free(s);
//	printf("HELL1\n");
//	char *tmp = NULL;
//	ft_strcpy(tmp, (char *)v_raw(&vec));
//	v_del(&vec);
//	printf("HELL2\n");
//	return (tmp);
	char *tmp;
	tmp = ft_strdup((char *)v_raw(&vec));
	v_del(&vec);
	return (tmp);
	return ((char *)v_raw(&vec));
}
