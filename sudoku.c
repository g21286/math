
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include <string.h>

typedef struct _v {
	int	v;
	int	d[9];
} v_t;

typedef struct _s {
	v_t	n[9][9];	
} s_t;

char get_display_char(char ch)
{
	if (ch == 0)
		return ' ';
	else
		return '0' + ch;
}

void init(s_t *s)
{
	int	x, y, z;
	for (y = 0; y < 9; y++) {
		for (x = 0; x < 9; x++) {
			s->n[y][x].v = 0;
			for (z = 0; z < 9; z++) {
				s->n[y][x].d[z] = z + 1;
			}
		}
	}
}

int count(v_t *v)
{
	int	x, n;

	for (x = 0, n = 0; x < 9; x++) {
		if (v->d[x] != 0)
			n++;
	}

	return n;
}

/* v1 >= v2 */
int ge(v_t *v1, v_t *v2)
{
	int	x;

	for (x = 0; x < 9; x++) {
		if ((v1->d[x] != v2->d[x]) && (v2->d[x] != 0))
			return 0;
	}

	return 1;
}

#define COLOR_NC			"\e[0m"			// No Color
#define COLOR_BLACK			"\e[0;30m"
#define COLOR_GRAY			"\e[1;30m"
#define COLOR_RED			"\e[0;31m"
#define COLOR_LIGHT_RED		"\e[1;31m"
#define COLOR_GREEN			"\e[0;32m"
#define COLOR_LIGHT_GREEN	"\e[1;32m"
#define COLOR_BROWN			"\e[0;33m"
#define COLOR_YELLOW		"\e[1;33m"
#define COLOR_BLUE			"\e[0;34m"
#define COLOR_LIGHT_BLUE	"\e[1;34m"
#define COLOR_PURPLE		"\e[0;35m"
#define COLOR_LIGHT_PURPLE	"\e[1;35m"
#define COLOR_CYAN			"\e[0;36m"
#define COLOR_LIGHT_CYAN	"\e[1;36m"
#define COLOR_LIGHT_GRAY	"\e[0;37m"
#define COLOR_WHITE			"\e[1;37m"
void print_sudoku(s_t *s)
{
	int		x, y, z, m;

	printf("===========================================================\n");
	printf("     |   1    2    3   |   4    5    6   |   7    8    9   |\n");
	for (y = 0; y < 9; y++) {
		if ((y != 0) && (y % 3) != 0)
			printf("     |                 |                 |                 |");
		for (z = 0; z < 3; z++) {
			if ((z == 0) && (y %3 ) == 0)
				//printf("==========================================================|");
				printf("-----+-----------------+-----------------+-----------------|");
			if (z == 0)
				printf("\n");
			if (z == 1)
				printf(COLOR_RED " %c:" COLOR_NC, 'A' + y);
			else
				printf("   ");
			for (x = 0; x < 9; x++) {
				if ((x % 3) == 0)
					printf("  |");
				if (s->n[y][x].v != 0) {
					if (z == 1)
						printf(COLOR_PURPLE "   %c " COLOR_NC, get_display_char(s->n[y][x].v));
					else
						printf("     ");
				} else {
					m = (z * 3);
					printf("  %c%c%c",
						get_display_char(s->n[y][x].d[m]),
						get_display_char(s->n[y][x].d[m + 1]),
						get_display_char(s->n[y][x].d[m + 2]));
				}
			}
			printf("  |\n");
		}
	}
	printf("===========================================================\n");
}

#define is_blank(c)		(((c) == ' ') || ((c) == '\t'))
#define CMD_QUIT		0
#define CMD_SET			1
#define CMD_SET_ROW		2
#define CMD_HELP		3
#define CMD_ERR			9
int get_cmd(int *row, int *col, int *v, char rdata[9])
{
	char	cmd[64];
	char	*s = cmd;
	int		i;

	printf("input: ");
	fgets(cmd, sizeof(cmd), stdin);

	while (is_blank(*s))
		s++;
	if ((*s >='A') && (*s <= 'Q'))
		*s += 'a' - 'A';

	if ((*s >='a') && (*s <= 'i')) {
		*row = *s - 'a';
		s++;

		/* a2 3 */
		if ((*s >='1') && (*s <= '9')) {
			*col = *s - '1';
			s++;

			while (is_blank(*s))
				s++;

			if ((*s >='1') && (*s <= '9')) {
				*v = *s - '0';
				return CMD_SET;
			}
		}

		/* b x97xx4168 */
		if (is_blank(*s)) {
			while (is_blank(*s))
				s++;

			for (i = 0; ((i < 9) && (s[i] != 0)); i++) {
				if (s[i] >= '0' && s[i] <= '9')
					rdata[i] = s[i];
				else
					rdata[i] = '0';
			}
			for (; i < 9; i++)
				rdata[i] = '0';

			return CMD_SET_ROW;
		}
	} else if (*s == '?') {
		return CMD_HELP;
	} else if (*s == 'q')
		return CMD_QUIT;


	return CMD_ERR;
}

#define BY_ROW		1
#define BY_COL		2
#define BY_CLUSTER	3
void get_data(s_t *s, int type, int n, v_t *vs[9])
{
	int		x, y, i, by, bx;

	memset(vs, 0, 9 * sizeof(vs[0]));
	assert(n >= 0 && n < 9);
	switch (type) {
	case BY_ROW:
		y = n;
		for (x = 0; x < 9; x++) {
			vs[x] = &s->n[y][x];
		}
		break;
	case BY_COL:
		x = n;
		for (y = 0; y < 9; y++) {
			vs[y] = &s->n[y][x];
		}
		break;
	case BY_CLUSTER:
		by = (n / 3) * 3;
		bx = (n % 3) * 3;
		i  = 0;
		for (y = by; y < by+3; y++) {
			for (x = bx; x < bx+3; x++) {
				vs[i++] = &s->n[y][x];
			}
		}
		break;	
	default:
		assert(0);
		break;
	}
}

