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

uint8_t		*des_ecb(t_hash *hash, bool print)
{
	t_ops ops = hash->ops;
	uint8_t *str;
	t_vec *vec;
	uint8_t true_pwd[8];
	uint32_t divided_key[32];
	uint8_t final_keys[16][6];

	(void)print;
	if (!ops.pwd)
		ops.pwd = get_pwd();

	vec = v_get(&hash->str, 0);
	get_true_pwd(ops.pwd, true_pwd);
	pc1(true_pwd);
	rotate_key(true_pwd, divided_key);
	pc2(divided_key, final_keys);
	if (v_size(vec) < 8)
		v_append_raw(vec, "00000000", 8 - v_size(vec));
	str = v_raw(vec);
	hash_des_message(str, final_keys);
	return (NULL);
}

void		create_salt(uint8_t *salt, uint8_t *ops_salt)
{
	int fd;

	if (ops_salt != NULL)
	{
		salt = ops_salt;
		return ;
	}
	fd = open_file("/dev/random", 0, 0);
	read(fd, salt, 8);
}

void		in_u8(uint8_t *cpy, uint8_t *in)
{
	in[0] = cpy[0];
	in[1] = cpy[1];
	in[2] = cpy[2];
	in[3] = cpy[3];
	in[4] = cpy[4];
	in[5] = cpy[5];
	in[6] = cpy[6];
	in[7] = cpy[7];
}

void		create_key(const char *pwd, uint8_t *salt, uint8_t *key, uint8_t *lv)
{
	t_hash concat;
	t_vec concat_vec;
	t_vec ret_vec;
	uint8_t *md5_ret;

	ret_vec = v_new(sizeof(t_vec));
	concat_vec = v_new(sizeof(uint8_t));
	v_append_raw(&concat_vec, (void *)pwd, ft_strlen(pwd));
	v_append_raw(&concat_vec, salt, 8);
	v_push(&ret_vec, &concat_vec);
	concat.arg = 0;
	concat.folder = v_new_null(sizeof(uint8_t));
	concat.str = ret_vec;
	concat.ops.fd = 0;
	md5_ret = md5(&concat, false);
	in_u8(md5_ret, key);
	in_u8(md5_ret + 8, lv);
}

uint8_t		*des(t_hash *tab, bool print)
{
	t_ops ops = tab->ops;
	uint8_t salt[8];
	uint8_t key[8];
	uint8_t lv[8];

	(void)print;
	if (ops.key)
		return (NULL); // LANCH HASH HERE
	if (!ops.pwd)
		ops.pwd = get_pwd();
	create_salt(salt, ops.salt);
	printf("salt=");
	for (int i = 0; i < 8; ++i)
		printf("%0.2x", salt[i]);
	create_key(ops.pwd, salt, key, lv);
	printf("\nkey=");
	for (int i = 0; i < 8; ++i)
		printf("%0.2x", key[i]);
	printf("\n");
	return (NULL);
}
