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
	hash_des_message(str, divided_key);
}

void		des(t_hash *tab)
{
	t_ops ops = tab->ops;
	int x;

	for (int i = 0; i < 8; ++i)
	{
		x = ((i * 2) << 4) | ((i * 2) + 1);
		write(ops.fd, &x, 1);
	}
}
