package ija.ija2018.homework2.game;

import ija.ija2018.homework2.common.Field;
import ija.ija2018.homework2.common.Figure;

public class Pawn implements Figure {
    private boolean color;

    private int col;
    private int row;

    private Field position;

    public Pawn(boolean isWhite){
        this.color = isWhite;
    }

    @Override
    public String getState(){
        String state = "P";
        if (color) state += "[W]";
        else state += "[B]";

        state += col + ":" + row;
        return state;
    }

    @Override
    public boolean isWhite(){
        return this.color;
    }

    @Override
    public boolean move(Field moveTo) {
        Field.Direction dir;

        if ( this.color ) dir = Field.Direction.U;
        else dir = Field.Direction.D;

        if ( this.position.nextField(dir) != moveTo ) return false;
        else {
            if ( !moveTo.isEmpty() ) {
                if ( moveTo.get().isWhite() == this.color ) return false;

                moveTo.remove(moveTo.get());
            }

            this.position.remove(this);
            this.setPosition(moveTo);
            this.setCol(moveTo.getCol());
            this.setRow(moveTo.getRow());

            moveTo.put(this);

            return true;
        }
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
