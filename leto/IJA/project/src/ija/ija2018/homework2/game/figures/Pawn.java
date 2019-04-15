package ija.ija2018.homework2.game.figures;

import ija.ija2018.homework2.common.Field;
import ija.ija2018.homework2.common.Figure;

public abstract class Pawn extends Figure {
    // TODO : neviem ci protected je uplne dobre, musim este pozistovat

    public Pawn(boolean isWhite){
        super(isWhite);
    }

    @Override
    public boolean move(Field moveTo) {
       return true; //each function has its own overridden move function
    }
}
