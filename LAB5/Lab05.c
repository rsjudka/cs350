/*#include <stdio.h>

main()
{
    int b[4] = {2, 4, 2, 8};

    int *p, *q, *r;

    p = &b[0];
    q = &b[1];
    r = &b[2];

    if (
		p < q < r
		//p != r && *p == *r
		//q-b == &b[3] - &p[1]
		//p[1] == r[-1]
		//&r[-2] == &b[0]
		//q-p+q-p == q+q-p-p
		)
    {
        printf("%s", "yeaaa\n");
    }
} */

#include <stdio.h>

main()
{
int b[4] = {12, 13, 14, 15};
int u = 20, v = 30, *x = &u, *y, *z;
y = &u;
z = &b[2];
// <----- Position 1
printf("x is %d, y is %d, z is %d\n", *x, *y, *z);
printf("u is %d, v is %d\n", u, v);
printf("b[0] is %d, b[1] is %d, b[2] is %d, b[3] is %d\n", b[0], b[1], b[2], b[3]);
++ *x;// (i.e., *x = *x + 1)
y = &v;
--z;
z[1] = 20;
// <----- Position 2
printf("x is %d, y is %d, z is %d\n", *x, *y, *z);
printf("u is %d, v is %d\n", u, v);
printf("b[0] is %d, b[1] is %d, b[2] is %d, b[3] is %d\n", b[0], b[1], b[2], b[3]);
}

/*#include <stdio.h>

main()
{
	typedef struct struct_a
	{
		int a;
		int b[3];
		char *str;
	} Struct_s;
	
	Struct_s x[2], *p = NULL;
	char *s1 = "hello";
	
	int i;
	for (i = 0; i < 3; i++)
	{
		x[0].b[i] = i*i;
		x[1].b[i] - -i;
	}
	p = &x[0];
	p -> a =12;
	p -> str = s1;
	p++;
	p -> a = -23;
	p -> str = s1;
	int *q = &x[1].a;
} */