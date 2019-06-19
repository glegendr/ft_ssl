#include "ft_ssl.h"
#include <libft.h>
#include <ft_printf.h>

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

void		des_ecb(t_hash *tab)
{
	t_ops ops = tab->ops;

	if (!ops.pwd)
		ops.pwd = get_pwd();
	write(1, ops.pwd, ft_strlen(ops.pwd));
//	read_file(&tab, 0, false);
//	into_vec(&tab.folder, NULL);
}

void		des(t_hash *tab)
{
	(void)tab;
}
