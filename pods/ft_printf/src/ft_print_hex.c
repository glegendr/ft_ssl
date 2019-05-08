/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/17 20:11:19 by glegendr          #+#    #+#             */
/*   Updated: 2018/01/31 23:12:45 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char		*ft_ini_hex(t_st *t)
{
	if ((t->mask & CONV) == 0)
		return (ft_itoa_base((unsigned int)t->data, 16, 0));
	if ((t->mask & LL) == LL)
		return (ft_uitoa_base((unsigned long long)t->data, 16, 0));
	if ((t->mask & L) == L)
		return (ft_uitoa_base((unsigned long)t->data, 16, 0));
	if ((t->mask & J) == J)
		return (ft_uitoa_base((uintmax_t)t->data, 16, 0));
	if ((t->mask & Z) == Z)
		return (ft_uitoa_base((size_t)t->data, 16, 0));
	if ((t->mask & HH) == HH)
		return (ft_uitoa_base((unsigned char)t->data, 16, 0));
	if ((t->mask & H) == H)
		return (ft_itoa_base((unsigned short)t->data, 16, 0));
	return ("Courez mon seigneur des sarasins !");
}

char		*ft_ini_oct(t_st *t)
{
	if ((t->mask & CONV) == 0)
		return (ft_itoa_base((unsigned int)t->data, 8, 'x'));
	if ((t->mask & LL) == LL)
		return (ft_uitoa_base((unsigned long long)t->data, 8, 'x'));
	if ((t->mask & L) == L)
		return (ft_uitoa_base((unsigned long)t->data, 8, 'x'));
	if ((t->mask & J) == J)
		return (ft_uitoa_base((uintmax_t)t->data, 8, 'x'));
	if ((t->mask & Z) == Z)
		return (ft_uitoa_base((size_t)t->data, 8, 'x'));
	if ((t->mask & HH) == HH)
		return (ft_uitoa_base((unsigned char)t->data, 8, 'x'));
	if ((t->mask & H) == H)
		return (ft_uitoa_base((unsigned short)t->data, 8, 'x'));
	return ("Whalla c'est pas moi !");
}

char		*ft_ini_unsigned(t_st *t)
{
	if ((t->mask & CONV) == 0)
		return (ft_uitoa_base((unsigned int)t->data, 10, 'x'));
	if ((t->mask & LL) == LL)
		return (ft_uitoa_base((unsigned long long)t->data, 10, 'x'));
	if ((t->mask & L) == L)
		return (ft_uitoa_base((unsigned long int)t->data, 10, 'x'));
	if ((t->mask & J) == J)
		return (ft_uitoa_base((uintmax_t)t->data, 10, 'x'));
	if ((t->mask & Z) == Z)
		return (ft_uitoa_base((size_t)t->data, 10, 'x'));
	if ((t->mask & HH) == HH)
		return (ft_uitoa_base((unsigned char)t->data, 10, 'x'));
	if ((t->mask & H) == H)
		return (ft_uitoa_base((unsigned short)t->data, 10, 'x'));
	return ("N'entrez pas commissaire,c'est une vrai boucherie a l'interieur");
}

char		*ft_ini_hex_bis(t_st *t)
{
	if ((t->mask & CONV) == 0)
		return (ft_itoa_base((unsigned int)t->data, 16, 'x'));
	if ((t->mask & LL) == LL)
		return (ft_uitoa_base((unsigned long long)t->data, 16, 'x'));
	if ((t->mask & L) == L)
		return (ft_uitoa_base((unsigned long)t->data, 16, 'x'));
	if ((t->mask & J) == J)
		return (ft_uitoa_base((uintmax_t)t->data, 16, 'x'));
	if ((t->mask & Z) == Z)
		return (ft_uitoa_base((size_t)t->data, 16, 'x'));
	if ((t->mask & HH) == HH)
		return (ft_uitoa_base((unsigned char)t->data, 16, 'x'));
	if ((t->mask & H) == H)
		return (ft_itoa_base((unsigned short)t->data, 16, 'x'));
	return ("Courez mon seigneur des sarasins *bis* !");
}

char		*ft_print_hex(t_st *t)
{
	t_vec	vec;

	vec = v_new(sizeof(char));
	if (t->prin == 'X')
		t->data = ft_ini_hex(t);
	else if (t->prin == 'x')
		t->data = ft_ini_hex_bis(t);
	else if (t->prin == 'o' || t->prin == 'O')
		t->data = ft_ini_oct(t);
	else
		t->data = ft_ini_unsigned(t);
	if (t->precision == 0 && ((char *)t->data)[0] == '0')
		return (ft_pre_is_null(t, NULL, &vec));
	ft_flag_is_sharp(t, ft_strlen(t->data), &vec);
	ft_flag_is_pre(&t->precision, ft_strlen(t->data), &vec);
	ft_flag_is_size(t, ft_strlen(t->data), &vec);
	if (t->size == 0)
		ft_flag_is_null(t, ft_strlen(t->data), &vec);
	if (t->size >= 0)
		v_append_raw(&vec, t->data, ft_strlen(t->data));
	v_push_int(&vec, '\0');
//	v_print(&vec);
//	*cmpt += v_size(&vec);
//	free(t->data);
//	v_del(&vec);
/*	char *tmp = NULL;
	ft_strcpy(tmp, (char *)v_raw(&vec));
	v_del(&vec);
	return (tmp);*/
	char *tmp;
	tmp = ft_strdup((char *)v_raw(&vec));
	v_del(&vec);
	return (tmp);
	return ((char *)v_raw(&vec));
}
