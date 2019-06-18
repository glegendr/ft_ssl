#include "ft_ssl.h"
#include <libft.h>
#include <ft_printf.h>

void	print_hash(t_vec *ret, t_hash *hash, int i, t_ops ops)
{
	t_vec vec;
	t_vec *init;
	t_vec *folder;

	folder = v_get(&hash->folder, i);
	init = v_get(&hash->str, i);
	vec = v_new(sizeof(char));
	if (!v_raw(folder)) {
		v_append_raw(&vec, v_raw(ret), v_size(ret));
	} else {
		if (!(hash->arg & R_FLAG) && !(hash->arg & Q_FLAG)) {
			v_append_raw(&vec, (void *)ops.name, ft_strlen(ops.name));
			v_push_int(&vec, '(');
			v_append_raw(&vec, v_raw(folder), v_size(folder));
			v_append_raw(&vec, ")= ", 3);
		}
		v_append_raw(&vec, v_raw(ret), v_size(ret));
		if ((hash->arg & R_FLAG) && !(hash->arg & Q_FLAG)) {
			v_push_int(&vec, ' ');
			v_append_raw(&vec, v_raw(folder), v_size(folder));
		}
	}
	v_push_int(&vec, '\n');
	write(ops.fd, v_raw(&vec), v_size(&vec));
	v_del(&vec);
}

static void	transform_hash(uint8_t *ret, t_hash *hash, int i, t_ops ops)
{
	t_vec vec;
	char *hex;

	vec = v_new(sizeof(char));
	for (int i = 0; i < ops.message_len; i++) {
		hex = ft_itoa_base(ret[i], 16, 'x');
		if (ft_strlen(hex) == 1)
			v_push_int(&vec, '0');
		v_append_raw(&vec, hex, ft_strlen(hex));
		free(hex);
	}
	print_hash(&vec, hash, i, ops);
	v_del(&vec);
}

static void	encript(t_vec *ck, t_ops ops, uint8_t *ret, int loop)
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
	if (!v_get(&hash->str, 0))
		print_usage(NULL);
	while (i < v_size(&hash->str))
	{
		final_len = pad_message(v_get(&hash->str, i), ops.endian, ops.encodage_len);
		encript(v_get(&hash->str, i), ops, ret, final_len / ops.encodage_len);
		transform_hash(ret, hash, i, ops);
		++i;
	}
	v_del_all(&hash->folder);
	v_del_all(&hash->str);
}
