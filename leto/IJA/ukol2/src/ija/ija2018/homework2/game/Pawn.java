package ija.ija2018.homework2.game;

import ija.ija2018.homework2.common.Field;
import ija.ija2018.homework2.common.Figure;

public abstract class Pawn implements Figure {
    // TODO : neviem ci protected je uplne dobre, musim este pozistovat

    protected boolean color;

    protected int col;
    protected int row;

    protected Field position;

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
       return true; //each function has its own overridden move function
    }

    protected void setAttributesAfterMove(Field moveTo){
        this.position.remove(this);

        this.setPosition(moveTo);
        this.setCol(moveTo.getCol());
        this.setRow(moveTo.getRow());

        moveTo.put(this);
    }

    public void setCol(int col){
        this.col = col;
    }

    public void setRow(int row){
        this.row = row;
    }

    public void setPosition(Field field){
        position = field;
    }

    public Field getPosition() { return this.position; }
}
