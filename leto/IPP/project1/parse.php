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
require_once("./XMLGenerator.php");

require_once("./InstructionParser.php");

#global variables for STATP extension
$comments = 0;
$LOC = 0;


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
    $parser = new InstructionParser();
    $parsedInstructions = $parser->parseData( readInput() );

    $generator = new XMLGenerator();
    #TODO: dorobit generovanie XML
}
catch (HeaderError $exception) {
    print("Err 21\n");
    exit(21);
}
catch (OPcodeError $exception) {
    print("Err 22\n");
    exit(22);
}
catch (LexSynError $exception) {
    print("Err 23\n");
    exit(23);
}
catch (Exception $exception) {
    print("Error\n");
    exit(99);
}
