package ija.ija2018.homework2.game;

import ija.ija2018.homework2.common.Field;
import ija.ija2018.homework2.common.Figure;
import ija.ija2018.homework2.common.Game;

import java.util.Stack;

public class GameInstance implements Game {

    private Board board;
    private Stack<Object[]> moves;

    public GameInstance(Board board){
        this.board = board;
        this.moves = new Stack<>();
    }

    @Override
    public boolean move(Figure figure, Field field) {
        Object[] thisMove = new Object[3];

        try {
            thisMove[0] = figure.getPosition();
            thisMove[1] = field;

            if ( !field.isEmpty() ) thisMove[2] = field.get();

            this.moves.push(thisMove);

            if ( !figure.move(field) ) {
                this.moves.pop();
                return false;
            }

        } catch (Exception NullPointerException) {
            return false;
        }

        return true;
    }

    @Override
    public void undo(){
        Object[] moveToUndo;

        try {
            moveToUndo = this.moves.pop();
        } catch (Exception EmptyStackException) {
            System.err.println("No more moves to undo!\n");
            return;
        }

        Field startField = (Field) moveToUndo[0];
        Field destField = (Field) moveToUndo[1];

        startField.put(destField.get());
        destField.remove(destField.get());

        if ( moveToUndo[2] != null ) {
            Figure deletedFigure = (Figure) moveToUndo[2];
            destField.put(deletedFigure);
        }
    }
}
