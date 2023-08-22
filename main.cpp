#include <iostream>
#include <vector>

/*
00 01 02 03 04 05 06
07 08 09 10 11 12 13
14 15 16 17 18 19 20
21 22 23 24 25 26 27
28 29 30 31 32 33 34
35 36 37 38 39 40 41
*/

/*
  6 13 20 27 34 41 48   55 62     Additional row
+---------------------+ 
| 5 12 19 26 33 40 47 | 54 61     top row
| 4 11 18 25 32 39 46 | 53 60
| 3 10 17 24 31 38 45 | 52 59
| 2  9 16 23 30 37 44 | 51 58
| 1  8 15 22 29 36 43 | 50 57
| 0  7 14 21 28 35 42 | 49 56 63  bottom row
+---------------------+
*/

void printUint_64(uint64_t board){
    std::cout << "  " << (board>>6)%2 << " " << (board>>13)%2 << " " << (board>>20)%2
              << " " << (board>>27)%2 << " " << (board>>34)%2 << " " << (board>>41)%2
              << " " << (board>>48)%2 << "   " << (board>>55)%2 << " " << (board>>62)%2 << std::endl;
    std::cout << "+---------------+" << std::endl;
    for(int i = 5; i>=0;i--){
        std::cout << "| ";
        for(int j = 0;j<7;j++){
            std::cout << (board>>(i+j*7))%2 << " ";
        }
        std::cout << "| " << (board>>(49+i))%2 << " " << (board>>(56+i))%2;
        if(i!=0) std::cout << std::endl;
    }
    std::cout << " " << (board>>(63))%2 << std::endl;
    std::cout << "+---------------+" << std::endl;
    
}


namespace con4{
    class board{
        public:
        uint64_t player1 = 0;
        uint64_t player2 = 0;
        bool turn = 0;
        std::vector<int> moves;
        
        void makeMove(int move){ //move is a number between 0 and 6
            uint64_t placement = (uint64_t(0x3F)<<(move*7))&posiblePlacements();
            if (placement){
                moves.push_back(move);
                if(!turn)
                    player1 |= placement;
                else
                    player2 |= placement;
                
                turn = !turn; 
            }
            
        }
        void undoMove(){
            uint64_t total = player1 | player2;
            uint64_t move = 0;
            
            move = (uint64_t(0x3F)<<(moves.back()*7))&((total>>1)^total)&0xFDFBF7EFDFBF;
            moves.pop_back();
            if(!turn){
                player2 ^= move;
            }
            else{
                player1 ^= move;
            }
            turn = !turn;
        }
        uint64_t posiblePlacements(){
            uint64_t total = player1 | player2;
            return (total<<1 | 0x40810204081)&(~total)&0xFDFBF7EFDFBF;
        }
        bool isWin(){
            uint64_t bitboard = 0;
            if(turn)
                bitboard = player1;
            else
                bitboard = player2;

            if (bitboard & (bitboard >> 6) & (bitboard >> 12) & (bitboard >> 18)) return true; // diagona "\"
            if (bitboard & (bitboard >> 8) & (bitboard >> 16) & (bitboard >> 24)) return true; // diagonal "/"
            if (bitboard & (bitboard >> 7) & (bitboard >> 14) & (bitboard >> 21)) return true; // horizontal "-"
            if (bitboard & (bitboard >> 1) & (bitboard >>  2) & (bitboard >>  3)) return true; // vertical "|"
            return false;
        }
        bool isFull(){
            if (0xFDFBF7EFDFBF^((player1|player2)&0xFDFBF7EFDFBF)) return false;
            return true;
        }
        std::vector<int> posibleMoves(){
            std::vector<int> moves;
            for(int i = 0; i < 7; i++){
                if((uint64_t(0x3F)<<(i*7))&posiblePlacements())
                    moves.push_back(i);
            }
            return moves;
        }
    };
    std::ostream &operator<<(std::ostream &os, board const &m) { 
            std::string printStr = "+---------------+\n";
            for(int i = 5;i>=0;i--){
                printStr += "| ";
                for(int j = 0; j < 43; j+=7){
                    if((m.player1>>(i+j))%2){
                        printStr += "X ";
                    }
                    else if((m.player2>>(i+j))%2){
                        printStr += "O ";
                    }
                    else{
                        printStr += ". ";
                    }
                }
                printStr += "|\n";
            }
            printStr += "+---------------+\n| A B C D E F G |\n+---------------+";
            os << printStr;
        return os;
    }
}

