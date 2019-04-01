package ija.ija2018.homework2.common;

public interface Field {
    boolean isEmpty();
    Figure get();

    void addNextField(Field.Direction dirs, Field field);
    Field nextField(Field.Direction dirs);
    boolean put(Figure figure);
    boolean remove(Figure figure);

    //my methods
    int getRow();
    int getCol();

    enum Direction {
        L, LD, D, RD, R, RU, U, LU
    }
}
