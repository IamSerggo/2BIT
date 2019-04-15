package ija.ija2018.homework2;

import ija.ija2018.homework2.common.Game;
import ija.ija2018.homework2.game.Board;
import ija.ija2018.homework2.game.GameInstance;
import ija.ija2018.homework2.game.figures.CheckersPawn;
import ija.ija2018.homework2.game.figures.ChessPawn;
import ija.ija2018.homework2.game.figures.Rook;

public abstract class GameFactory{

    public static Game createChessGame(Board board){
        for (int i = 1; i < board.getSize()+1; i++){

            board.getField(i, 2).put( new ChessPawn(true) );
            board.getField(i, board.getSize()-1).put( new ChessPawn(false) );

            if (i == 1 || i == board.getSize()) {
                board.getField(i, 1).put( new Rook(true) );
                board.getField(i, board.getSize()).put( new Rook(false) );
            }
        }

        return new GameInstance(board);
    }

    public static Game createCheckersGame(Board board){
        for (int i = 1; i < board.getSize()+1; i++){

            if ( i%2 != 0 ) board.getField(i, 1).put( new CheckersPawn(true) );
            else board.getField(i, 2).put( new CheckersPawn(true) );
        }

        return new GameInstance(board);
    }
}
