#include "ft_ssl.h"
#include <libft.h>



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

void		des_ecb(t_hash *hash)
{
	t_ops ops = hash->ops;
	uint8_t *str;
	t_vec *vec;

	if (!ops.pwd)
		ops.pwd = get_pwd();
	write(1, ops.pwd, ft_strlen(ops.pwd));

	vec = v_get(&hash->str, 0);
	write(1, v_raw(vec), v_size(vec));
	str = v_raw(vec);
	printf("HELL1\n");
	str[0] ^= ops.pwd[0];
	printf("HELL2\n");
	str[1] ^= ops.pwd[1];
	printf("HELL3\n");
	str[2] ^= ops.pwd[2];
	printf("HELL4\n");
	str[3] ^= ops.pwd[3];
	printf("HELL5\n");
	str[4] ^= ops.pwd[4];
	printf("HELL6\n");
	str[5] ^= ops.pwd[5];
	printf("HELL7\n");
	str[6] ^= ops.pwd[6];
	printf("HELL8\n");
	str[7] ^= ops.pwd[7];
	printf("HELL9\n");
	write(1, str, 8);

//	read_file(&tab, 0, false);
//	into_vec(&tab.folder, NULL);
}

void		des(t_hash *tab)
{
	(void)tab;
}
