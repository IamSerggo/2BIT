<?php
/**
 * Project: First part of project for IPP
 * File: Instructions.php
 * Author: Radovan Babic, xbabic09
 * Date: 2.3.2019
 * School: VUT FIT, 2BIT
 */

require_once("./Exceptions/LexSynError.php");
require_once ("./InstructionChecker.php");

/**
 * Abstract class for generating instructions
 *
 * @var int $requiredArgs - Nubmer of arguments required by instruction
 * @var string $opCode - Operation code of instruction
 * @var string|null $arg1 - First arguemnt of instruction
 * @var string|null $arg2 - Second arguemnt of instruction
 * @var string|null $arg3 - Third arguemnt of instruction
 */
abstract class Instruction {

    protected $requiredArgs;
    public $opCode;
    public $arg1;
    public $arg2;
    public $arg3;

    /**
          * Constructor for instructions
          *
          * Method will check if given arguments are correct
          * All given information will be saved and instation of instruction will be created
          *
          * @param string $lineOfCode - Line of code (instruction) to be transformed into object
          *
          * @return void
          */
    public function __construct($lineOfCode) {
        $explodedLine = explode(" ", trim($lineOfCode));

        $givenArgs = sizeof($explodedLine) - 1;

        if ($givenArgs != $this->requiredArgs) {
            throw new LexSynError("Error! Wrong number of arguments!\n");
        }

        $this->opCode = strtoupper($explodedLine[0]);

        if ( $givenArgs >= 1 ) $this->arg1 = $explodedLine[1];
        if ( $givenArgs >= 2 ) $this->arg2 = $explodedLine[2];
        if ( $givenArgs == 3 ) $this->arg3 = $explodedLine[3];
    }

    /**
          * Converting instruction's argument into XML nodes
          *
          * @param DOMDocument $xml - Instation of DOMDocument to which node will be appended
          * @param string $argNum - Number of argument (arg1, arg2, arg3)
          * @param string $arg - Argument to be converted into XML node
          *
          * @return DOMElement $argNode - Argument converted to XML node
          */
    private function argToXML($xml, $argNum, $arg){
        $argNode = $xml->createElement($argNum);
        $explodedArg = explode("@", $arg);

        if ( count($explodedArg) != 2 ) {
            $type = "label";
            $text = $arg;
        }
        elseif ( $explodedArg[0] == 'GF' or $explodedArg[0] == 'TF' or $explodedArg[0] == 'LF') {
            $type = "var";
            $text = $arg;
        }
        else {
            $type = $explodedArg[0];
            $text = $explodedArg[1];
            if ( $type == "string" ) $text = str_replace(array("&", "<", ">"), array("&amp", "&lt", "&gt"), $text);
        }

        $argNode->setAttribute("type", $type);
        $argNode->textContent = $text;

        return $argNode;
    }

    /**
          * Converting instruction into XML node
          *
          * Method is using argToXML method for arguments conversion into XML nodes
          *
          * @param DOMDocument $xml - Instation of DOMDocument to which nodes will be appended
          * @param int $orderNumber - Keeping order of instructions
          *
          * @return DOMElement $node - Instruction converted to XML node
          */
    public function convertToXML($xml, $orderNumber){
        $node = $xml->createElement("instruction");
        $node->setAttribute("order", $orderNumber);
        $node->setAttribute("opcode", $this->opCode);

        if ( $this->requiredArgs >= 1 ) {
            $arg1Node = $this->argToXML($xml, "arg1", $this->arg1);
            $node->appendChild($arg1Node);
        }

        if ( $this->requiredArgs >= 2 ) {
            $arg2Node = $this->argToXML($xml, "arg2", $this->arg2);
            $node->appendChild($arg2Node);
        }

        if ( $this->requiredArgs == 3) {
            $arg3Node = $this->argToXML($xml, "arg3", $this->arg3);
            $node->appendChild($arg3Node);
        }

        return $node;
    }
}

/**
 * Classes for each instruction
 *
 * Each instruciton has it's own nubmer of required arguments
 * Constructors are calling methods from InstructionChecker to check Lex and Syn correctness
 */
class Ins_Move extends Instruction {
    protected $requiredArgs = 2;

    public function __construct($lineOfCode) {
        parent::__construct($lineOfCode);

        $instructionCheck = new InstructionChecker();

        $instructionCheck->checkVariable($this->arg1);
        $instructionCheck->checkSymbol($this->arg2);
    }
}

class Ins_Frame extends Instruction {
    protected $requiredArgs = 0;
}


class Ins_DefVar extends Instruction {
    protected $requiredArgs = 1;

    public function __construct($lineOfCode) {
        parent::__construct($lineOfCode);

        $instructionCheck = new InstructionChecker();

        $instructionCheck->checkVariable($this->arg1);
    }
}

class Ins_Call extends Instruction {
    protected $requiredArgs = 1;

    public function __construct($lineOfCode) {
        parent::__construct($lineOfCode);

        $instructionCheck = new InstructionChecker();

        $instructionCheck->checkLabel($this->arg1);
    }
}

class Ins_Return extends Instruction {
    protected $requiredArgs = 0;
}

class Ins_Stack extends Instruction {
    protected $requiredArgs = 1;

