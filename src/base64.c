#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <ft_printf.h>
#include <libft.h>
#include "ft_ssl.h"

void		generate_base(char *base, bool url)
{
	int y;

	y = 0;
	for (int i = 0; 'A' + i <= 'Z'; ++i, ++y)
		base[y] = 'A' + i;
	for (int i = 0 ; 'a' + i <= 'z'; ++i, ++y)
		base[y] = 'a' + i;
	for (int i = 0; '0' + i <= '9'; ++i, ++y)
		base[y] = '0' + i;
	if (url) {
		base[y] = '-';
		base[y + 1] = '_';
	} else {
		base[y] = '+';
		base[y + 1] = '/';
	}
	base[y + 2] = '=';
}

void		launch_base(uint8_t *raw, char *base, int len, t_vec *print)
{
	uint8_t	ret[4];

	ret[0] = (raw[0] & 0xfc) >> 2;
	ret[1] = (((raw[0] & 0x3) << 6) | ((raw[1] & 0xf0) >> 2)) >> 2;
	if (len > 2) {
		ret[2] = (((raw[1] & 0xf) << 4) | ((raw[2] & 0xc0) >> 4)) >> 2;
		ret[3] = (raw[2] & 0x3f);
	} else {
		ret[2] = len == 2 ? (((raw[1] & 0xf) << 4) | 0) >> 2 : 64;
		ret[3] = 64;
	}
	v_push_int(print, base[ret[0]]);
	v_push_int(print, base[ret[1]]);
	v_push_int(print, base[ret[2]]);
	v_push_int(print, base[ret[3]]);
}

// ECHO AVANT
// Enlever 0 dans le hash
// Whirlpool

int			decript_base(uint8_t *raw, char *base, int len, int *z, t_vec *print)
{
	uint8_t ret[3] = {0};
	uint8_t index[4] = {255, 255, 255, 255};
	size_t i;

	for (i = 0; i < 4; ++i) {
		while (ft_isspace(raw[i]) && *z < len)
		{
			++raw;
			++(*z);
		}
		if (*z >= len)
			break;
		for (size_t y = 0; y < 65; ++y) {
			if (raw[i] == base[y])
			{
				index[i] = y;
				break ;
			}
		}
		if (index[i] == 255 && raw[i] != '\n' && raw[i] != '\0')
		{
			v_reset(print);
			v_append_raw(print, "Invalid character in input stream.\n", 35);
			return (1);
		}
	}
	if (i > 0)
	ret[0] = (index[0] << 2) | ((index[1] & 0x30) >> 4);
	if (i > 1)
	ret[1] = ((index[1] & 0xf) << 4) | ((index[2] & 0x3c) >> 2);
	if (i > 2)
	ret[2] = ((index[2] & 0x3) << 6) | index[3];
	if (ret[2] == '@')
		ret[2] = '\0';
	v_append_raw(print, ret, 3);
	return (0);
}

void		bases(t_hash *tab, char *base)
{
	int i;
	t_vec print;

	print = v_new(sizeof(char));
	i = 0;
	while (i < v_size(&tab->str)) {
		t_vec *vec = v_get(&tab->str, i);
		if (i > 0)
		v_push_int(&print, '\n');
		uint8_t *tmp = (uint8_t *)v_raw(vec);
		for (int z = 0; z < v_size(vec); z += 3)
			if (tab->arg & D_FLAG)
			{
				if (decript_base(tmp + z, base, v_size(vec), &z, &print))
					break ;
				++z;
			}
			else
				launch_base(tmp + z, base, v_size(vec) - z, &print);
		write(tab->ops.fd, v_raw(&print), v_size(&print));
		v_reset(&print);
		++i;
	}
	v_del(&print);
}

void		base64(t_hash *tab)
{
	char base[65];

	generate_base(base, false);
	bases(tab, base);
}

void		base64url(t_hash *tab)
{
	char base[65];

	generate_base(base, true);
	bases(tab, base);
}
