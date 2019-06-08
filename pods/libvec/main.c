#include "inc/vector.h"

int main()
{
	t_vec lol;
	lol = v_new(4);
	v_append_raw(&lol, "WTF\n", 4);
	v_print(&lol);
	v_del(&lol);
}
