<?php
/**
 * Project: First part of project for IPP
 * File: XMLGenerator.php
 * Author: Radovan Babic, xbabic09
 * Date: 2.3.2019
 * School: VUT FIT, 2BIT
 */

 /**
  * Class for generating output XML file
  *
  * Crating new DOMDocument object instantion
  * Each instruction has it's own methods to convert it to XMLnode
  * This class will use these methods and then connect nodes into XML
  */
class XMLGenerator {

  /**
    * This method sets a description.
    *
    * @param array $parsedInstructions - array of instructions parsed using InstructionParser
    *
    * @return void - output of this method is XML file written to STDOUT
    */
    public function generate($parsedInstructions){
        $xml = new DOMDocument('1.0', 'utf-8');

        $xmlRoot = $xml->createElement("program");
        $xmlRoot->setAttribute("language", "IPPcode19");

        $counter = 1;
        foreach ( $parsedInstructions as $instruction ){
            $instructionNode = $instruction->convertToXML($xml, $counter++);

            $xmlRoot->appendChild($instructionNode);
        }

        $xml->appendChild($xmlRoot);

        fwrite(STDOUT, $xml->saveXML());
    }
}
