/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/15 17:00:10 by glegendr          #+#    #+#             */
/*   Updated: 2019/05/06 16:08:12 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void		use_string(char **s, t_vec *v, t_st *t)
{
			if (t->string_size == -1)
			{
			}
			else if (ft_strlen(*s) == 0
				 && (t->prin == 'c' || t->prin == 'C'))
				v_push_int(v, '\0');
			else 
				v_append_raw(v, ft_strdup(*s), ft_strlen(*s));
			free(*s);
}

static char		*ft_print(t_vec *vec, char *str, int *cmpt)
{
	int		i;
	int		y;
	t_st		t;
	t_vec		v;
	char		*s;

	y = 0;
	i = 0;
	if (str == NULL)
		return NULL;
	v = v_new(sizeof(char));
	while (str[i])
	{
		if (str[i] != '%')
			v_push_int(&v, (char)str[i]);
		else
		{
			t = *(t_st *)v_get(vec, y++);
			s = match_flags(&t);
			use_string(&s, &v, &t);
		}
		++i;
	}
	*cmpt = v_size(&v);
	v_push_int(&v, '\0');
	return ((char *)v_raw(&v));
}

char			*ft_format(char const *restrict format, ...)
{
	t_vec		vec;
	va_list		v;
	char		*str;
	char		*ret;
	int		cmpt;

	str = NULL;
	if (format[0] == 0)
		return (0);
	va_start(v, format);
	vec = ft_pars(format, &v, &str);
	ret = ft_print(&vec, str, &cmpt);
	free(str);
	v_del(&vec);
	return (ret);
}

int			ft_printf(char const *restrict format, ...)
{
	t_vec		vec;
	va_list		v;
	char		*str;
	char		*ret;
	int 		cmpt;

	str = NULL;
	if (!format[0])
		return (0);
	va_start(v, format);
	vec = ft_pars(format, &v, &str);
	ret = ft_print(&vec, str, &cmpt);
	write(1, ret, cmpt);
	free(ret);
	free(str);
	v_del(&vec);
	return (cmpt);
}
