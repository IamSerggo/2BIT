package ija.ija2018.homework2.game;

import ija.ija2018.homework2.common.Field;

public class CheckersPawn extends Pawn {

    public CheckersPawn(boolean isWhite){
        super(isWhite);
    }

    // TODO : nepaci sa mi tato funkcia, funguje ale musim ju este inak lepsie vymysliet
    @Override
    public boolean move(Field moveTo){
        if ( this.color ) {
            if ( this.position.nextField(Field.Direction.RU) == moveTo){
                return makeMove(moveTo, Field.Direction.RU);
            }
            else if ( this.position.nextField(Field.Direction.LU) == moveTo ){
                return makeMove(moveTo, Field.Direction.LU);
            }
            else return false;
        }
        else {
            if ( this.position.nextField(Field.Direction.RD) == moveTo){
                return makeMove(moveTo, Field.Direction.RD);
            }
            else if ( this.position.nextField(Field.Direction.LD) == moveTo ){
                return makeMove(moveTo, Field.Direction.LD);
            }
            else return false;
        }
    }

    private boolean makeMove(Field moveTo, Field.Direction dir){
        Field field = this.position.nextField(dir);

        if ( !field.isEmpty() ){
            if ( moveTo.get().isWhite() == this.color ) return false;
            else moveTo.remove(moveTo.get());
        }

        setAttributesAfterMove(moveTo);

        return true;
    }
}
