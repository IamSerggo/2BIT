<?php
/**
 * Project: First part of project for IPP
 * File: parse.php
 * Author: Radovan Babic, xbabic09
 * Date: 2.3.2019
 * School: VUT FIT, 2BIT
 */

require_once("./Exceptions/HeaderError.php");
require_once("./Exceptions/LexSynError.php");
require_once("./Exceptions/OPcodeError.php");
require_once("./Exceptions/ProgramArgsError.php");
require_once("./Exceptions/StatsParamError.php");

require_once("./XMLGenerator.php");
require_once("./InstructionParser.php");

#global variables for STATP extension
$comments = 0;
$LOC = 0;
$labels = 0;
$jumps = 0;

/**
      * This method prints help for program usage
      *
      * @param void
      *
      * @return void
      */
function printHelp(){
    echo("**Parser for non-structured imperative language IPPcode19**\n");
    echo("Supported program arguments: --help, --stats\n");
    echo("--help : shows this help menu, can't be combined with other arguments\n");
    echo("--stats : this argument must be in format --stats=file and it takes 4 optional arguments\n");
    echo("**OPTIONAL ARGUMENTS**\n");
    echo("--loc : lines of code\n");
    echo("--comments : amount of comments\n");
    echo("--labels : amount of labels\n");
    echo("--jumps : amount of jumps\n");
    echo("Any of these 4 arguments without --stats argument are invalid!\n");
    exit(0);
}

/**
      * This method prints statistics to file chosen by argument --stats
      *
      * Implementation of STATP extension
      *
      * @param array $argv - Array of arguments
      * @param string $outputFile - Path to output file
      * @param int $statsIndex - Index to the array of arguments where --stats argument is saved
      *
      * @return void
      */
function printStats($argv, $outputFile, $statsIndex){
    #$actualDir = dirname(__FILE__);

    $file = fopen($outputFile, "w");

    if ( $file === false ) throw new Exception("Error while file opening!\n");

    for ($i = 1; $i <= (count($argv)-1); $i++){
        if ( $i == $statsIndex ) continue;
        switch ($argv[$i]){
            case "--loc":
                fwrite($file, $GLOBALS['LOC']."\n");
                break;
            case "--comments":
                fwrite($file, $GLOBALS['comments']."\n");
                break;
            case "--labels":
                fwrite($file, $GLOBALS['labels']."\n");
                break;
            case "--jumps":
                fwrite($file, $GLOBALS['jumps']."\n");
                break;
            default:
                throw new ProgramArgsError("Error! Wrong program arguments inserted!\n");
        }
    }
    fclose($file);
}

/**
      * This method reads input from STDIN line by line
      *
      * Comments and header correctness check included
      *
      * @param void
      *
      * @return string $loadedLine - yielding lines for further analysis
      */
function readInput(){
    $input = fopen('php://stdin', 'r');

    $firstLine = strtolower( fgets($input) );

    if ( trim($firstLine) != ".ippcode19" ){
        if ( trim(explode("#", $firstLine)[0]) != ".ippcode19" ) {
            throw new HeaderError("Error! Code doesn't starts with '.IPPcode19' !!");
        }
        else $GLOBALS['comments']++;
    }

    while ( $loadedLine = fgets($input) ){
        $commentPosition = strpos($loadedLine, "#");

        if ( $commentPosition !== false ) {
            $GLOBALS['comments']++;

            if ( $commentPosition === 0 ) continue;
            else $loadedLine = explode("#", $loadedLine)[0];
        }

        $loadedLine = preg_replace('![\s\t]+!', ' ', $loadedLine);

        if ( strlen($loadedLine) <= 1 ) continue;

        $GLOBALS['LOC']++;

        yield $loadedLine;
    }
}

/**
      * This method runs whole program
      *
      * Method will check correcntess of given arguments and will call adequate methods
      * Method is called in try-catch block to catch Exceptions and return relevant code
      *
      * @param array $argv - Array of arguments
      * @param int $argc - Number of given arguments
      *
      * @return array $parsedInstructions - Array of parsed instructions (objects extended from abstract class Instruction)
      */
function runParser($argv, $argc){
    if ( $argc == 1 ) {
        $parser = new InstructionParser();
        return $parser->parseData( readInput() );
    }
    elseif ( $argc == 2 and $argv[1] == "--help" ) {
        printHelp();
    }
    else {
        $index = 0;
        $statsPresent = false;

        foreach ($argv as $argument) {
            if ( strpos($argument, "--stats=") !== false ){
                $statsPresent = true;
                break;
            }
            $index++;
        }

        if ( $statsPresent === false ) throw new ProgramArgsError("Error! Wrong program arguments inserted!\n");
        else {
            $outputFile = explode("=", $argv[$index])[1];

            $parser = new InstructionParser();
            $parsedInstructions = $parser->parseData( readInput() );

            printStats($argv, $outputFile, $index);

            return $parsedInstructions;
        }
    }
}

try {
    $parsedInstructions = runParser($argv, $argc);

    $generator = new XMLGenerator();
    $generator->generate($parsedInstructions);
}
catch (ProgramArgsError $exception) {
    exit(10);
}
catch (HeaderError $exception) {
    exit(21);
}
catch (OPcodeError $exception) {
    exit(22);
}
catch (LexSynError $exception) {
    exit(23);
}
catch (Exception $exception) {
    exit(99);
}
