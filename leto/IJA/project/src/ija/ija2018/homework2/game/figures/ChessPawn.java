package ija.ija2018.homework2.game.figures;

import ija.ija2018.homework2.common.Field;

public class ChessPawn extends Pawn {

    public ChessPawn(boolean isWhite){
        super(isWhite);
    }

    @Override
    public boolean move(Field moveTo) {
        Field.Direction dir;

        if ( this.color ) dir = Field.Direction.U;
        else dir = Field.Direction.D;

        if ( this.position.nextField(dir) != moveTo ) return false;
        else {
            if ( !moveTo.isEmpty() ) {
                if ( moveTo.get().isWhite() == this.color ) return false;

                moveTo.remove(moveTo.get());
            }

            setAttributesAfterMove(moveTo);
            return true;
        }
    }
}
