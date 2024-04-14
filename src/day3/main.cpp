/* Tic Tac Toe Game Design */

#include <graphics.h>

char board_data[3][3] = {
	{'-', '-', '-'},
	{'-', '-', '-'},
	{'-', '-', '-'}
};

char current_piece = 'O';

/* Check whether player with certain color has won */
bool CheckWin(char chr) {
	if (board_data[0][0] == chr && board_data[0][1] == chr && board_data[0][2] == chr)
		return true;
	if (board_data[1][0] == chr && board_data[1][1] == chr && board_data[1][2] == chr)
		return true;
	if (board_data[2][0] == chr && board_data[2][1] == chr && board_data[2][2] == chr)
		return true;
	if (board_data[0][0] == chr && board_data[1][0] == chr && board_data[2][0] == chr)
		return true;
	if (board_data[0][1] == chr && board_data[1][1] == chr && board_data[2][1] == chr)
		return true;
	if (board_data[0][2] == chr && board_data[1][2] == chr && board_data[2][2] == chr)
		return true;
	if (board_data[0][0] == chr && board_data[1][1] == chr && board_data[2][2] == chr)
		return true;
	if (board_data[0][2] == chr && board_data[1][1] == chr && board_data[2][0] == chr)
		return true;

	return false;
}

/* Check whether it is a draw */
bool CheckDraw() {
	for (size_t i = 0; i < 3; ++i)
		for (size_t j = 0; j < 3; ++j)
			if (board_data[i][j] == '-')
				return false;
	return true;
}

/* Draw the net of board  */
void DrawBoard() {
	line(0, 200, 600, 200);
	line(0, 400, 600, 400);
	line(200, 0, 200, 600);
	line(400, 0, 400, 600);
}

/* Draw piece */
void DrawPiece() {
	for (size_t i = 0; i < 3; ++i)
		for (size_t j = 0; j < 3; ++j)
			switch (board_data[i][j]) {
			case 'O':
				circle(200 * j + 100, 200 * i + 100, 100);
				break;
			case 'X':
				line(200 * j, 200 * i, 200 * (j + 1), 200 * (i + 1));
				line(200 * (j + 1), 200 * i, 200 * j, 200 * (i + 1));
				break;
			case '-':
				break;
			}
	
}

/* Draw current tip text */
void DrawTipText() {
	static TCHAR tip_text[64];
	_stprintf_s(tip_text, _T("Current Piece Type: %c"), current_piece);
	settextcolor(RGB(0x39, 0xc5, 0xbb));
	outtextxy(0, 0, tip_text);
}

int main() {
	/* Initialize */
	initgraph(600, 600);
	bool running = true;
	ExMessage msg;
	BeginBatchDraw();
	

	while (running) {

		/* ReadOperation */
		while (peekmessage(&msg)) {
			DWORD start_time = GetTickCount();
			
			/* Calculate the click place  */
			if (msg.message == WM_LBUTTONDOWN) {
				int x = msg.x, 
					y = msg.y;

				int index_x = x / 200, 
					index_y = y / 200;

				/* Try to move a piece */
				if (board_data[index_y][index_x] == '-') {
					board_data[index_y][index_x] = current_piece;


					/* Change the type of current piece */
					if (current_piece == 'O')
						current_piece = 'X';
					else
						current_piece = 'O';
				}
			}

			DWORD end_time = GetTickCount();
			DWORD delta_time = end_time - start_time;

			/* Game is 144Hz */
			if (delta_time < 1000 / 144)
				Sleep(1000 / 144 - delta_time);
		}

		/* DataProcessing */

		/* DrawPicture */
		cleardevice();
		DrawBoard();
		DrawPiece();
		DrawTipText();
		FlushBatchDraw();

		/* Game Over Check */
		if (CheckWin('X')) {
			MessageBox(GetHWnd(), _T("X Player Win"), _T("Game Over!"), MB_OK);
			running = false;
		}
		else if (CheckWin('O')) {
			MessageBox(GetHWnd(), _T("O Player Win"), _T("Game Over!"), MB_OK);
			running = false;
		}
		else if (CheckDraw()) {
			MessageBox(GetHWnd(), _T("Draw!"), _T("Game Over!"), MB_OK);
			running = false;
		}
	}

	EndBatchDraw();
}
