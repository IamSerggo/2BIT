package ija.ija2018.homework2.game;

import ija.ija2018.homework2.common.Field;


public class Board {
    private int size;
    private Field[][] board;

    public Board(int size) {
        this.size = size;
        this.board = new Field[size][size];

        initializeBoard();
    }

    private void initializeBoard() {
        for (int col = 0; col < size; col++) {
            for (int row = size - 1; row >= 0; row--) {
                this.board[col][row] = new BoardField(col + 1, row + 1);
            }
        }

        for (int col = 0; col < size; col++) {
            for (int row = size - 1; row >= 0; row--) {
                setNeighbors(this.board[col][row]);
            }
        }
    }

    private void setNeighbors(Field field) {
        int rowIdx = field.getRow();
        int colIdx = field.getCol();

        int[] rowIdxChange = {0, -1, -1, -1, 0, +1, +1, +1};
        int[] colIdxChange = {-1, -1, 0, +1, +1, +1, 0, -1};

        int changeIdx = 0;
        for (Field.Direction dir : Field.Direction.values()) {
            field.addNextField(dir, getField(colIdx + colIdxChange[changeIdx], rowIdx + rowIdxChange[changeIdx]));
            changeIdx++;
        }
    }

    public Field getField(int col, int row) {
        if (col <= size && col > 0 && row <= size && row > 0) return this.board[col - 1][row - 1];
        else return null;
    }

    public int getSize() {
        return this.size;
    }

}

