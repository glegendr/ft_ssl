/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/20 14:58:58 by glegendr          #+#    #+#             */
/*   Updated: 2019/05/06 15:14:19 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include "ft_restrictions.h"
# include "../pods/libvec/inc/vector.h"
# include "../pods/libft/inc/libft.h"
# include <wchar.h>

typedef struct	s_st
{
	void			*data;
	char			prin;
	int			precision;
	int			zero;
	int			size;
	int			mask;
	int			string_size;
}				t_st;

char			*ft_format(char const *restrict format, ...);
int			ft_printf(char const *restrict format, ...);

char			*ft_print_dec(t_st *t);
char			*ft_print_hex(t_st *t);
char			*ft_print_pc(t_st *t);

char			*ft_str_is_null(t_st *t, t_vec *vec);
char			*ft_pre_is_null(t_st *t, char **s, t_vec *vec);

void			ft_flag_is_size(t_st *t, int string_s, t_vec *v);
void			ft_flag_is_pre(int *size, int string_s, t_vec *v);
void			ft_flag_is_sharp(t_st *t, int string_s, t_vec *v);
void			ft_flag_is_null(t_st *t, int string_s, t_vec *v);
int			ft_flag_is_space(t_st *t, t_vec *vec, void *data);

char			*wchar_t_to_str(wchar_t c, int i, int *err);
char			*bin_to_dec(char *s);
int			ft_rawtoi(char const *restrict s, int *i, t_st *t, int *cmpt);

int			ft_conv(char c);
int			ft_flags(char c);
void			ft_ini_struct(t_st *t);
char			*ft_push_pointeur(void *data, t_st *t);
char			*match_flags(t_st *t);
int			ft_long_conv(char c);
t_vec			ft_pars(char const *restrict s, va_list *v, char **str);
void			ft_conv_is_zj(t_st *t, int *cmpt, char c);
int			ft_no_flag_founded(char c, t_st *t, int cpmt, va_list *v);
void			ft_s_flags(t_st *t, t_vec *vec);

#endif
