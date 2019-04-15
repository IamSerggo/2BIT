package ija.ija2018.homework2.common;

public abstract class Figure {
    protected boolean color;

    protected int col;
    protected int row;

    protected Field position;

    public Figure(boolean isWhite){
        this.color = isWhite;
    }

    public boolean isWhite(){
        return this.color;
    }

    public boolean move(Field moveTo){
        return true;
    }

    protected boolean makeMove(Field moveTo, Field.Direction dir){
        return true;
    }

    protected boolean isMovePossible(Field moveTo, Field.Direction dir){
        return true;
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
        this.position = field;
    }

    public Field getPosition() { return this.position; }
}
