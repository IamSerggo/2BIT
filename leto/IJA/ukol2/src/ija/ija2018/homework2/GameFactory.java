package ija.ija2018.homework2;

import ija.ija2018.homework2.common.Figure;
import ija.ija2018.homework2.common.Game;
import ija.ija2018.homework2.game.*;

public abstract class GameFactory{

    public static Game createChessGame(Board board){
        Figure bpawn;
        Figure wpawn;
        Figure brook;
        Figure wrook;

        for (int i = 1; i < board.getSize()+1; i++){
            bpawn = new ChessPawn(false);
            wpawn = new ChessPawn(true);

            board.getField(i, 2).put(wpawn);
            board.getField(i, board.getSize()-1).put(bpawn);

            if (i == 1 || i == board.getSize()) {
                brook = new Rook(false);
                wrook = new Rook(true);

                board.getField(i, 1).put(wrook);
                board.getField(i, board.getSize()).put(brook);
            }
        }

        return new GameInstance(board);
    }

    public static Game createCheckersGame(Board board){
        Figure pawn;

        for (int i = 1; i < board.getSize()+1; i++){
            pawn = new CheckersPawn(true);

            if ( i%2 != 0 ) board.getField(i, 1).put(pawn);
            else board.getField(i, 2).put(pawn);
        }

        return new GameInstance(board);
    }
}
