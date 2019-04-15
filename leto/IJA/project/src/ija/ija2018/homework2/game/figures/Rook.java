package ija.ija2018.homework2.game.figures;

import ija.ija2018.homework2.common.Field;
import ija.ija2018.homework2.common.Figure;


public class Rook extends Figure {
    public Rook(boolean isWhite){
        super(isWhite);
    }

    @Override
    public boolean move(Field moveTo){
        int newRow = moveTo.getRow();
        int newCol = moveTo.getCol();

        if (this.row != newRow && this.col == newCol){
            if (this.row > newRow) return makeMove(moveTo, Field.Direction.D);
            else return makeMove(moveTo, Field.Direction.U);
        }
        else if (this.row == newRow && this.col != newCol){
            if (this.col > newCol) return makeMove(moveTo, Field.Direction.L);
            else return makeMove(moveTo, Field.Direction.R);
        }
        else return false;
    }

    @Override
    protected boolean makeMove(Field moveTo, Field.Direction dir){
        if ( !isMovePossible(moveTo, dir) ) return false;

        if ( !moveTo.isEmpty() ) {
            Figure figure = moveTo.get();
            if ( figure.isWhite() == this.color ) return false;

            moveTo.remove(figure);
        }

        setAttributesAfterMove(moveTo);

        return true;
    }

    @Override
    protected boolean isMovePossible(Field moveTo, Field.Direction dir){
        Field field = this.position.nextField(dir);

        while (field != moveTo){
            if ( !field.isEmpty() ) return false;
            field = field.nextField(dir);
        }

        return true;
    }
}
