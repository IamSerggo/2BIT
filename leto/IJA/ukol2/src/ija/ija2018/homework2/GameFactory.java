package ija.ija2018.homework2;

import ija.ija2018.homework2.common.Figure;
import ija.ija2018.homework2.common.Game;
import ija.ija2018.homework2.game.Board;
import ija.ija2018.homework2.game.GameInstance;
import ija.ija2018.homework2.game.Pawn;
import ija.ija2018.homework2.game.Rook;

public abstract class GameFactory{

    public static Game createChessGame(Board board){
        Figure bpawn;
        Figure wpawn;
        Figure brook;
        Figure wrook;

        for (int i = 1; i < board.getSize()+1; i++){
            bpawn = new Pawn(false);
            wpawn = new Pawn(true);

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
            pawn = new Pawn(true);

            if ( i%2 != 0 ) board.getField(i, 1).put(pawn);
            else board.getField(i, 2).put(pawn);
        }

        return new GameInstance(board);
    }
}
