#include "ft_ssl.h"
#include <libft.h>
#include <ft_printf.h>

void	print_hash_tmp(uint8_t *ret, t_hash *hash, int i, t_ops ops)
{
	if ((hash->arg & P_FLAG) && !(hash->arg & Q_FLAG))
		printf("%s\n", v_raw(v_get(&hash->str, i)));
	if (!(hash->arg & R_FLAG) && !(hash->arg & Q_FLAG))
		printf("%s(%s)= ", ops.name, hash->folder[i]);
	for (int i = 0; i < ops.message_len; i++)
		printf("%0.2x", ret[i]);
	if ((hash->arg & R_FLAG) && !(hash->arg & Q_FLAG))
		printf(" %s", hash->folder[i]);
	printf("\n");
}

static void	GOO(t_vec *ck, t_ops ops, uint8_t *ret, int loop)
{
	uint64_t m[80];
	uint64_t h[8];
	uint64_t tmp[8];

	ops.init_h(h);
	for (int y = 0; y < loop; ++y) {
		ops.declare_chunk(ck, y, m);
		tmp[0] = h[0];
		tmp[1] = h[1];
		tmp[2] = h[2];
		tmp[3] = h[3];
		tmp[4] = h[4];
		tmp[5] = h[5];
		tmp[6] = h[6];
		tmp[7] = h[7];
		ops.binary_operation(tmp, m);
		h[0] += tmp[0];
		h[1] += tmp[1];
		h[2] += tmp[2];
		h[3] += tmp[3];
		h[4] += tmp[4];
		h[5] += tmp[5];
		h[6] += tmp[6];
		h[7] += tmp[7];
	}
	ops.digest(h, ret);
}

void		launch_hash(t_hash *hash)
{
	int		i;
	int		final_len;
	uint8_t	ret[64];
	t_ops	ops;

	ops = hash->ops;
	i = 0;
	if (!hash->folder)
		print_usage(NULL);
	while (hash->folder[i])
	{
		final_len = pad_message(v_get(&hash->str, i), ops.endian, ops.encodage_len);
		GOO(v_get(&hash->str, i), ops, ret, final_len / ops.encodage_len);
		print_hash_tmp(ret, hash, i, ops);
		++i;
	}
	del_tab(hash->folder);
	v_del_all(&hash->str);
//	del_tab((char **)hash->str);
}
