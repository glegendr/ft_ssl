#include "ft_ssl.h"
#include <libft.h>


#include <ft_printf.h>
#include <stdio.h>

const char *get_pwd(void)
{
	const char *ret;

	ret = ft_strdup(getpass("enter des-ecb encryption password"));
	if (ft_strcmp(ret, getpass("Verifying - enter des-ecb encryption password")))
	{
		write(2, "Verify failure\nbad password read\n", 34);
		exit(1);
	}
	return ret;
}

void		get_true_pwd(const char *pwd, uint8_t *ret)
{
	int i;
	uint8_t in;

	i = 0;
	while (i < 16)
	{
		in = pwd[i];
		if (in >= 'a' && in <= 'f')
			in -= 97 - 10;
		else if (in >= 'A' && in <= 'F')
			in -= 65 - 10;
		else if (in >= '0' && in <= '9')
			in -= 48;
		ret[i / 2] = ret[i / 2] << 4;
		ret[i / 2] = ret[i / 2] | in;
		++i;
	}
}

void		bit(uint8_t *ret, int i, uint8_t *str, int pos)
{
	uint8_t x;
	int posit;
	int bit;
	int in_ret;

	posit = pos - pos / 8 * 8;
	in_ret = ((posit == 0 ? pos - 1: pos) / 8);
	if (posit == 0)
		posit = 8;
	x = 1 << (8 - posit);
	bit = (str[in_ret] & x) >> (8 - posit);
//	ft_printf("%i -- %i -- %i -- %i -- %0.8b\n", i, pos, x, bit, str[in_ret]);
	ret[i / 8] = ret[i/8] << 1;
	ret[i / 8] = ret[i/8] | bit;
}
/*
void		bit(uint8_t *ret, int i, uint8_t *str, int pos)
{
	uint8_t x;
	int posit;
	int bit;

	posit = pos - pos / 8 * 8;
	if (posit == 0)
		posit = 8;
	x = 1 << (8 - posit);
	bit = (str[pos / 8] & x) >> (8 - posit);
	ft_printf("%i -- %i -- %i -- %i -- %b\n", i, pos, x, bit, str[pos / 8]);
	ret[i / 8] = ret[i / 8] << 1;
	ret[i / 8] = ret[i / 8] | bit;
}*/

void		in_str(uint8_t *str, uint8_t *ret)
{
	str[0] = ret[0];
	str[1] = ret[1];
	str[2] = ret[2];
	str[3] = ret[3];
	str[4] = ret[4];
	str[5] = ret[5];
	str[6] = ret[6];
}

void		pc1(uint8_t *str)
{
	uint8_t ret[7];
	int x;
	int loop;

	loop = 0;
	for (int i = 0; i < 56; ++i)
	{
		if (i == 28)
			loop = 8;
		else if (i == 52)
			loop = -27;
		if (i < 28)
		{
			if (i % 8 == 0)
				++loop;
			x = 56 + loop - (i % 8) * 8;
		}
		else
		{
			if ((i + 4) % 8 == 0)
				--loop;
			x = 56 + loop - ((i + 4) % 8) * 8;
		}
		bit(ret, i, str, x);
	}
	in_str(str, ret);
}

int const g_pc2[48] = {
		14, 17, 11, 24, 1, 5,
		3, 28, 15, 6, 21, 10,
		23, 19, 12, 4, 26, 8,
		16, 7,  27, 20, 13, 2,
		41, 52, 31, 37, 47, 55,
		30, 40, 51, 45, 33, 48,
		44, 49, 39, 56, 34, 53,
		46, 42, 50, 36, 29, 32 };

void		def_in(uint8_t *in, uint32_t cn, uint32_t dn)
{
	in[0] = (cn & 0xff00000) >> 20;
	in[1] = (cn & 0xff000) >> 12;
	in[2] = (cn & 0xff0) >> 4;
	in[3] = ((cn & 0xf) << 4) | ((dn & 0xf000000) >> 24);
	in[4] = (dn & 0xff0000) >> 16;
	in[5] = (dn & 0xff00) >> 8;
	in[6] = dn & 0xff;
}

void		pc2(uint32_t *str)
{
	int i;
	int y;
	uint8_t ret[16][6];
	uint8_t in[7];

	y = 0;
	while (y < 16)
	{
		def_in(in, str[y], str[y + 16]);
		i = 0;
		while (i < 48)
		{
			bit(ret[y], i, in, g_pc2[i]);
			++i;
		}
		++y;
	}
}


uint32_t		u8_to_b28(uint8_t *val, bool first)
{
	int i;
	uint32_t bytes;
	uint8_t *str;

	i = 0;
	str = val;
	bytes = 0;
	if (!first)
	{
		bytes = bytes | (str[0] & 0xf);
		bytes = bytes << 8;
		str += 1;
	}
	while (i < 3)
	{
		if (!(!first && i == 0))
			bytes = bytes << 8;
		bytes = bytes | str[i];
		++i;
	}
	if (first)
	{
		bytes = bytes << 4;
		bytes = bytes | ((str[i] & 0xf0) >> 4);
	}
	return (bytes);
}

void		rotate_key(uint8_t *pwd, uint32_t *div_key)
{
	uint32_t c0;
	uint32_t d0;
	int x;

	c0 = u8_to_b28(pwd, true);
	d0 = u8_to_b28(pwd + 3, false);

	x = 1;
	for (int i = 0; i < 16; ++i)
	{
		if (i == 2 || i == 9)
			x = 2;
		else if (i == 8 || i == 15)
			x = 1;
		div_key[i] = (ROTL(i == 0 ? c0 : div_key[i - 1], x, 28) & 0xfffffff);
	}
	for (int i = 0; i < 16; ++i)
	{
		if (i == 2 || i == 9)
			x = 2;
		else if (i == 8 || i == 15)
			x = 1;
		div_key[i + 16] = (ROTL(i == 0 ? d0 : div_key[i + 15], x, 28)
							& 0xfffffff);
	}
}

void		des_ecb(t_hash *hash)
{
	t_ops ops = hash->ops;
	uint8_t *str;
	t_vec *vec;
	uint8_t true_pwd[8];
	uint32_t divided_key[32];

	if (!ops.pwd)
		ops.pwd = get_pwd();

	printf("\n");
	vec = v_get(&hash->str, 0);
	str = v_raw(vec);
	get_true_pwd(ops.pwd, true_pwd);
	pc1(true_pwd);
	rotate_key(true_pwd, divided_key);
	pc2(divided_key);
}

void		des(t_hash *tab)
{
	(void)tab;
}
