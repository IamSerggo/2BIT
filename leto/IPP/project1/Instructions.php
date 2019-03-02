<?php

require_once ("./Exceptions/LexicalError.php");
require_once ("./InstructionChecker.php");

abstract class Instruction {
    protected $requiredArgs;
    public $opCode;
    public $arg1;
    public $arg2;
    public $arg3;

    public function __construct($lineOfCode) {
        $explodedLine = explode(" ", trim($lineOfCode));

        $givenArgs = sizeof($explodedLine) - 1;

        if ($givenArgs != $this->requiredArgs) {
            throw new LexicalError("Error! Wrong number of arguments!\n");
        }

        $this->opCode = $explodedLine[0];

        if ( $givenArgs >= 1 ) $this->arg1 = $explodedLine[1];
        if ( $givenArgs >= 2 ) $this->arg2 = $explodedLine[2];
        if ( $givenArgs == 3 ) $this->arg3 = $explodedLine[3];
    }
}

class Ins_Move extends Instruction {
    protected $requiredArgs = 2;

    public function __construct($lineOfCode) {
        parent::__construct($lineOfCode);

        $instructionCheck = new InstructionChecker();

        $instructionCheck->checkVariable($this->arg1);
        $instructionCheck->checkSymbol($this->arg2);
    }
}

class Ins_CreateFrame extends Instruction {
    protected $requiredArgs = 0;
}

class Ins_PushFrame extends Instruction {
    protected $requiredArgs = 0;
}

class Ins_PopFrame extends Instruction {
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

class Ins_Pushs extends Instruction {
    protected $requiredArgs = 1;

    public function __construct($lineOfCode) {
        parent::__construct($lineOfCode);

        $instructionCheck = new InstructionChecker();

        $instructionCheck->checkSymbol($this->arg1);
    }
}

class Ins_Pops extends Instruction {
    protected $requiredArgs = 1;

    public function __construct($lineOfCode) {
        parent::__construct($lineOfCode);

        $instructionCheck = new InstructionChecker();

        $instructionCheck->checkVariable($this->arg1);
    }
}

class Ins_Add extends Instruction {
    protected $requiredArgs = 3;

    public function __construct($lineOfCode) {
        parent::__construct($lineOfCode);

        $instructionCheck = new InstructionChecker();

        $instructionCheck->checkVariable($this->arg1);
        $instructionCheck->checkSymbol($this->arg2);
        $instructionCheck->checkSymbol($this->arg3);
    }
}

class Ins_Sub extends Instruction {
    protected $requiredArgs = 3;

    public function __construct($lineOfCode) {
        parent::__construct($lineOfCode);

        $instructionCheck = new InstructionChecker();

        $instructionCheck->checkVariable($this->arg1);
        $instructionCheck->checkSymbol($this->arg2);
        $instructionCheck->checkSymbol($this->arg3);
    }
}

class Ins_Mul extends Instruction {
    protected $requiredArgs = 3;

    public function __construct($lineOfCode) {
        parent::__construct($lineOfCode);

        $instructionCheck = new InstructionChecker();

        $instructionCheck->checkVariable($this->arg1);
        $instructionCheck->checkSymbol($this->arg2);
        $instructionCheck->checkSymbol($this->arg3);
    }
}

class Ins_IDiv extends Instruction {
    protected $requiredArgs = 3;

    public function __construct($lineOfCode) {
        parent::__construct($lineOfCode);

        $instructionCheck = new InstructionChecker();

        $instructionCheck->checkVariable($this->arg1);
        $instructionCheck->checkSymbol($this->arg2);
        $instructionCheck->checkSymbol($this->arg3);
    }
}

class Ins_LT extends Instruction {
    protected $requiredArgs = 3;

    public function __construct($lineOfCode) {
        parent::__construct($lineOfCode);

        $instructionCheck = new InstructionChecker();

        $instructionCheck->checkVariable($this->arg1);
        $instructionCheck->checkSymbol($this->arg2);
        $instructionCheck->checkSymbol($this->arg3);
    }
}

class Ins_GT extends Instruction {
    protected $requiredArgs = 3;

    public function __construct($lineOfCode) {
        parent::__construct($lineOfCode);

        $instructionCheck = new InstructionChecker();

        $instructionCheck->checkVariable($this->arg1);
        $instructionCheck->checkSymbol($this->arg2);
        $instructionCheck->checkSymbol($this->arg3);
    }
}

class Ins_EQ extends Instruction {
    protected $requiredArgs = 3;

    public function __construct($lineOfCode) {
        parent::__construct($lineOfCode);

        $instructionCheck = new InstructionChecker();

        $instructionCheck->checkVariable($this->arg1);
        $instructionCheck->checkSymbol($this->arg2);
        $instructionCheck->checkSymbol($this->arg3);
    }
}

class Ins_And extends Instruction {
    protected $requiredArgs = 3;

    public function __construct($lineOfCode) {
        parent::__construct($lineOfCode);

        $instructionCheck = new InstructionChecker();

        $instructionCheck->checkVariable($this->arg1);
        $instructionCheck->checkSymbol($this->arg2);
        $instructionCheck->checkSymbol($this->arg3);
    }
}

class Ins_Or extends Instruction {
    protected $requiredArgs = 3;

    public function __construct($lineOfCode) {
        parent::__construct($lineOfCode);

        $instructionCheck = new InstructionChecker();

        $instructionCheck->checkVariable($this->arg1);
        $instructionCheck->checkSymbol($this->arg2);
        $instructionCheck->checkSymbol($this->arg3);
    }
}

class Ins_Not extends Instruction {
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

class Ins_Getchar extends Instruction {
    protected $requiredArgs = 3;

    public function __construct($lineOfCode) {
        parent::__construct($lineOfCode);

        $instructionCheck = new InstructionChecker();

        $instructionCheck->checkVariable($this->arg1);
        $instructionCheck->checkSymbol($this->arg2);
        $instructionCheck->checkSymbol($this->arg3);
    }
}

class Ins_Setchar extends Instruction {
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
    }
}

class Ins_Jump extends Instruction {
    protected $requiredArgs = 1;

    public function __construct($lineOfCode) {
        parent::__construct($lineOfCode);

        $instructionCheck = new InstructionChecker();

        $instructionCheck->checkLabel($this->arg1);
    }
}

class Ins_JumpIfEQ extends Instruction {
    protected $requiredArgs = 3;

    public function __construct($lineOfCode) {
        parent::__construct($lineOfCode);

        $instructionCheck = new InstructionChecker();

        $instructionCheck->checkLabel($this->arg1);
        $instructionCheck->checkSymbol($this->arg2);
        $instructionCheck->checkSymbol($this->arg3);
    }
}

class Ins_JumpIfNotEQ extends Instruction {
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