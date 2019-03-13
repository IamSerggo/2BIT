<?php
/**
 * Project: First part of project for IPP
 * File: InstructionChecker.php
 * Author: Radovan Babic, xbabic09
 * Date: 2.3.2019
 * School: VUT FIT, 2BIT
 */

 /**
  * Class for checking lexical and sytax correcntess of instructions
  */
class InstructionChecker {

  /**
    * This method checks correcntess of variable
    *
    * @param string $input - Variable for check
    *
    * @return void - Function is throwing exceptions in case of invalid variable
    */
    public function checkVariable($input){
        if ( count($explodedInput = explode("@", $input)) != 2 ) {
            throw new LexSynError("Error! Wrong frame syntax!\n");
        }

        $frame = $explodedInput[0];
        $varName = $explodedInput[1];

        if ( $frame != 'LF' and $frame != 'GF' and $frame != 'TF' ) {
            throw new LexSynError("Error! Wrong frame syntax!\n");
        }

        $this->checkLabel($varName);
    }

    /**
      * This method checks correcntess of symbol
      *
      * @param string $input - Symbol for check
      *
      * @return void - Function is throwing exceptions in case of invalid symbol
      */
    public function checkSymbol($input){
        try { $this->checkVariable($input); }
        catch (LexSynError $exception) {

            if ( count($explodedInput = explode("@", $input)) != 2 ) {
                throw new LexSynError("Error! Wrong symbol syntax!\n");
            }

            $type = $explodedInput[0];
            $constant = $explodedInput[1];

            $this->checkType($type);
            $this->checkConstant($type, $constant);
        }
    }

    /**
      * This method checks correcntess of label
      *
      * @param string $input - Label for check
      *
      * @return void - Function is throwing exceptions in case of invalid label
      */
    public function checkLabel($input){
        $match = preg_match("/^([a-zA-Z]|-|[_$&%*!?])([a-zA-Z]|-|[_$&%*!?]|[0-9]+)*$/", $input);

        if ( $match === 0 ) throw new LexSynError("Error! Bad variable/label name!\n");
    }

    /**
      * This method checks correcntess of type
      *
      * @param string $input - Type for check
      *
      * @return void - Function is throwing exceptions in case of invalid type
      */
    #TODO : sem este musim zistit ze kedy to moze byt nil a kedy nemoze
    public function checkType($input){
        if ( $input != 'int' and $input != 'bool' and $input != 'string' and $input != 'nil' ) {
            throw new LexSynError("Error! Wrong type selected!\n");
        }
    }

    /**
      * This method checks correcntess of constant
      *
      * @param string $type - Type of constant
      * @param string $constant - Constant for check
      *
      * @return void - Function is throwing exceptions in case of invalid constant
      */
    private function checkConstant($type, $constant){
        switch ($type){
            case "bool":
                $this->checkBool($constant);
                break;

            case "int":
                $this->checkInt($constant);
                break;

            case "string":
                $this->checkString($constant);
                break;

            case "nil":
                break;

            default:
                throw new LexSynError("Error! Unknown type!n");
                break;
        }
    }

    /**
      * This method checks correcntess of boolean
      *
      * @param string $bool - Boolean for check
      *
      * @return void - Function is throwing exceptions in case of invalid boolean
      */
    private function checkBool($bool){
        if ($bool != 'true' and $bool != 'false'){
            throw new LexSynError("Error! Wrong boolean value!\n");
        }
    }

    /**
      * This method checks correcntess of integer
      *
      * @param string $int - Integer for check
      *
      * @return void - Function is throwing exceptions in case of invalid integer
      */
    private function checkInt($int){
        $match = preg_match("/^[\x2B\x2D]?[0-9]*$/", $int);

        if ( $match === 0) throw new LexSynError("Error! Wrong int format!\n");
    }

    /**
      * This method checks correcntess of string
      *
      * @param string $string - String for check
      *
      * @return void - Function is throwing exceptions in case of invalid string
      */
    private function checkString($string){
        $match = preg_match("/^([^@\\\\]*([\\\\]\d{3})*)*$/", $string);

        if ( $match === 0 ) throw new LexSynError("Error! Wrong string format!\n");
        #([a-zA-Z0-9!--?|(\\[0-9]{3}])*
        #^([a-zA-Z\x{0021}\x{0022}\x{0024}-\x{005B}\x{005D}-\x{FFFF}|(\\\\[0-90-90-9])*$
    }
}
