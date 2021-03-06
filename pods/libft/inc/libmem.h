/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libmem.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glegendr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/13 14:28:31 by glegendr          #+#    #+#             */
/*   Updated: 2019/06/23 15:48:53 by glegendr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBMEM_H
# define LIBMEM_H
# include <stdlib.h>
# include <unistd.h>
# include <string.h>

void				*ft_memcut(void *data, int size);
void				*ft_realloc(void *mem, int mem_size, int new_size);
void				*ft_memccpy(void *dst, const void *src, int c, size_t n);
void				*ft_memchr(const void *s, int c, size_t n);
int					ft_memcmp(const void *s1, const void *s2, size_t n);
void				ft_memdel(void **ap);
void				*ft_memmove(void *dest, const void *src, size_t n);
void				*ft_memset(void *s, int c, size_t n);
void				*ft_memcpy(void *dst, const void *src, size_t n);
void				*ft_memalloc(size_t size);
#endif
