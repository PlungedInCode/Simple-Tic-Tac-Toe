#include <iostream>

const int WIN = 1000;
const int DRAW = 0;
const int LOSS = -1000;

char AI_MARKER ='O';
char PLAYER_MARKER = 'X';
char EMPTY_SPACE = ' ';


class TicTacToe{
public:
  bool playerMove(char character) {
    std::cout << "Move " << character << ": ";
    int row, col;
    std::cin >> row >> col;
    
    return TicTacToeBoard.put({row, col}, character);
  }

  void Ai_Move(char character) {
    std::pair<int, std::pair<int, int>> ai_move = minimax_optimization(character, 0, LOSS, WIN);
    TicTacToeBoard.put(ai_move.second, character);
    printTicTacToeBoard();
  } 
  // Check if the game is finished
  bool  gameOver() {
    if (TicTacToeBoard.BoardFull() || DRAW != TicTacToeBoard.getBoardState(AI_MARKER)) {
      return true;
    }
    return false;
  }

  void printTicTacToeBoard(){
    TicTacToeBoard.printBord();
  }

  // Apply the minimax game optimization algorithm
  std::pair<int, std::pair<int, int>> minimax_optimization(char marker, int depth, int alpha, int beta) {
    // Initialize best move
    std::pair<int, int> best_move = std::make_pair(-1, -1);
    int best_score = (marker == AI_MARKER) ? LOSS : WIN;

    // If we hit a terminal state (leaf node), return the best score and move
    if (TicTacToeBoard.BoardFull() || DRAW != TicTacToeBoard.getBoardState(AI_MARKER)) {
      best_score = TicTacToeBoard.getBoardState(AI_MARKER);
      return {best_score, best_move};
    }

    for (int i = 0; i < 3; ++i){
      for (int j = 0; j < 3; ++j) {
        if (TicTacToeBoard.get(i,j) != PLAYER_MARKER && TicTacToeBoard.get(i,j) != AI_MARKER) {
          std::pair<int, int> curr_move = {i, j};
          // board[curr_move.first][curr_move.second] = marker;
          TicTacToeBoard.putAi(curr_move, marker);
          // Maximizing player's turn
          if (marker == AI_MARKER) {
            int score = minimax_optimization(PLAYER_MARKER, depth + 1, alpha, beta).first;

            // Get the best scoring move
            if (best_score < score) {
              best_score = score - depth * 10;
              best_move = curr_move;

              // Check if this branch's best move is worse than the best
              // option of a previously search branch. If it is, skip it
              alpha = std::max(alpha, best_score);
              TicTacToeBoard.putAi(curr_move, ' ');
              if (beta <= alpha) { 
                break; 
              }
            }

          } else { // Minimizing opponent's turn 
            int score = minimax_optimization(AI_MARKER, depth + 1, alpha, beta).first;

            if (best_score > score)
            {
              best_score = score + depth * 10;
              best_move = curr_move;

              // Check if this branch's best move is worse than the best
              // option of a previously search branch. If it is, skip it
              beta = std::min(beta, best_score);
              TicTacToeBoard.putAi(curr_move, ' ');
              if (beta <= alpha) 
              { 
                break; 
              }
            }

          }
          TicTacToeBoard.putAi(curr_move, ' '); // Undo move
        }
      }
    }
    return std::make_pair(best_score, best_move);
  }

private:
  struct Board{
  public:
    Board() {
      for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
          board[i][j] = EMPTY_SPACE;
        }
      }
    }
    void printBord() {
      for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
          std::cout << board[i][j];
          if (j != 2) {
            std::cout << '|';
          }
        }
        std::cout << '\n';
        if (i != 2) {
          std::cout << "-+-+-\n";
        }
      }
    }

    bool BoardFull() {
      for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
          if (board[i][j] != 'X' && board[i][j] != 'O') {
            return false;
          }
        }
      }
      return true;
    }

    int getBoardState(const char &character) {
      char oponent_character = 'X';
      if (character == 'X') oponent_character = 'O';

      if (getWinner() == character) {
        return WIN;
      } else if (getWinner() == oponent_character) {
        return LOSS;
      }
      
      if (BoardFull()) return DRAW;

      return DRAW;
    }

    char getWinner() {
      for (int i = 0; i < 3; ++i) {
        if (board[i][0] == board[i][1]  && board[i][2] == board[i][1]) {
          return board[i][0];
        }
        if (board[0][i] == board[1][i] && board[2][i] == board[1][i]) {
          return board[0][i];
        }
      }
      if (board[1][1] == board[0][0] && board[2][2] == board[1][1]) {
        return board[0][0];;
      }
      if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        return board[1][1];;
      }
      return '*';
    }
    
    bool put(const std::pair<int, int> &move, const char &character) {
      if (3 > move.second && 3 > move.first && board[move.first][move.second] != AI_MARKER && board[move.first][move.second] != PLAYER_MARKER) {
        board[move.first][move.second] = character;
        return true;
      }
      std::cout << "Position (" << move.first << ", " << move.second << ") unavaliable.\n";
      return false;
    }

    void putAi(std::pair<int, int> &move, const char &character) {
      board[move.first][move.second] = character;
    }

    char get(int i, int j) {
      return board[i][j];
    }

  private:
    char board[3][3];
  };
  Board TicTacToeBoard;
};

void startGame(int &turn) {
  TicTacToe X;
  X.printTicTacToeBoard();
  if (turn%2==1) {
    std::swap(PLAYER_MARKER, AI_MARKER);
  }
  while (!X.gameOver()) {
    if (turn % 2 == 0) {
      while (!X.playerMove(PLAYER_MARKER));
      X.Ai_Move(AI_MARKER);
    } else {
      X.Ai_Move(AI_MARKER);
      while (!X.playerMove(PLAYER_MARKER));
    }
  }
  X.printTicTacToeBoard();
}

int main() {
  int turn = 0;
  while (true) {
    startGame(turn);
    turn++;
    std::cout << "Do you want continue game. Yes/No?\n";
    std::string ok;
    std::cin >> ok;
    std::cout << ok;
    if (ok == "No" || ok == "no" || ok == "NO") {
      break;
    }
  }
}