namespace AI{

    class bot{
        public:
        int fullSearch(con4::board position){
            if(position.isWin())
                return -1;
            if(position.isFull())
                return 0;
            int value = -1;
            for(int move:position.posibleMoves()){
                position.makeMove(move);
                value = std::max(value,-fullSearch(position));
                position.undoMove();
                if(value == 1){
                    break;
                }
            }
            return value;
        }
    };
    
    
    
}




int main(){
    uint64_t tal = 0xFDFBF7EFDFBF;
    //con4::board myBoard;
    con4::board myBoard;
    
    myBoard.makeMove(0);std::cout << myBoard << std::endl;
    myBoard.makeMove(1);std::cout << myBoard << std::endl;
    myBoard.makeMove(1);std::cout << myBoard << std::endl;
    myBoard.makeMove(2);std::cout << myBoard << std::endl;
    myBoard.makeMove(3);std::cout << myBoard << std::endl;

    myBoard.makeMove(4);std::cout << myBoard << std::endl;
    
    myBoard.makeMove(5);std::cout << myBoard << std::endl;
    myBoard.makeMove(6);std::cout << myBoard << std::endl;
    myBoard.makeMove(1);std::cout << myBoard << std::endl;
    myBoard.makeMove(4);std::cout << myBoard << std::endl;
    myBoard.makeMove(2);std::cout << myBoard << std::endl;
    myBoard.makeMove(5);std::cout << myBoard << std::endl;
    myBoard.makeMove(2);std::cout << myBoard << std::endl;
    myBoard.makeMove(1);std::cout << myBoard << std::endl;
    myBoard.makeMove(4);std::cout << myBoard << std::endl;
    myBoard.makeMove(1);std::cout << myBoard << std::endl;
    myBoard.makeMove(3);std::cout << myBoard << std::endl;
    myBoard.makeMove(0);std::cout << myBoard << std::endl;
    myBoard.makeMove(0);std::cout << myBoard << std::endl;
    myBoard.makeMove(5);std::cout << myBoard << std::endl;
    myBoard.makeMove(2);std::cout << myBoard << std::endl;
    myBoard.makeMove(3);std::cout << myBoard << std::endl;
    myBoard.makeMove(6);std::cout << myBoard << std::endl;
    myBoard.makeMove(6);std::cout << myBoard << std::endl;
    myBoard.makeMove(6);std::cout << myBoard << std::endl;
    
    
    /*
    myBoard.makeMove(6);std::cout << myBoard << std::endl;
    myBoard.makeMove(6);std::cout << myBoard << std::endl;
    myBoard.makeMove(0);std::cout << myBoard << std::endl;
    myBoard.makeMove(0);std::cout << myBoard << std::endl;
    myBoard.makeMove(3);std::cout << myBoard << std::endl;
    myBoard.makeMove(0);std::cout << myBoard << std::endl;
    myBoard.makeMove(3);std::cout << myBoard << std::endl;
    myBoard.makeMove(1);std::cout << myBoard << std::endl;
    */
    
    
    
    /*
    std::cout << std::endl << std::endl << "Player 1" << std::endl;
    printUint_64(myBoard.player1);
    std::cout << "Player 2" << std::endl;
    printUint_64(myBoard.player2);
    */
    
   std::cout << myBoard << std::endl;
   std::cout << myBoard.isWin() << std::endl;
   AI::bot myBot;
   std::cout << myBot.fullSearch(myBoard) << std::endl;
    
    
    

    return 0;
}