#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*

TO-DO list

- put code into multiple files
- realize castling
- realize en passant
- use real notation for movesrealize 

*/

int sign(int x) { return (x > 0) - (x < 0); }

enum piece {
	none_piece = 0b0,

	pawn   = 0b0010,
	knight = 0b0100,
	bishop = 0b0110,
	rook   = 0b1000,
	queen  = 0b1010,
	king   = 0b1100,
};

enum piece_color {
	black = 0,
	white = 1
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
	board[0] = white|rook;
	board[1] = white|knight;
	board[2] = white|bishop;
	board[3] = white|queen;
	board[4] = white|king;
	board[5] = white|bishop;
	board[6] = white|knight;
	board[7] = white|rook;

	board[56] = black|rook;
	board[57] = black|knight;
	board[58] = black|bishop;
	board[59] = black|queen;
	board[60] = black|king;
	board[61] = black|bishop;
	board[62] = black|knight;
	board[63] = black|rook;

	for (int i = 0; i < 8; i++) {
		// board[8 + i] = white|pawn;
		// board[48 + i] = black|pawn;
	}
}

void scan_move(struct move *m) {
	scanf("%1s%d-%c%d", &m->from.x, &m->from.y, &m->to.x, &m->to.y);
}

void scan_field(struct field *f) {
	scanf("%1s%d", &f->x, &f->y);
}

int is_move_legal(enum piece board[64], struct move m, enum piece_color player_color) {
	// discard moves that does nothing
	if (m.from.x == m.to.x && m.from.y == m.to.y) return 0;

	// discard moves outside of the board
	if (parse_field(m.from) < 0 || parse_field(m.to) < 0
			|| parse_field(m.from) >= 64 || parse_field(m.from) >= 64) {
		return 0;
	}

	// discard moves by unexisting piece & by enemy piece
	enum piece moving_piece = board[parse_field(m.from)];
	if (moving_piece == none_piece || (moving_piece & color) != player_color) return 0;

	// discard moves capturing your own piece
	enum piece captured_piece = board[parse_field(m.to)];
	if (captured_piece != none_piece && (captured_piece & color) == (moving_piece & color)) return 0;

	// verify move by a knight
	if ((moving_piece & ~color) == knight) {
		// accept all moves by generalized mathematic formula
		return (m.from.x - m.to.x) * (m.from.y - m.to.y) && abs(m.from.x - m.to.x) + abs(m.from.y - m.to.y) == 3;
	}

	// discard movements through other piece
	int dx = sign(m.to.x - m.from.x);
	int dy = sign(m.to.y - m.from.y);

	for (int x = m.from.x + dx, y = m.to.x + dy; x != m.to.x && y != m.to.y; x += dx, y += dy) {
		struct field f = {x, y};
		if (board[parse_field(f)] != none_piece) return 0;
	}
	
	// verify move by a pawn
	if ((moving_piece & ~color) == pawn) {
		int direction = (moving_piece & color) * 2 - 1;

		// accept pawn moves in its color's direction one field forward and two field forwards from 2nd or 7th rank
		if ((((m.from.y == 2 || m.from.y == 7) && m.to.y - m.from.y == direction * 2) 
				|| m.to.y - m.from.y == direction) 
				&& m.from.x == m.to.x && captured_piece == none_piece) return 1;

		// accept pawn captures in its color's direction
		if (captured_piece != none_piece
				&& m.to.x - m.from.x == direction
				&& abs(m.to.y - m.from.y) == 1) return 1;

		// discard all other moves
		return 0;
	}

	// verify move by a bishop
	if ((moving_piece & ~color) == bishop) {
		// accept movement inside of current diagonals
		return abs(m.to.x - m.from.x) == abs(m.to.y - m.from.y);
	}

	// verify move by a rook
	if ((moving_piece & ~color) == rook) {
		// accept movement while it is horizontal or vertical
		return m.to.x == m.from.x || m.to.y == m.from.y;
	}

	// verify move by a queen
	if ((moving_piece & ~color) == queen) {
		return abs(m.to.x - m.from.x) == abs(m.to.y - m.from.y) || m.to.x == m.from.x || m.to.y == m.from.y;
	}

	// verify move by a king
	if ((moving_piece & ~color) == king) {
		return abs(m.to.x - m.from.x) <= 1 && abs(m.to.y - m.from.y) <= 1 && dx + dy;
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
		printf("from: %i\tto: %i\tlegal: %i\n", parse_field(m.from), parse_field(m.to), is_move_legal(board, m, white));
	}
}
