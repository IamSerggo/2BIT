package ija.ija2018.homework2.common;

import ija.ija2018.homework2.game.Board;

public interface Game {
    boolean move(Figure figure, Field field);
    void undo();
}
