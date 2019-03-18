package ija.ija2018.homework1.board;

import java.util.HashMap;
import java.util.Map;

public class BoardField implements Field {
    private int col;
    private int row;

    private Disk disk;

    private Map<Field.Direction, Field> neighbors;

    public BoardField(int col, int row){
        this.col = col;
        this.row = row;

        neighbors = new HashMap<>();
    }

    @Override
    public void addNextField(Field.Direction dirs, Field field){
        neighbors.put(dirs, field);
    }

    @Override
    public Field nextField(Field.Direction dirs){
        return neighbors.get(dirs);
    }

    @Override
    public boolean put(Disk disk){
        if ( !isEmpty() ) return false;
        else {
            this.disk = disk;
            this.disk.setCol(this.col);
            this.disk.setRow(this.row);
            this.disk.setPosition(this);
            return true;
        }
    }

    @Override
    public Disk get(){
        return this.disk;
    }

    @Override
    public boolean remove(Disk disk){
        if ( isEmpty() || this.disk != disk ) return false;
        else {
            this.disk = null;
            return true;
        }
    }

    @Override
    public boolean isEmpty(){
        return this.disk == null;
    }

    @Override
    public int getCol(){
        return this.col;
    }

    @Override
    public int getRow(){
        return this.row;
    }
}
