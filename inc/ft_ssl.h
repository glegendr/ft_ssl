#ifndef FT_SSL_H
# define FT_SSL_H

#include <stdint.h>

typedef struct	s_hash
{
	int	arg;
	void	(*f)(struct s_hash *);
	char	**folder;
	uint8_t	**str;
}		t_hash;



#define NB_HASH 2
#define P_FLAG 1
#define Q_FLAG 1 << 1
#define R_FLAG 1 << 2
#define S_FLAG 1 << 3

void		md5(t_hash *);
void		sha256(t_hash *);
void		print_usage(char *name);
#endif
