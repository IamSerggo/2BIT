<?php
/**
 * Project: First part of project for IPP
 * File: InstructionParser.php
 * Author: Radovan Babic, xbabic09
 * Date: 2.3.2019
 * School: VUT FIT, 2BIT
 */

require_once("./Instructions.php");

/**
 * Class for parsing instrucions into array
 *
 * One big switch to check if given instruction is supported - otherwise throw exception
 */
class InstructionParser{

    /**
        * Parsing instructions into array
        *
        * @param string $input - Line of code (instruction)
        *
        * @return array $parsedInstructions - Array of parsed instructions
        */
    public function parseData($input){
        $parsedInstructions = array();

        foreach ($input as $instruction){
            array_push($parsedInstructions, $this->createInstruction($instruction));
        }

        return $parsedInstructions;
    }

    /**
        * Checking if instruction is supported
        *
        * @param string $instruction - Instruction to be checked
        *
        * @return Instruction - New instantion of instruction
        */
    private function createInstruction($instruction){
        $opCode = strtoupper( explode(" ", trim($instruction))[0] );

        switch ($opCode){
            case "MOVE":
                return new Ins_Move($instruction);

            case "CREATEFRAME":
            case "PUSHFRAME":
            case "POPFRAME":
                return new Ins_Frame($instruction);

            case "DEFVAR":
                return new Ins_DefVar($instruction);

            case "CALL":
                return new Ins_Call($instruction);

            case "RETURN":
                return new Ins_Return($instruction);

            case "PUSHS":
            case "POPS":
                return new Ins_Stack($instruction);

            case "ADD":
            case "SUB":
            case "MUL":
            case "IDIV":
                return new Ins_MathOp($instruction);

            case "LT":
            case "GT":
            case "EQ":
            case "AND":
            case "OR":
            case "NOT":
                return new Ins_LogOp($instruction);

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
            case "SETCHAR":
                return new Ins_Char($instruction);

            case "TYPE":
                return new Ins_Type($instruction);

            case "LABEL":
                return new Ins_Label($instruction);

            case "JUMP":
                return new Ins_Jump($instruction);


            case "JUMPIFEQ":
            case "JUMPIFNEQ":
                return new Ins_CondJump($instruction);

            case "EXIT":
                return new Ins_Exit($instruction);

            case "DPRINT":
                return new Ins_Dprint($instruction);

            case "BREAK":
                return new Ins_Break($instruction);

            default:
                throw new OPcodeError("Unknown OP Code!\n");
                break;
        }
    }
}
