<?php


class InstructionChecker {
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

    public function checkLabel($input){
        $match = preg_match("/^([a-zA-Z]|-|[_$&%*!?])([a-zA-Z]|-|[_$&%*!?]|[0-9]+)*$/", $input);

        if ( $match === 0 ) throw new LexSynError("Error! Bad variable/label name!\n");
    }

    #TODO : sem este musim zistit ze kedy to moze byt nil a kedy nemoze
    public function checkType($input){
        if ( $input != 'int' and $input != 'bool' and $input != 'string' and $input != 'nil' ) {
            throw new LexSynError("Error! Wrong type selected!\n");
        }
    }

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

    private function checkBool($bool){
        if ($bool != 'true' and $bool != 'false'){
            throw new LexSynError("Error! Wrong boolean value!\n");
        }
    }

    private function checkInt($int){
        $match = preg_match("/^[\x2B\x2D]?[0-9]*$/", $int);

        if ( $match === 0) throw new LexSynError("Error! Wrong int format!\n");
    }

    private function checkString($string){
        $match = preg_match("/^([a-zA-Z\x{0021}\x{0022}\x{0024}-\x{005B}\x{005D}-\x{FFFF}|(\\\\[0-90-90-9])*$/u",
                            $string);

        if ( $match === 0 ) throw new Exception("Error! Wrong string format!\n");
    }
}