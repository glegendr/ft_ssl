/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/16 18:27:36 by glegendr          #+#    #+#             */
/*   Updated: 2019/05/16 21:12:56 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <ft_printf.h>
#include <libft.h>
#include "ft_ssl.h"
#include "md5.h"

#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))

void		push_original_len(uint8_t **str, int len, uint64_t original_len)
{
	(*str)[len + 7] = ((original_len >> 56) & 0xff);
	(*str)[len + 6] = ((original_len >> 48) & 0xff);
	(*str)[len + 5] = ((original_len >> 40) & 0xff);
	(*str)[len + 4] = ((original_len >> 32) & 0xff);
	(*str)[len + 3] = ((original_len >> 24) & 0xff);
	(*str)[len + 2] = ((original_len >> 16) & 0xff);
	(*str)[len + 1] = ((original_len >> 8) & 0xff);
	(*str)[len + 0] = ((original_len & 0xff));
}

int			pad_message(uint8_t **tab)
{
	uint8_t	*str;
	int		len;
	int		original_len;

	if (*tab == NULL)
		original_len = 0;
	else
		original_len = ft_strlen((char *)(*tab));
	len = original_len + 1;
	while (len % 64)
		++len;
	if (original_len + 8 >= len)
		len += 64;
	*tab = ft_realloc(*tab, original_len, len);
	str = *tab;
	len -= 8;
	push_original_len(&str, len, (uint64_t)original_len * 8);
	str[original_len] = (char)128;
	while (original_len < len - 1)
	{
		++original_len;
		str[original_len] = (char)0;
	}
	*tab = str;
	return (len + 8);
}

uint32_t	declare_chunk(uint8_t *ck, int i)
{
	int32_t	val;

	val = (ck[i + 3] << 24) + (ck[i + 2] << 16) + (ck[i + 1] << 8) + ck[i + 0];
	return (val);
}

void		lol2(int32_t *tab[4], int32_t m[16])
{
	int32_t		f;
	int32_t		g;
	uint32_t	temp;

	for (int i = 0; i < 64; i++) {
		if (i < 16) {
			f = (*(tab[1]) & *(tab[2])) | ((~*(tab[1])) & *(tab[3]));
			g = i;
		} else if (i < 32) {
			f = (*(tab[3]) & *(tab[1])) | ((~*(tab[3])) & *(tab[2]));
			g = (5 * i + 1) % 16;
		} else if (i < 48) {
			f = *(tab[1]) ^ *(tab[2]) ^ *(tab[3]);
			g = (3 * i + 5) % 16;
		} else {
			f = *(tab[2]) ^ (*(tab[1]) | (~*(tab[3])));
			g = (7 * i) % 16;
		}
		temp = *(tab[3]);
		*(tab[3]) = *(tab[2]);
		*(tab[2]) = *(tab[1]);
		*(tab[1]) = *(tab[1]) +
					LEFTROTATE((*(tab[0]) + f + g_k[i] + m[g]), g_s[i]);
		*(tab[0]) = temp;
	}
}

uint8_t		*digest(int32_t a0, int32_t b0, int32_t c0, int32_t d0)
{
	uint8_t	*digest;

	digest = malloc(sizeof(uint8_t) * 16);
	digest[0] = (a0 & 0xff);
	digest[1] = ((a0 >> 8) & 0xff);
	digest[2] = ((a0 >> 16) & 0xff);
	digest[3] = ((a0 >> 24) & 0xff);
	digest[4] = (b0 & 0xff);
	digest[5] = ((b0 >> 8) & 0xff);
	digest[6] = ((b0 >> 16) & 0xff);
	digest[7] = ((b0 >> 24) & 0xff);
	digest[8] = (c0 & 0xff);
	digest[9] = ((c0 >> 8) & 0xff);
	digest[10] = ((c0 >> 16) & 0xff);
	digest[11] = ((c0 >> 24) & 0xff);
	digest[12] = (d0 & 0xff);
	digest[13] = ((d0 >> 8) & 0xff);
	digest[14] = ((d0 >> 16) & 0xff);
	digest[15] = ((d0 >> 24) & 0xff);
	return (digest);
}

uint8_t		*lol(uint8_t *ck, int loop)
{
	int32_t	a0 = 0x67452301;
	int32_t	b0 = 0xefcdab89;
	int32_t	c0 = 0x98badcfe;
	int32_t	d0 = 0x10325476;
	int32_t	m[16];

	for (int y = 0; y < loop; y++) {
		for (int i = 0; i < 16; i++)
			m[i] = declare_chunk(ck + 64 * y, i * 4);
		int32_t	a = a0;
		int32_t	b = b0;
		int32_t	c = c0;
		int32_t	d = d0;

		int32_t	*tab[4] = {&a, &b, &c, &d};
		lol2(tab, m);
		a0 = a0 + a;
		b0 = b0 + b;
		c0 = c0 + c;
		d0 = d0 + d;
	}
	return (digest(a0, b0, c0, d0));
}

void		md5(t_hash *tab)
{
	uint8_t	*ret;
	int		i;
	int		tmp;

	i = 0;
	while (tab->folder[i])
	{
		tmp = pad_message(&(tab->str[i]));
		ret = lol(tab->str[i], tmp / 64);
		if ((tab->arg & P_FLAG) && !(tab->arg & Q_FLAG))
			printf("%s\n", tab->str[i]);
		if (!(tab->arg & R_FLAG) && !(tab->arg & Q_FLAG))
			printf("MD5(%s)= ", tab->folder[i]);
		for (int i = 0; i < 16; i++)
			printf("%0.2x", ret[i]);
		if ((tab->arg & R_FLAG) && !(tab->arg & Q_FLAG))
			printf(" %s", tab->folder[i]);
		printf("\n");
		free(ret);
		++i;
	}
}