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

/*
** 57 - 1   8
** 49 - 2   8
** 41 - 3   8
** 33 - 4   8
** 25 - 5   8
** 17 - 6   8
** 9 - 7    8
** 1 - 8    END
**
** 58 - 9   8
** 50 - 10  8
** 42 - 11  8
** 34 - 12  8
** 26 - 13  8
** 18 - 14  8
** 10 - 15  8
** 2  - 16  8
**
** 59 - 17  8
** 51 - 18  8
** 43 - 19  8
** 35 - 20  8
** 27 - 21  8
** 19 - 22  8
** 11 - 23  8
** 3  - 24
**
** 60 - 25  8
** 52 - 26  8
** 44 - 27  8
** 36 - 28
**
** 63 - 29  8
** 55 - 30  8
** 47 - 31  8
** 39 - 32  8
** 31 - 33  8
** 23 - 34  8
** 15 - 35  8
** 7  - 36
**
** 62 - 37  8
** 54 - 38  8
** 46 - 39  8
** 38 - 40  8
** 30 - 41  8
** 22 - 42  8
** 14 - 43  8
** 6  - 44  8
**
** 61 - 45  8
** 53 - 46  8
** 45 - 47  8
** 37 - 48  8
** 29 - 49  8
** 21 - 50  8
** 13 - 51  8
** 5  - 52
**
** 28 - 53  8
** 20 - 54  8
** 12 - 55  8
** 4  - 56  8
**
*/


void		bit(uint8_t *ret, int i, uint8_t *str, int pos)
{
	uint8_t x;
	int bit;

	x = pos - pos / 8 * 8;
	x = 1 << (8 - x);
	bit = (str[pos / 8] & x) >> (8 - (pos - pos / 8 * 8));
	ret[i / 8] = ret[i / 8] << 1;
	ret[i / 8] = ret[i / 8] | bit;
}

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

void		des_ecb(t_hash *hash)
{
	t_ops ops = hash->ops;
	uint8_t *str;
	t_vec *vec;
	uint8_t true_pwd[8];

	if (!ops.pwd)
		ops.pwd = get_pwd();

	printf("\n");
	vec = v_get(&hash->str, 0);
	str = v_raw(vec);
	get_true_pwd(ops.pwd, true_pwd);
	pc1(str);
	printf("%x %x %x %x %x %x %x\n", str[0], str[1], str[2], str[3], str[4], str[5], str[6]);
//	read_file(&tab, 0, false);
//	into_vec(&tab.folder, NULL);
}

void		des(t_hash *tab)
{
	(void)tab;
}
