#include "ft_ssl.h"
#include <libft.h>


#include <ft_printf.h>
#include <stdio.h>

uint8_t *get_pwd(void)
{
	uint8_t *ret;

	ret = (uint8_t *)ft_strdup(getpass("enter des-ecb encryption password"));
	if (ft_strcmp((char *)ret, getpass("Verifying - enter des-ecb encryption password")))
	{
		write(2, "Verify failure\nbad password read\n", 34);
		exit(1);
	}
	return ret;
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

void		create_salt(uint8_t *salt, uint8_t *ops_salt)
{
	int fd;

	if (ops_salt)
	{
		in_u8(ops_salt, salt);
		return ;
	}
	fd = open_file("/dev/random", 0, 0);
	read(fd, salt, 8);
}


void		create_key(uint8_t *pwd, uint8_t *salt, uint8_t *key, uint8_t *iv)
{
	t_hash concat;
	t_vec concat_vec;
	t_vec ret_vec;
	uint8_t *md5_ret;

	ret_vec = v_new(sizeof(t_vec));
	concat_vec = v_new(sizeof(uint8_t));
	v_append_raw(&concat_vec, pwd, ft_strlen((char *)pwd));
	v_append_raw(&concat_vec, salt, 8);
	v_push(&ret_vec, &concat_vec);
	concat.arg = 0;
	concat.folder = v_new_null(sizeof(uint8_t));
	concat.str = ret_vec;
	concat.ops.fd = 0;
	md5_ret = md5(&concat, false);
	in_u8(md5_ret, key);
	if (iv)
		in_u8(md5_ret + 8, iv);
}


uint8_t		*des_ecb(t_hash *hash, bool print)
{
	t_ops ops = hash->ops;
	uint32_t divided_key[32];
	uint8_t final_keys[16][6];
	uint8_t salt[8];
	uint8_t key[8];

	(void)print;
	if (!ops.key)
	{
		if (!ops.pwd)
			ops.pwd = get_pwd();
		create_salt(salt, ops.salt);
		create_key(ops.pwd, salt, key, NULL);
		ops.salt = salt;
	}
	else
		in_u8(ops.key, key);
	pc1(key);
	rotate_key(key, divided_key);
	pc2(divided_key, final_keys);
	hash_des_message(hash, final_keys);
	return (NULL);
}
