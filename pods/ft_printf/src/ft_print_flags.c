/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_flags.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/15 18:55:09 by glegendr          #+#    #+#             */
/*   Updated: 2019/05/08 19:53:30 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "ft_restrictions.h"

static char		*ft_print_s(t_st *t)
{
	t_vec	vec;

	vec = v_new(sizeof(char));
	if (t->data == NULL)
		return (ft_str_is_null(t, &vec));
	t->string_size = ft_strlen(t->data);
	if (t->precision != -1 && t->string_size > t->precision)
	{
		ft_memcut(t->data, t->precision);
		t->string_size = t->precision;
	}
	v_append_raw(&vec, t->data, t->string_size);
	v_push_int(&vec, '\0');
	ft_s_flags(t, &vec);
//	v_print(&vec, 1);
//	*cmpt += v_size(&vec);
//	v_del(&vec);
	char *tmp;
	tmp = ft_strdup((char *)v_raw(&vec));
	v_del(&vec);
	return (tmp);
	return ((char *)v_raw(&vec));
}

static char		*ft_print_ls(wchar_t *s, int precision, t_st *t)
{
	t_vec	vec;
	char	*s1;
	int		i;

	vec = v_new(sizeof(char));
	if (s == NULL)
		return (ft_str_is_null(t, &vec));
	i = 0;
	while (s[i] && precision != 0)
	{
		if ((s1 = wchar_t_to_str(s[i++], 0, &t->string_size)) == NULL)
			continue ;
		if (precision > 0 && (int)(v_size(&vec) + ft_strlen(s1)) > precision)
			break ;
		v_append_raw(&vec, s1, ft_strlen(s1));
		free(s1);
	}
	v_push_int(&vec, '\0');
	ft_s_flags(t, &vec);
//	v_print(&vec, 1);
//	i = v_size(&vec);
//	v_del(&vec);
//	return (i);
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

static char		*ft_putwchar(wchar_t c, t_st *t, int i)
{
	t_vec	vec;
	char	*s;

	if ((s = wchar_t_to_str(c, i, &t->string_size)) == NULL)
		return(NULL);
	vec = v_new(sizeof(char));
	if (s[0] == 0)
	{
		while (t->size > v_size(&vec) + 1 && t->size > 0)
			v_push_first(&vec, " ");
		while (t->size == 0 && t->zero > v_size(&vec) + 1)
			v_push_first(&vec, "0");
		v_push(&vec, "\0");
	}
	v_append_raw(&vec, s, ft_strlen(s));
	while (t->size > v_size(&vec) && t->size > 0)
		v_push_first(&vec, " ");
	while (t->size < -v_size(&vec) && t->size < 0)
		v_push(&vec, " ");
	while (t->size == 0 && t->zero > v_size(&vec))
		v_push_first(&vec, "0");
	v_push_int(&vec, '\0');
	free(s);
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

static char		*ft_print_bin(t_st *t)
{
	char	*s;
	t_vec	vec;

	vec = v_new(sizeof(char));
	s = ft_uitoa_base((unsigned long long int)t->data, 2, 0);
	t->data = s;
	ft_flag_is_sharp(t, ft_strlen(s), &vec);
	ft_flag_is_pre(&t->precision, ft_strlen(s), &vec);
	ft_flag_is_size(t, ft_strlen(s), &vec);
	if (t->size == 0)
		ft_flag_is_null(t, ft_strlen(s), &vec);
	if (t->size >= 0)
		v_append_raw(&vec, t->data, ft_strlen(s));
//	*cmpt += v_size(&vec);
//	v_print(&vec, 1);
//	v_del(&vec);
	free(s);
/*	char *tmp = NULL;
	ft_strcpy(tmp, (char *)v_raw(&vec));
	v_del(&vec);
	return (tmp);*/
	v_push_int(&vec, '\0');
	char *tmp;
	tmp = ft_strdup((char *)v_raw(&vec));
	v_del(&vec);
	return (tmp);
	return ((char *)v_raw(&vec));
}

char			*match_flags(t_st *t)
{
	if ((t->mask & PC) == PC)
		return ft_print_pc(t);
	else if (!ft_conv(t->prin))
		return ft_putwchar((char)t->prin, t, 0);
	else if (t->prin == 's' && (t->mask & L) == 0)
		return ft_print_s(t);
	else if (t->prin == 's' || t->prin == 'S')
		return ft_print_ls(t->data, t->precision, t);
	else if (t->prin == 'p')
		return ft_push_pointeur(t->data, t);
	else if (t->prin == 'c' || t->prin == 'C')
		if ((t->mask & L) == L)
			return ft_putwchar((wchar_t)t->data, t, 0);
		else
			return ft_putwchar((char)t->data, t, 1);
	else if (t->prin == 'i' || t->prin == 'd' || t->prin == 'D')
		return ft_print_dec(t);
	else if (t->prin == 'x' || t->prin == 'X' || t->prin == 'O' ||
			t->prin == 'o' || t->prin == 'u' || t->prin == 'U')
		return ft_print_hex(t);
	else if (t->prin == 'b')
		return ft_print_bin(t);
	return (NULL);
}
