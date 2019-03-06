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
    exit(0);
}

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

function readInput(){
    $input = fopen('php://stdin', 'r');

    while (true){
        $firstLine = strtolower( fgets($input) );

        if ( trim($firstLine) != ".ippcode19" ){

            if ( strpos($firstLine, "#") !== false ) {
                if ( trim(explode("#", $firstLine)[0]) == ".ippcode19" ) break;
                elseif ( strpos($firstLine, "#") === 0 ) continue;
                else throw new HeaderError("Error! Code doesn't starts with '.ippcode19' !!");
            }
            else throw new HeaderError("Error! Code doesn't starts with '.ippcode19' !!");
        }
        else break;
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

