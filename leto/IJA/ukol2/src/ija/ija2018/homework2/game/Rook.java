package ija.ija2018.homework2.game;

import ija.ija2018.homework2.common.Field;
import ija.ija2018.homework2.common.Figure;


public class Rook implements Figure{
    private boolean color;

    private int col;
    private int row;

    private Field position;

    public Rook(boolean isWhite){
        this.color = isWhite;
    }

    @Override
    public boolean isWhite(){
        return this.color;
    }

    @Override
    public String getState(){
        String state = "V";
        if (color) state += "[W]";
        else state += "[B]";

        state += col + ":" + row;
        return state;
    }

    @Override
    public boolean move(Field moveTo){
        int newRow = moveTo.getRow();
        int newCol = moveTo.getCol();

        if (this.row != newRow && this.col == newCol){
            if (this.row > newRow) return makeMove(moveTo, Field.Direction.D);
            else return makeMove(moveTo, Field.Direction.U);
        }
        else if (this.row == newRow && this.col != newCol){
            if (this.col > newCol) return makeMove(moveTo, Field.Direction.L);
            else return makeMove(moveTo, Field.Direction.R);
        }
        else return false;
    }


    private boolean makeMove(Field moveTo, Field.Direction dir){
        if ( !isMovePossible(moveTo, dir) ) return false;

        if ( !moveTo.isEmpty() ) {
            Figure figure = moveTo.get();
            if ( figure.isWhite() == this.color ) return false;

            moveTo.remove(figure);
        }

        this.position.remove(this);

        moveTo.put(this);

        this.setPosition(moveTo);
        this.setCol(moveTo.getCol());
        this.setRow(moveTo.getRow());

        return true;
    }

    private boolean isMovePossible(Field moveTo, Field.Direction dir){
        Field field = this.position.nextField(dir);

        while (field != moveTo){
            if ( !field.isEmpty() ) return false;
            field = field.nextField(dir);
        }

        return true;
    }

    public void setCol(int col){
        this.col = col;
    }

    public void setRow(int row){
        this.row = row;
    }

    public void setPosition(Field field){
        this.position = field;
    }

    public Field getPosition() { return this.position; }
}
