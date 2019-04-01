package ija.ija2018.homework2.common;

public interface Figure {
    String getState();
    void setCol(int col);
    void setRow(int row);
    void setPosition(Field field);
    Field getPosition();
    boolean isWhite();
    boolean move(Field moveTo);
}
