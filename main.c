#include <stdio.h>
#include <stdlib.h>

enum piece {
	none_piece = 0b0,

	white_pawn   = 0b0010,
	white_knight = 0b0100,
	white_bishop = 0b0110,
	white_rook   = 0b1000,
	white_queen  = 0b1010,
	white_king   = 0b1100,

	black_pawn   = 0b0011,
	black_knight = 0b0101,
	black_bishop = 0b0111,
	black_rook   = 0b1001,
	black_queen  = 0b1011,
	black_king   = 0b1101,
};

enum piece_color {
	white_piece = 0,
	black_piece = 1
};

int color = 1;

struct field {
	char x;
	int y;
};

struct move {
	struct field from, to;
};

int parse_field(struct field f) {
	return 8 * (f.y - 1) + f.x - 'a';
}

void setup_board(enum piece board[64]) {
	board[0] = white_rook;
	board[1] = white_knight;
	board[2] = white_bishop;
	board[3] = white_queen;
	board[4] = white_king;
	board[5] = white_bishop;
	board[6] = white_knight;
	board[7] = white_rook;

	board[56] = black_rook;
	board[57] = black_knight;
	board[58] = black_bishop;
	board[59] = black_queen;
	board[60] = black_king;
	board[61] = black_bishop;
	board[62] = black_knight;
	board[63] = black_rook;

	for (int i = 0; i < 8; i++) {
		board[8 + i] = white_pawn;
		board[48 + i] = black_pawn;
	}
}

void scan_move(struct move *m) {
	scanf("%1s%d-%c%d", &m->from.x, &m->from.y, &m->to.x, &m->to.y);
}

void scan_field(struct field *f) {
	scanf("%1s%d", &f->x, &f->y);
}

int is_move_legal(enum piece board[64], struct move m, enum piece_color color) {
	if (parse_field(m.from) < 0 || parse_field(m.to) < 0
			|| parse_field(m.from) >= 64 || parse_field(m.from) >= 64) {
		return 0;
	}

	enum piece moving_piece = board[parse_field(m.from)];
	if (moving_piece == none_piece) return 0;

	enum piece captured_piece = board[parse_field(m.to)];
	if (captured_piece != none_piece && (captured_piece & color) == (moving_piece & color)) return 0;

	if (moving_piece == white_pawn) {
		if (((m.from.y == 2 && m.to.y == 4) || m.to.y > m.from.y) && m.from.x == m.to.x) return 1;
		if ((moving_piece & color) != (board[parse_field(m.from)] & color)
				&& m.to.x - m.from.x == 1 
				&& abs(m.to.y - m.from.y) == 1) return 1;
		return 0;
	}

	return 0;
}

enum move_result {
	game_continues,
	white_won,
	black_won,
	draw
};

enum move_result make_move(struct field *a, struct field *b);

int main() {
	enum piece board[64];
	setup_board(board);
	
	struct move m;

	while (1) {
		scan_move(&m);
		printf("from: %i\tto: %i\tlegal: %i\n", parse_field(m.from), parse_field(m.to), is_move_legal(board, m, white_piece));
	}
}
