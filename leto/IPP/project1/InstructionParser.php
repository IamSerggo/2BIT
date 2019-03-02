<?php

require_once("./Instructions.php");

class InstructionParser{

    public function parseData($input){
        $parsedInstructions = array();

        foreach ($input as $instruction){
            array_push($parsedInstructions, $this->createInstruction($instruction));
        }

        return $parsedInstructions;
    }

    public function createInstruction($instruction){
        $opCode = strtoupper( explode(" ", trim($instruction))[0] );

        switch ($opCode){
            case "MOVE":
                return new Ins_Move($instruction);
            case "CREATEFRAME":
                return new Ins_CreateFrame($instruction);
            case "PUSHFRAME":
                return new Ins_PushFrame($instruction);
            case "POPFRAME":
                return new Ins_PopFrame($instruction);
            case "DEFVAR":
                return new Ins_DefVar($instruction);
            case "CALL":
                return new Ins_Call($instruction);
            case "RETURN":
                return new Ins_Return($instruction);
            case "PUSHS":
                return new Ins_Pushs($instruction);
            case "POPS":
                return new Ins_Pops($instruction);
            case "ADD":
                return new Ins_Add($instruction);
            case "SUB":
                return new Ins_Sub($instruction);
            case "MUL":
                return new Ins_Mul($instruction);
            case "IDIV":
                return new Ins_IDiv($instruction);
            case "LT":
                return new Ins_LT($instruction);
            case "GT":
                return new Ins_GT($instruction);
            case "EQ":
                return new Ins_EQ($instruction);
            case "AND":
                return new Ins_And($instruction);
            case "OR":
                return new Ins_Or($instruction);
            case "NOT":
                return new Ins_Not($instruction);
            case "INT2CHAR":
                return new Ins_Int2Char($instruction);
            case "STRI2INT":
                return new Ins_Stri2Int($instruction);
            case "READ":
                return new Ins_Read($instruction);
            case "WRITE":
                return new Ins_Write($instruction);
            case "CONCAT":
                return new Ins_Concat($instruction);
            case "STRLEN":
                return new Ins_Strlen($instruction);
            case "GETCHAR":
                return new Ins_Getchar($instruction);
            case "SETCHAR":
                return new Ins_Setchar($instruction);
            case "TYPE":
                return new Ins_Type($instruction);
            case "LABEL":
                return new Ins_Label($instruction);
            case "JUMP":
                return new Ins_Jump($instruction);
            case "JUMPIFEQ":
                return new Ins_JumpIfEQ($instruction);
            case "JUMPIFNEQ":
                return new Ins_JumpIfNotEQ($instruction);
            case "EXIT":
                return new Ins_Exit($instruction);
            case "DPRINT":
                return new Ins_Dprint($instruction);
            case "BREAK":
                return new Ins_Break($instruction);
            default:
                throw new OPcodeError("Unknown OP Code!\n");
        }
    }
}