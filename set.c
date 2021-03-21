#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>

#define MAXV	16
#define ARRAY_SIZE(a)	(int)(sizeof(a)/sizeof(a[0]))

typedef struct __set
{
	char	n;
	char	t[MAXV];
} set_t;

static int		last_n = 3;
static int		n_recs = 0;
static set_t	recs[1024] = { 0 };

static int equal(const set_t *s1, const set_t *s2)
{
	int	x;

	if (s1->n != s2->n)
		return 0;

	for (x = 0; x < s1->n; x++) {
		if (s1->t[x] != s2->t[x])
			return 0;
	}

	return 1;
}

static void rm(set_t *s, int n)
{
	int	x;

	assert(n < s->n);
	assert(n >= 0 && n < MAXV);

	for (x = n; x < s->n - 1; x++)
		s->t[x] = s->t[x + 1];

	s->t[x] = 0;

	s->n -= 1;
}

static void print(set_t *s, int seq)
{
	int	x;

	printf("%3d ==> %d: ", seq, s->n);
	for (x = 0; x < s->n; x++)
		printf(" %2d", s->t[x]);
	printf("\n");
}

static void add_rec(set_t *s)
{
	int	x;

	if (last_n < s->n) {
		last_n = s->n;
		n_recs = 0;
	}

	if (n_recs == ARRAY_SIZE(recs))
		return;

	for (x = 0; x < n_recs; x++) {
		if (equal(&recs[x], s))
			return;
	}

	recs[x] = *s;
	n_recs++;
	print(s, n_recs);
}

static int ok(set_t *s)
{
	int	x, y, z, a, b, c;

	for (x = 0; x < s->n - 2; x++) {
		a = s->t[x];
		for (y = x + 1; y < s->n - 1; y++) {
			b = s->t[y];
			for (z = y + 1; z < s->n; z++) {
				c = s->t[z];
				if ((a + c) == 2 * b)
					return 0;
			}
		}
	}

	add_rec(s);

	return 1;
}

static int try(set_t *s0)
{
	int		x;
	set_t	s;

	if (s0->n < last_n)
		return 1;

	if (ok(s0))
		return 1;

	for (x = 0; x < s0->n; x++) {
		s = *s0;
		rm(&s, x);
		try(&s);
	}

	return 0;
}

int main(int argc, char *argv[])
{
	int	x;
	set_t	s;

	s.n = MAXV;
	for (x = 0; x < ARRAY_SIZE(s.t); x++) {
		s.t[x] = x + 1;
	}
	try(&s);

	return 0;
}