void check_determine(v_t *u)
{
	int	i, n, t = 0;

	if (u->v != 0)
		return;

	for (i = 0, n = 0; i < 9; i++) {
		if (u->d[i] != 0) {
			n++;
			t = u->d[i];
		}
	}

	if (n == 1) {
		u->v = t;
	}
}

void rm_list(v_t *u, int v)
{
	assert((v >= 0) && (v <= 9));
	if (v == 0)
		return;

	u->d[v - 1] = 0;
	check_determine(u);
}

int is_fs(v_t *v1, v_t *v2, int *n, int *fsv)
{
	int	x, c;

	if ((v1->v != 0) || (v2->v != 0))
		return 0;

	for (x = 0, c = 0; x < 9; x++) {
		if (v1->d[x] != v2->d[x])
			return 0;
		if (v1->d[x] != 0) {
			fsv[c++] = v1->d[x];
		}
	}
	*n = c;

	return c != 0;
}

void rm_fs_candidate(s_t *s, int type, int n, v_t *vs[9])
{
	int	x, y, fsv[9];

	for (x = 0; x < 9; x++) {
		for (y = x + 1; y < 9; y++) {
			if (!is_fs(vs[x], vs[y], &n, fsv))
				continue;
		}
	}
	// TODO
}

void update_candidate(s_t *s, int y, int x, int v)
{
	int	r, c, by, bx;
	v_t	*vrow[9], *vcol[9], *vcluster[9];

	/* remove from row/column */
	for (r = 0; r < 9; r++)
		rm_list(&s->n[r][x], v);
	for (c = 0; c < 9; c++)
		rm_list(&s->n[y][c], v);

	/* remove from cluster */
	by = (y/3) * 3;
	bx = (x/3) * 3;
	for (r = by; r < by + 3; r++) {
		for (c = bx; c < bx + 3; c++) {
			if ((r == y) && (c == x))
				continue;
			rm_list(&s->n[r][c], v);
		}
	}

	/* remove row full-score */
	for (y = 0; y < 9; y++) {
		get_data(s, BY_ROW, y, vrow);
		rm_fs_candidate(s, BY_ROW, y, vrow);
	}

	/* remove column full-score */
	for (x = 0; x < 9; x++) {
		get_data(s, BY_COL, x, vcol);
		rm_fs_candidate(s, BY_COL, x, vcol);
	}

	/* remove cluster full-score */
	for (x = 0; x < 9; x++) {
		get_data(s, BY_CLUSTER, x, vcluster);
		rm_fs_candidate(s, BY_CLUSTER, x, vcluster);
	}
}

void set(s_t *s, int y, int x, int v)
{
	int	i;

	s->n[y][x].v = v;

	if (v != 0) {
		for (i = 0; i < 9; i++) {
//			if (v != (i+1))
				s->n[y][x].d[i] = 0;
//			else
//				assert(s->n[y][x].d[i] == v);
		}
	}

	update_candidate(s, y, x, v);
}

void set_row(s_t *s, int y, char rdata[9])
{
	int	c;

	for (c = 0; c < 9; c++) {
		assert(rdata[c] >= '0' && rdata[c] <= '9');
		set(s, y, c, rdata[c] - '0');
	}
}

int check_dup(v_t **vs, int *dup)
{
	int	x, t, count[9];

	memset(count, 0, sizeof(count));
	for (x = 0; x < 9; x++) {
		t = vs[x]->v;
		if (t != 0) {
			if (count[t-1] != 0) {
				*dup = t;
				return 1;
			}
			count[t-1]++;
		}
	}
	return 0;
}

int check(s_t *s)
{
	v_t	*vs[9];
	int	x, y, cluster, dup;

	for (y = 0; y < 9; y++) {
		get_data(s, BY_ROW, y, vs);
		if (check_dup(vs, &dup) != 0) {
			printf("dup value %d in row %C\n", dup, 'A'+y);
			return 1;
		}
	}

	for (x = 0; x < 9; x++) {
		get_data(s, BY_COL, x, vs);
		if (check_dup(vs, &dup) != 0) {
			printf("dup value %d in column %d\n", dup, x+1);
			return 1;
		}
	}

	for (cluster = 0; cluster < 9; cluster++) {
		get_data(s, BY_CLUSTER, cluster, vs);
		if (check_dup(vs, &dup) != 0) {
			printf("dup value %d in cluster %d\n", dup, cluster+1);
			return 1;
		}
	}

	return 0;
}

void show_help(void)
{
	printf("b2 1: let B2 = 1\n");
	printf("c 03xx79: let C2 = 3, C5 = 7, C6 = 9\n");
	printf("q to quit\n");
	printf("notes: All commands are case insensitive\n");
}

int main(int argc, char *argv[])
{
	s_t		s;
	char	rdata[9];
	int		cmd, x, y, v;

	init(&s);
	print_sudoku(&s);

	while ((cmd = get_cmd(&y, &x, &v, rdata)) != CMD_QUIT) {
		switch (cmd) {
		case CMD_SET:
			set(&s, y, x, v);
			break;
		case CMD_SET_ROW:
			set_row(&s, y, rdata);
			break;
		case CMD_HELP:
			show_help();
			break;
		case CMD_ERR:
			break;
		}
		if (cmd != CMD_HELP)
			print_sudoku(&s);
		if (check(&s) != 0)
			break;
	}

	return 0;
}
