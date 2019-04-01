package ija.ija2018.homework2.game;

import ija.ija2018.homework2.common.Field;
import ija.ija2018.homework2.common.Figure;

import java.util.HashMap;
import java.util.Map;

public class BoardField implements Field {
    private int col;
    private int row;

    private Figure figure;

    private Map<Field.Direction, Field> neighbors;

    public BoardField(int col, int row){
        this.col = col;
        this.row = row;

        neighbors = new HashMap<>();
    }

    public void addNextField(Field.Direction dirs, Field field){
        neighbors.put(dirs, field);
    }

    public Field nextField(Field.Direction dirs){
        return neighbors.get(dirs);
    }

    public boolean put(Figure figure){
        if ( !isEmpty() ) return false;
        else {
            this.figure = figure;
            this.figure.setCol(this.col);
            this.figure.setRow(this.row);
            this.figure.setPosition(this);
            return true;
        }
    }

    public Figure get(){
        return this.figure;
    }

    public boolean remove(Figure figure){
        if ( isEmpty() || this.figure != figure ) return false;
        else {
            this.figure = null;
            return true;
        }
    }

    public boolean isEmpty(){
        return this.figure == null;
    }

    public int getCol(){
        return this.col;
    }

    public int getRow(){
        return this.row;
    }
}
