# Project: Second part of project for IPP
# File: interpret.py
# Author: Radovan Babic, xbabic09
# Date: 8.4.2019
# School: VUT FIT, 2BIT

#\/ ================ IMPORTS ================ \/#
import xml.etree.ElementTree as XMLparser
import sys

#\/ ================ ATTRIBUTES ================ \/#
allowedInstructions = ['MOVE', 'CREATEFRAME', 'PUSHFRAME', 'POPFRAME', 'DEFVAR',
                       'CALL', 'RETURN', 'PUSHS', 'POPS', 'ADD', 'SUB', 'MUL',
                       'IDIV', 'LT', 'GT', 'EQ', 'AND', 'OR', 'NOT', 'INT2CHAR',
                       'STRI2INT', 'READ', 'WRITE', 'CONCAT', 'STRLEN', 'GETCHAR',
                       'SETCHAR', 'TYPE', 'LABEL', 'JUMP', 'JUMPIFEQ', 'JUMPIFNEQ',
                       'EXIT', 'DPRINT', 'BREAK']
parsedInstructions = []
globalFrame = {}
labels = {}

#localFrame = []
#isTempFrameCreated = False
#tempFrame = []


#\/ ================ CLASSES ================ \/#
class Instructions ():
    @staticmethod
    def __replaceSpecialCharacters(what):
        replaced = what.replace("\\032", " ")
        replaced = replaced.replace("\\092", "\\")
        replaced = replaced.replace("\\010", "\n")
        replaced = replaced.replace("\\035", "#")
        return replaced

    @staticmethod
    def __writeFromFrame(what):
        for arg in what:
            split = arg.text.split("@")
            if split[0] != "GF":
                continue

            else:
                if not isVariableDefined(globalFrame, split[-1]):
                    sys.exit(52)
                else:
                    stuffToPrint = globalFrame.get(split[-1])
                    if stuffToPrint == "Uninitialized":
                        sys.exit(56)
                    else:
                        stuffToPrint = Instructions.__replaceSpecialCharacters(stuffToPrint)
                        print(stuffToPrint, end='')

    @staticmethod
    def __parseArg(arg):
        type = arg.attrib['type']
        if type == "string":
            compare = arg.text

        elif type == "int":
            compare = int(arg.text)

        elif type == "bool":
            if arg.text == "false":
                compare = False
            else:
                compare = True

        elif type == "var":
            argument = arg.text.split("@")
            if argument[0] != "GF":
                return
            else:
                compare = globalFrame.get(argument[-1])
                if compare == None:
                    sys.exit(52)

            try:
                compare = int(compare)
            except ValueError:
                return compare

        return compare

    def WRITE(what):
        allowedForWrite = ["int", "bool", "string"]
        for arg in what:
            if arg.attrib['type'] == "var":
                Instructions.__writeFromFrame(what)
            else:
                if arg.attrib['type'] not in allowedForWrite:
                    sys.exit(53)
                else:
                    textToWrite = Instructions.__replaceSpecialCharacters(arg.text)
                    print(textToWrite, end='')

    def MOVE(what, where):
        if what.attrib['type'] == "var":
            splitWhat = what.text.split("@")
            if splitWhat[0] != "GF":
                return
            else:
                if not isVariableDefined(globalFrame, splitWhat[-1]):
                    sys.exit(52)
                else:
                    what = globalFrame[splitWhat[-1]]
        else:
            what = what.text

        splitWhere = where.text.split("@")

        if splitWhere[0] != "GF":
            return
        else:
            if not isVariableDefined(globalFrame, splitWhere[-1]):
                sys.exit(52)
            else:
                globalFrame[splitWhere[-1]] = what

    def LABEL(label):
        for arg in label: #TODO dorobit checky ze ci tam nie je viac arg a tak
            if arg.attrib['type'] != "label":
                sys.exit(32)

            labels[arg.text] = label.attrib['order']

    def JUMP(where):
        newIP = 0
        for arg in where: #TODO dorobit checky ze ci tam nie je viac arg a tak
            if arg.attrib['type'] != "label":
                sys.exit(32)

            newIP = labels.get(arg.text)
            if newIP == None:
                sys.exit(52)

        return int(newIP)

    def JUMPIFEQ(where):
        newIP = 0
        compare1 = compare2 = None

        for arg in where: #TODO dorobit checky ze ci tam nie je viac arg a tak
            if arg.tag == "arg1":
                if arg.attrib['type'] != "label":
                    sys.exit(32)

                newIP = labels.get(arg.text)
                if newIP == None:
                    sys.exit(52)

            elif arg.tag == "arg2":
                compare1 = Instructions.__parseArg(arg)

            elif arg.tag == "arg3":
                compare2 = Instructions.__parseArg(arg)

        if type(compare1) != type(compare2):
            sys.exit(53)

        if compare1 != compare2:
            return None

        return int(newIP)

    def JUMPIFNEQ(where):
        newIP = 0
        compare1 = compare2 = None

        for arg in where: #TODO dorobit checky ze ci tam nie je viac arg a tak
            if arg.tag == "arg1":
                if arg.attrib['type'] != "label":
                    sys.exit(32)

                newIP = labels.get(arg.text)
                if newIP == None:
                    sys.exit(52)

            elif arg.tag == "arg2":
                compare1 = Instructions.__parseArg(arg)

            elif arg.tag == "arg3":
                compare2 = Instructions.__parseArg(arg)

        if type(compare1) != type(compare2):
            sys.exit(53)

        if compare1 == compare2:
            return None

        return int(newIP)

    def EXIT(code):
        returnCode = 0
        for arg in code:
            if arg.attrib['type'] == "int":
                if int(arg.text.split("@")[-1]) not in range(-1, 50):
                    sys.exit(57)

                return int(arg.text.split("@")[-1])

            elif arg.attrib['type'] == "var":
                split = arg.text.split("@")
                if split[0] != "GF":
                    sys.exit(-1)

                else:
                    returnCode = globalFrame.get(split[-1])
                    if returnCode == None:
                        sys.exit(52)
                    else:
                        try:
                            returnCode = int(returnCode)
                        except ValueError:
                            sys.exit(53)

                    if returnCode not in range(-1, 50):
                        sys.exit(57)
            else:
                sys.exit(32)

        return returnCode

    def ADD(instruction):
        for arg in instruction:
            if arg.tag == "arg1":
                if arg.attrib['type'] != "var":
                    sys.exit(32)

                split = arg.text.split("@")
                if split[0] != "GF":
                    print("Not implemented")
                    sys.exit(-1)

                dest = split[-1]
                if globalFrame.get(dest) == None:
                    sys.exit(52)

            elif arg.tag == "arg2":
                split = arg.text.split("@")

                if arg.attrib['type'] == "var":
                    if split[0] != "GF":
                        print("Not implemented")
                        sys.exit(-1)

                    value1 = globalFrame.get(split[-1])
                    if value1 == None:
                        sys.exit(52)

                    try:
                        value1 = int(value1)
                    except ValueError:
                        sys.exit(53)

                elif arg.attrib['type'] == "int":
                    try:
                        value1 = int(split[-1])
                    except ValueError:
                        sys.exit(53)

                else:
                    sys.exit(32)

            elif arg.tag == "arg3":
                split = arg.text.split("@")

                if arg.attrib['type'] == "var":
                    if split[0] != "GF":
                        print("Not implemented")
                        sys.exit(-1)

                    value2 = globalFrame.get(split[-1])
                    if value2 == None:
                        sys.exit(52)

                    try:
                        value2 = int(value1)
                    except ValueError:
                        sys.exit(53)

                elif arg.attrib['type'] == "int":
                    try:
                        value2 = int(split[-1])
                    except ValueError:
                        sys.exit(53)

                else:
                    sys.exit(23)

        globalFrame[dest] = str(value1 + value2)

    def SUB(instruction):
        for arg in instruction:
            if arg.tag == "arg1":
                if arg.attrib['type'] != "var":
                    sys.exit(23)

                split = arg.text.split("@")
                if split[0] != "GF":
                    print("Not implemented")
                    sys.exit(-1)

                dest = split[-1]
                if globalFrame.get(dest) == None:
                    sys.exit(52)

            elif arg.tag == "arg2":
                split = arg.text.split("@")

                if arg.attrib['type'] == "var":
                    if split[0] != "GF":
                        print("Not implemented")
                        sys.exit(-1)

                    value1 = globalFrame.get(split[-1])
                    if value1 == None:
                        sys.exit(52)

                    try:
                        value1 = int(value1)
                    except ValueError:
                        sys.exit(53)

                elif arg.attrib['type'] == "int":
                    try:
                        value1 = int(split[-1])
                    except ValueError:
                        sys.exit(53)

                else:
                    sys.exit(23)

            elif arg.tag == "arg3":
                split = arg.text.split("@")

                if arg.attrib['type'] == "var":
                    if split[0] != "GF":
                        print("Not implemented")
                        sys.exit(-1)

                    value2 = globalFrame.get(split[-1])
                    if value2 == None:
                        sys.exit(52)

                    try:
                        value2 = int(value1)
                    except ValueError:
                        sys.exit(53)

                elif arg.attrib['type'] == "int":
                    try:
                        value2 = int(split[-1])
                    except ValueError:
                        sys.exit(53)

                else:
                    sys.exit(23)

        globalFrame[dest] = str(value1 - value2)

    def MUL(instruction):
        for arg in instruction:
            if arg.tag == "arg1":
                if arg.attrib['type'] != "var":
                    sys.exit(12) #TODO

                split = arg.text.split("@")
                if split[0] != "GF":
                    print("Not implemented")
                    sys.exit(-1)

                dest = split[-1]
                if globalFrame.get(dest) == None:
                    sys.exit(52)

            elif arg.tag == "arg2":
                split = arg.text.split("@")

                if arg.attrib['type'] == "var":
                    if split[0] != "GF":
                        print("Not implemented")
                        sys.exit(-1)

                    value1 = globalFrame.get(split[-1])
                    if value1 == None:
                        sys.exit(52)

                    try:
                        value1 = int(value1)
                    except ValueError:
                        sys.exit(53)

                elif arg.attrib['type'] == "int":
                    try:
                        value1 = int(split[-1])
                    except ValueError:
                        sys.exit(53)

                else:
                    sys.exit(32)

            elif arg.tag == "arg3":
                split = arg.text.split("@")

                if arg.attrib['type'] == "var":
                    if split[0] != "GF":
                        print("Not implemented")
                        sys.exit(-1)

                    value2 = globalFrame.get(split[-1])
                    if value2 == None:
                        sys.exit(52)

                    try:
                        value2 = int(value1)
                    except ValueError:
                        sys.exit(53)

                elif arg.attrib['type'] == "int":
                    try:
                        value2 = int(split[-1])
                    except ValueError:
                        sys.exit(53)

                else:
                    sys.exit(32)

        globalFrame[dest] = str(value1 * value2)

    def IDIV(instruction):
        for arg in instruction:
            if arg.tag == "arg1":
                if arg.attrib['type'] != "var":
                    sys.exit(12) #TODO

                split = arg.text.split("@")
                if split[0] != "GF":
                    print("Not implemented")
                    sys.exit(-1)

                dest = split[-1]
                if globalFrame.get(dest) == None:
                    sys.exit(52)

            elif arg.tag == "arg2":
                split = arg.text.split("@")

                if arg.attrib['type'] == "var":
                    if split[0] != "GF":
                        print("Not implemented")
                        sys.exit(-1)

                    value1 = globalFrame.get(split[-1])
                    if value1 == None:
                        sys.exit(52)

                    try:
                        value1 = int(value1)
                    except ValueError:
                        sys.exit(53)

                elif arg.attrib['type'] == "int":
                    try:
                        value1 = int(split[-1])
                    except ValueError:
                        sys.exit(53)

                else:
                    sys.exit(32)

            elif arg.tag == "arg3":
                split = arg.text.split("@")

                if arg.attrib['type'] == "var":
                    if split[0] != "GF":
                        print("Not implemented")
                        sys.exit(-1)

                    value2 = globalFrame.get(split[-1])
                    if value2 == None:
                        sys.exit(52)

                    try:
                        value2 = int(value1)
                    except ValueError:
                        sys.exit(53)

                elif arg.attrib['type'] == "int":
                    try:
                        value2 = int(split[-1])
                    except ValueError:
                        sys.exit(53)

                else:
                    sys.exit(32) #TODO

        if value2 == 0:
            sys.exit(57)

        globalFrame[dest] = str(value1 // value2)

#\/ ================ METHODS ================ \/#
def printHelp():
    print("help\n")
    sys.exit(0)

def parseXML(sourceFile):
    try:
        xmlTree = XMLparser.parse(sourceFile)
    except: #TODO
        sys.exit(99)

    root = xmlTree.getroot()

    if ( root.tag != "program" ):
        sys.exit(31)

    try:
        if ( root.attrib['language'] != "IPPcode19" ):
            sys.exit(31)
    except KeyError:
        sys.exit(31)

    for instruction in root:
        if ( instruction.tag != "instruction" ):
            sys.exit(31)

        parsedInstructions.append(instruction)

    #parsedInstructions.append("EOF")

def getLabels():
    for instruction in parsedInstructions:
        if instruction.attrib['opcode'] == "LABEL":
            Instructions.LABEL(instruction)

def checkProgramArguments():
    sourcePresent = False
    inputPresent = False

    for argument in sys.argv:
        if ( argument == "--help" ):
            if ( len(sys.argv) > 2 ):
                sys.exit(10)
            printHelp()

        elif ( argument.startswith("--source=") ):
            if not sourcePresent:
                sourcePresent = True
                parseXML(argument.split("--source=")[-1])
                # TODO zavolat fukciu alebo nastavit nejaky flag
            else:
                sys.exit(10)

        elif ( argument.startswith("--input=") ):
            if not inputPresent:
                inputPresent = True
                # TODO: znovu to iste
            else:
                sys.exit(10)

    if not sourcePresent and not inputPresent:
        sys.exit(10)

def checkInstructions():
    for instruction in parsedInstructions:
        try:
            if ( instruction.attrib['opcode'] not in allowedInstructions ):
                sys.exit(32)
        except KeyError:
            sys.exit(32)

def defineVariable(frame, variable):
    try:
        frame[variable] = "Uninitialized"
    except NameError:
        sys.exit(55)

def isVariableDefined(frame, variable):
    try:
        if frame.get(variable) == None:
            return False
        return True
    except NameError:
        sys.exit(55)

def sortInstructions():
    i = 0
    for instruction in parsedInstructions:
        i+=1
        order = int(instruction.attrib['order'])
        if ( order != i ):
            tmpInstruction = parsedInstructions[order-1]
            parsedInstructions[order-1] = instruction
            parsedInstructions[i-1] = tmpInstruction

#helper function
def printInstructions():
    for instruction in parsedInstructions:
        print(instruction.attrib['order']+". "+instruction.attrib['opcode'])
        for arg in instruction:
            print(arg.tag, arg.attrib, arg.text)

def runProgram():
    instructionPointer = 0

    while instructionPointer != len(parsedInstructions):
        instruction = parsedInstructions[instructionPointer]
        opcode = instruction.attrib['opcode']

        if opcode == "DEFVAR":
            for arg in instruction: # TODO : vymyslioet co ked bude viac argumentov
                frame = arg.text.split("@")[0]
                if frame != "GF":
                    continue

                if not isVariableDefined(globalFrame, arg.text.split("@")[-1]):
                    defineVariable(globalFrame, arg.text.split("@")[-1]) #TODO co ak definujem uz definovanu premennu?

        elif opcode == "WRITE":
            Instructions.WRITE(instruction)

        elif opcode == "MOVE":
            where = what = None
            for arg in instruction:
                if arg.tag == "arg1":
                    where = arg
                elif arg.tag == "arg2":
                    what = arg

            Instructions.MOVE(what, where)

        elif opcode == "JUMP":
            instructionPointer = Instructions.JUMP(instruction)
            continue

        elif opcode == "JUMPIFEQ":
            retVal = Instructions.JUMPIFEQ(instruction)
            if retVal == None:
                instructionPointer+=1
                continue

            instructionPointer = retVal
            continue

        elif opcode == "JUMPIFNEQ":
            retVal = Instructions.JUMPIFNEQ(instruction)
            if retVal == None:
                instructionPointer+=1
                continue

            instructionPointer = retVal
            continue

        elif opcode == "EXIT":
            sys.exit(Instructions.EXIT(instruction))

        elif opcode == "ADD":
            Instructions.ADD(instruction)

        elif opcode == "SUB":
            Instructions.SUB(instruction)

        elif opcode == "MUL":
            Instructions.MUL(instruction)

        elif opcode == "IDIV":
            Instructions.IDIV(instruction)

        elif opcode == "LABEL":
            instructionPointer+=1
            continue
            
        else:
            print("Instruction " + opcode + " not implemented!")

        instructionPointer+=1
#\/ ================ SCRIPT BODY ================ \/#
def main():
    checkProgramArguments()
    checkInstructions()
    sortInstructions()
    getLabels()
    runProgram()

if __name__ == "__main__":
    main()
