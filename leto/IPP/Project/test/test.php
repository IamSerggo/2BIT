<?php

/**
 * Supporting functions
 */
function printHelp(){
	print("This is help!\n");
	exit(0);
}

function getTestsFiles($rootDir, &$targetVar){
	$root = scandir($rootDir);

	foreach($root as $subfile){
		if(is_dir(realpath($rootDir . "/" . $subfile))){
			// Since we don't want to parse current directory and parent directory
			if(!($subfile == "." || $subfile == "..")) getTestsFiles(realpath($rootDir . "/" . $subfile), $targetVar);
		} else {
			// We will make sure that no hidden files are being tested
			if (preg_match("/^\..*$/", $subfile) || !preg_match("/^.*\.(src|in|out|rc)$/", $subfile)) continue;
			
			$subfile = preg_replace("/\.(src|in|out|rc)$/", "", $subfile);
			
			if(!in_array($rootDir . "/" . $subfile, $targetVar)) $targetVar[] = $rootDir . "/" . $subfile;
		}
	}
}

/**
 * Parsing arguments
 */
$args = array(
	"help",
	"directory::",
	"recursive",
	"parse-script::",
	"int-script::",
	"parse-only",
	"int-only"
);

/**
 * Fetching arguments
 */
$prog_args = getopt('', $args);

if(!array_key_exists("directory", $prog_args)) $prog_args["directory"] = "./";
if(!array_key_exists("parse-script", $prog_args)) $prog_args["parse-script"] = "./";
if(!array_key_exists("int-script", $prog_args)) $prog_args["int-script"] = "./";

if(array_key_exists("parse-only", $prog_args) && array_key_exists("int-only", $prog_args)){
	fwrite(STDERR, "Error: Please use either --parse-only or --int-only\n");
	exit(-1);
}

if(array_key_exists("help", $prog_args)) printHelp();

$test_files = [];

getTestsFiles($prog_args['directory'], $test_files);

/**
 * Lets run all tests!!!
 */

$passed = 0;
$total = count($test_files);

foreach($test_files as $test){
	if(array_key_exists("int-script", $prog_args)){
		// Only interpret
		$retVal;
		$output;

		if(!file_exists($test . ".in")) exec("touch {$test}.in");
		if(!file_exists($test . ".out")) exec("touch {$test}.out");
		if(!file_exists($test . ".rc")) exec("touch {$test}.rc && echo 0 > {$test}.rc");

		exec("python3 {$prog_args['int-script']} --source={$test}.src --input={$test}.in 2>errors.out", $output, $retVal);

		$retCodeFile = fopen($test . ".rc", "r");

		$expectedValue = fgets($retCodeFile);

		if($expectedValue == $retVal){
			print("\e[32mOK: \e[0m");
			$passed++;
		} else {
			print("\e[31mWRONG: \e[0m");
		}
		print($test . "\n");

		fclose($retCodeFile);
	
	} else if(array_key_exists("parse-only", $prog_args)){
		// Only parser
	
	} else {
		// Everything
	
	}
}

print("SUMMARY: \nPASSED {$passed}/{$total}\n");