    public function __construct($lineOfCode) {
        parent::__construct($lineOfCode);

        $instructionCheck = new InstructionChecker();

        $instructionCheck->checkSymbol($this->arg1);
    }
}

class Ins_MathOp extends Instruction {
    protected $requiredArgs = 3;

    public function __construct($lineOfCode) {
        parent::__construct($lineOfCode);

        $instructionCheck = new InstructionChecker();

        $instructionCheck->checkVariable($this->arg1);
        $instructionCheck->checkSymbol($this->arg2);
        $instructionCheck->checkSymbol($this->arg3);
    }
}


class Ins_LogOp extends Instruction {
    protected $requiredArgs = 3;

    public function __construct($lineOfCode) {
        parent::__construct($lineOfCode);

        $instructionCheck = new InstructionChecker();

        $instructionCheck->checkVariable($this->arg1);
        $instructionCheck->checkSymbol($this->arg2);
        $instructionCheck->checkSymbol($this->arg3);
    }
}


class Ins_Int2Char extends Instruction {
    protected $requiredArgs = 2;

    public function __construct($lineOfCode) {
        parent::__construct($lineOfCode);

        $instructionCheck = new InstructionChecker();

        $instructionCheck->checkVariable($this->arg1);
        $instructionCheck->checkSymbol($this->arg2);
    }
}

class Ins_Stri2Int extends Instruction {
    protected $requiredArgs = 3;

    public function __construct($lineOfCode) {
        parent::__construct($lineOfCode);

        $instructionCheck = new InstructionChecker();

        $instructionCheck->checkVariable($this->arg1);
        $instructionCheck->checkSymbol($this->arg2);
        $instructionCheck->checkSymbol($this->arg3);
    }
}

class Ins_Read extends Instruction {
    protected $requiredArgs = 2;

    public function __construct($lineOfCode) {
        parent::__construct($lineOfCode);

        $instructionCheck = new InstructionChecker();

        $instructionCheck->checkVariable($this->arg1);
        $instructionCheck->checkType($this->arg2);
    }
}

class Ins_Write extends Instruction {
    protected $requiredArgs = 1;

    public function __construct($lineOfCode) {
        parent::__construct($lineOfCode);

        $instructionCheck = new InstructionChecker();

        $instructionCheck->checkSymbol($this->arg1);
    }
}

class Ins_Concat extends Instruction {
    protected $requiredArgs = 3;

    public function __construct($lineOfCode) {
        parent::__construct($lineOfCode);

        $instructionCheck = new InstructionChecker();

        $instructionCheck->checkVariable($this->arg1);
        $instructionCheck->checkSymbol($this->arg2);
        $instructionCheck->checkSymbol($this->arg3);
    }
}

class Ins_Strlen extends Instruction {
    protected $requiredArgs = 2;

    public function __construct($lineOfCode) {
        parent::__construct($lineOfCode);

        $instructionCheck = new InstructionChecker();

        $instructionCheck->checkVariable($this->arg1);
        $instructionCheck->checkSymbol($this->arg2);
    }
}

class Ins_Char extends Instruction {
    protected $requiredArgs = 3;

    public function __construct($lineOfCode) {
        parent::__construct($lineOfCode);

        $instructionCheck = new InstructionChecker();

        $instructionCheck->checkVariable($this->arg1);
        $instructionCheck->checkSymbol($this->arg2);
        $instructionCheck->checkSymbol($this->arg3);
    }
}


class Ins_Type extends Instruction {
    protected $requiredArgs = 2;

    public function __construct($lineOfCode) {
        parent::__construct($lineOfCode);

        $instructionCheck = new InstructionChecker();

        $instructionCheck->checkVariable($this->arg1);
        $instructionCheck->checkSymbol($this->arg2);
    }
}

class Ins_Label extends Instruction {
    protected $requiredArgs = 1;

    public function __construct($lineOfCode) {
        parent::__construct($lineOfCode);

        $instructionCheck = new InstructionChecker();

        $instructionCheck->checkLabel($this->arg1);

        $GLOBALS['labels']++;
    }
}

class Ins_Jump extends Instruction {
    protected $requiredArgs = 1;

    public function __construct($lineOfCode) {
        parent::__construct($lineOfCode);

        $instructionCheck = new InstructionChecker();

        $instructionCheck->checkLabel($this->arg1);

        $GLOBALS['jumps']++;
    }
}

class Ins_CondJump extends Instruction {
    protected $requiredArgs = 3;

    public function __construct($lineOfCode) {
        parent::__construct($lineOfCode);

        $instructionCheck = new InstructionChecker();

        $instructionCheck->checkLabel($this->arg1);
        $instructionCheck->checkSymbol($this->arg2);
        $instructionCheck->checkSymbol($this->arg3);
    }
}

class Ins_Exit extends Instruction {
    protected $requiredArgs = 1;

    public function __construct($lineOfCode) {
        parent::__construct($lineOfCode);

        $instructionCheck = new InstructionChecker();

        $instructionCheck->checkSymbol($this->arg1);
    }
}

class Ins_Dprint extends Instruction {
    protected $requiredArgs = 1;

    public function __construct($lineOfCode) {
        parent::__construct($lineOfCode);

        $instructionCheck = new InstructionChecker();

        $instructionCheck->checkSymbol($this->arg1);
    }
}

class Ins_Break extends Instruction {
    protected $requiredArgs = 0;
}
