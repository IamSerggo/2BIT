package ija.ija2018.homework1.board;

public interface Field {
    void addNextField(Field.Direction dirs, Field field);
    Field nextField(Field.Direction dirs);
    Disk get();
    boolean isEmpty();
    boolean put(Disk disk);
    boolean remove(Disk disk);

    //my methods
    int getRow();
    int getCol();

    enum Direction {
        L, LD, D, RD, R, RU, U, LU
    }
}
