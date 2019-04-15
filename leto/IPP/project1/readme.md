#HODNOTENIE : 
##parse.php : 8/7
##interpret.py : /13

Implementační dokumentace k 1. úloze do IPP 2018/2019

Jméno a příjmení: Radovan Babic

Login: xbabic09


# parse.php <h1>
Main file of the program. Parse.php is responsible for argument checking and program running.
All exceptions thrown while program is running are handled in a try/catch block.
### Used functions <h3>
* **printHelp():**              prints information about program usage
* **printStats():**             STATP extension - prints statistics into file
* **readInput():**              line by line instruction reading from STDIN
* **runParser():**              function to check program arguments and adequate methods calling


# Instructions.php <h1>
File including all instructions as a single class. All instructions are extended from abstract class Instruction.

## abstract class Instruction <h3>
Parent for all instructions.

### Attributes <h3>
* ***protected* $requiredArgs:**  Required number of arguments, can be values 0,1,2 or 3.
* ***public* $opCode:**           Operation code of instruction.
* ***public* $arg1:**             First argument of instruction (if needed, else *null*).
* ***public* $arg2:**             Second argument of instruction (if needed, else *null*).
* ***public* $arg3:**             Third argument of instruction (if needed, else *null*).

### Methods <h3>
* ***public* __construct():**     Constructor of class. Function will check if given args are correct (number of args and syntax)
                                  and store them into attributes.
* ***private* argToXML():**       Method will transform instruction's argument into XML node for final output XML file.
* ***public* convertToXML():**    Method will transform instruction into XML node for final output XML file.

# InstructionParser.php <h1>

##  class InstructionParser <h3>

### Methods <h3>
* ***public* parseData():**       Method will go through all instructions in foreach cycle and call function *createInstruction*.
                                  Every created instruction is then appended into an array which is after that returned from method.      
* ***private* createInstruction():**    FSM (Final State Machine) implemented as a switch. Each instruction's opCode is checked and in case of 
                                  known and correct opCode, new instance of instruction is created.

# InstructionChecker.php <h1>

##  class InstructionChecker <h3>

Class for checking lexical and syntax correctness of instruction.

### Methods <h3>
* ***public* checkVariable():**   Method to check variable in instruction. Used private method *checkLabel()*.
* ***public* checkSymbol():**     Method to check symbol in instruction. Used methods *checkType()* and *checkConstant()*.
* ***public* checkLabel():**      Method to check label in instruction. Label is checked using regular expression.
* ***public* checkType():**       Method to check type in instruction.
* ***private* checkConstant():**  Method to check constant in instruction. Each type of constant is checked in private methods.
* ***private* checkBool():**      Method to check boolean in instruction. Booleans can be *true* or *false*.
* ***private* checkInt():**       Method to check integer in instruction. Integer is checked using regular expression.
* ***private* checkString():**    Method to check string in instruction. String is checked using regular expression.

# XMLGenerator.php <h1>

##  class XMLGenerator <h3>

Class for generating output XML file

### Methods <h3>
* ***public* generate():**        Generating output XML file. Method will create main body of XML document using instance of DOMDocument class,
                                  then it will create nodes using implemented methods in instruction's instances and append them to main body.
                                  Output file is generated to STDOUT.
