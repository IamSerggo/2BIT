<?php

class XMLGenerator {

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