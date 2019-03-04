<?php
/**
 * File: First part of project for IPP
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

function printHelp(){
    echo("Tu sa vypise help\n");
}

function printStats($argv, $outputFile){
    #$actualDir = dirname(__FILE__);

    $file = fopen($outputFile, "w");

    if ( $file === false ) throw new Exception("Error while file opening!\n");

    for ($i = 2; $i <= 5; $i++){
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

function runProgram($argv, $argc){
    if ( $argc == 1 ) {
        $parser = new InstructionParser();
        return $parser->parseData( readInput() );
    }
    if ( $argc == 2 and $argv[1] == "--help" ) {
        printHelp();
    }
    elseif ( $argc == 6 and strpos($argv[1], "--stats=") !== false ) {
        $outputFile = explode("=", $argv[1])[1];

        $parser = new InstructionParser();
        $parsedInstructions = $parser->parseData( readInput() );

        printStats($argv, $outputFile);

        return $parsedInstructions;

    }
    else throw new ProgramArgsError("Error! Wrong program arguments inserted!\n");

}

function readInput(){
    $input = fopen('php://stdin', 'r');
    $firstLine = strtolower( fgets($input) );

    if ( trim($firstLine) != ".ippcode19" ){
        throw new HeaderError("Error! Code doesn't starts with '.ippcode19' !!");
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

try {
    $parsedInstructions = runProgram($argv, $argc);

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

