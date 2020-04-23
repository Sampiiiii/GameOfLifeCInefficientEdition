#include <iostream>
using namespace std;

#include "olcConsoleGameEngine.h"

class GameOfLife : public olcConsoleGameEngine{

public:
	GameOfLife(){
		m_sAppName = L"Game Of Life";
	}

private:
	int *m_output;
	int *m_state;

protected:
	virtual bool OnUserCreate(){

		// Allocating Arrays to Memory
		m_output = new int[ScreenWidth() * ScreenHeight()];
		m_state = new int[ScreenWidth() * ScreenHeight()];

		// Setting Arrays to Zeroes;
		memset(m_output, 0, ScreenWidth() * ScreenHeight() * sizeof(int));
		memset(m_state, 0, ScreenWidth() * ScreenHeight() * sizeof(int));

		//Randomly Filling Array;

		for (int i = 0; i < ScreenWidth() * ScreenHeight(); i++) {
			m_state[i] = rand() % 2;
		}

		auto set = [&](int x, int y, wstring s)
		{
			int p = 0;
			for (auto c : s)
			{
				m_state[y * ScreenWidth() + x + p] = c == L'#' ? 1 : 0;
				p++;
			}
		};

		// Infinite Growth
		set(20, 50, L"########.#####...###......#######.#####");

		return true;
	}

	//One call of this function = one Epoch of time.
	virtual bool OnUserUpdate(float fElapsedTime){

		this_thread::sleep_for(50ms);

		auto cell = [&](int x, int y) {
			return m_output[y * ScreenWidth() + x];
		};

		//Store output state
		for (int i = 0; i < ScreenHeight() * ScreenWidth(); i++)
			m_output[i] = m_state[i];

		for (int x=1; x < ScreenWidth() - 1; x++)
			for (int y = 1; y < ScreenHeight() - 1; y++)
			{
				int numNeighbours = cell(x - 1, y - 1) + cell(x, y - 1) + cell(x + 1, y - 1) + cell(x - 1, y) + cell(x + 1, y) + cell(x - 1, y + 1) + cell(x, y + 1) + cell(x + 1, y + 1);
				if (cell(x, y) == 1)
					m_state[y * ScreenWidth() + x] = numNeighbours == 2 || numNeighbours == 3;
				else
					m_state[y * ScreenWidth() + x] = numNeighbours == 3;

				if (cell(x, y) == 1)
					Draw(x, y, PIXEL_SOLID, rand() % 30 + 1);
				else
					Draw(x, y, PIXEL_SOLID, FG_BLACK);
			}

		return true;
	}
};

int main() {
	GameOfLife game;
	game.ConstructConsole(640, 400, 2, 2);
	game.Start();
	return 0;
}
