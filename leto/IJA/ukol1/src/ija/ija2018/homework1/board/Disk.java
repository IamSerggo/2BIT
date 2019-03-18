package ija.ija2018.homework1.board;

public class Disk {
    private boolean color;

    private int col;
    private int row;

    private Field position;

    public Disk(boolean isWhite){
        this.color = isWhite;
    }

    public boolean isWhite(){
        return this.color;
    }

    public boolean move(Field moveTo){
        int newRow = moveTo.getRow();
        int newCol = moveTo.getCol();

        if (this.row != newRow && this.col == newCol){
            if (this.row > newRow) return rowMove(moveTo, Field.Direction.U);
            else return rowMove(moveTo, Field.Direction.D);
        }
        else if (this.row == newRow && this.col != newCol){
            if (this.col > newCol) return colMove(moveTo, Field.Direction.R);
            else return colMove(moveTo, Field.Direction.L);
        }
        else return false;
    }

    private boolean rowMove(Field moveTo, Field.Direction dir){
        Field field = moveTo;

        while (field.getRow() != this.row){
            if ( !field.isEmpty() ) return false;
            field = field.nextField(dir);
        }

        makeMove(moveTo);
        return true;
    }

    private boolean colMove(Field moveTo, Field.Direction dir){
        Field field = moveTo;

        while (field.getCol() != this.col){
            if ( !field.isEmpty() ) return false;
            field = field.nextField(dir);
        }

        makeMove(moveTo);
        return true;
    }

    private void makeMove(Field moveTo){
        this.position.remove(this);

        if ( !moveTo.isEmpty() ) moveTo.remove(moveTo.get());

        moveTo.put(this);

        this.setPosition(moveTo);
        this.setCol(moveTo.getCol());
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
}
