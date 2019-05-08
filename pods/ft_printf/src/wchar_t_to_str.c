/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wchar_t_to_str.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/25 21:23:57 by glegendr          #+#    #+#             */
/*   Updated: 2018/03/01 02:16:06 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static char		*wchar32_to_str(wchar_t c)
{
	char *s;

	s = ft_strnew(4);
	s[0] = ((c >> 18) | 0xf0);
	s[1] = (((c >> 12) & 0x3f) | 0x80);
	s[2] = (((c >> 6) & 0x3f) | 0x80);
	s[3] = ((c & 0x3f) | 0x80);
	return (s);
}

static char		*wchar24_to_str(wchar_t c)
{
	char *s;

	s = ft_strnew(3);
	s[0] = ((c >> 12) | 0xe0);
	s[1] = (((c >> 6) & 0x3f) | 0x80);
	s[2] = ((c & 0x3f) | 0x80);
	return (s);
}

static char		*wchar16_to_str(wchar_t c)
{
	char *s;

	s = ft_strnew(2);
	s[0] = ((c >> 6) | 0xc0);
	s[1] = ((c & 0x3f) | 0x80);
	return (s);
}

static char		*wchar8_to_str(wchar_t c)
{
	char *s;

	s = ft_strnew(1);
	s[0] = c;
	return (s);
}

char			*wchar_t_to_str(wchar_t c, int i, int *err)
{
	char *s;

	if (i == 1)
	{
		s = ft_strnew(1);
		s[0] = (char)c;
		return (s);
	}
	if (c < 0 || (c >= 0xD800 && c <= 0xDFFF))
	{
		*err = -1;
		return (NULL);
	}
	if (c <= 0x007f)
		return (wchar8_to_str(c));
	else if (c <= 0x07ff)
		return (wchar16_to_str(c));
	else if (c <= 0xffff)
		return (wchar24_to_str(c));
	else if (c <= 0x10ffff)
		return (wchar32_to_str(c));
	else
		return (NULL);
}
