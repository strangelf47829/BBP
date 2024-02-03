import xml.etree.ElementTree as ET

# This function looks for functions and returns them as a list
def findFunctions(dir):

    # Parse xml
    tree = ET.parse(dir)
    root = tree.getroot()

    # Create list where each element is a tuple (the file it is in, the list of functions within that file)
    functions = []
    
    for object in root:
        # Create file name
        searchIn = "data/" + object.attrib["refid"] + ".xml"
        print(f"Searching in {searchIn}")
        
        # Local function list
        localFuncs = []
        
        for member in object:
            # If not a function, who cares
            if member.attrib == {}:
                continue
               
            if not member.attrib["kind"] == "function":
                continue
                
            localFuncs.append(member.attrib["refid"])
        
        if not localFuncs == []:
            functions.append((searchIn, localFuncs))
        
    return functions
    
    
    
def getFunctionData(functions):
    # Create list of functions
    listOfFunctions = []
    
    # Loop over each file
    for file in functions:
        print(f"In file {file[0]}: ")
        
        # Parse that file
        sourceFile = ET.parse(file[0])
        root = sourceFile.getroot()
        
        # Print something
        for thing in root:
            
            # Search for section defs
            sectionDefs = thing.findall("sectiondef")
            
            # Loop over each sectionDef
            for sectionDef in sectionDefs:
                # If kind is not funcs, do nothing                
                if not ("func" in sectionDef.attrib["kind"]):
                    continue
                    
                print("\treading functions of type", sectionDef.attrib["kind"])
                
                # Find all member defs
                for member in sectionDef:
                    
                    # Get name, definition, id, and params
                    name = member.find("name")
                    definition = member.find("definition")
                    args = member.find("argsstring")
                    refid = member.attrib["id"]
                    
                    # Also, get file
                    loc = member.find("location")
                    inFile = loc.attrib["file"]
                    funcBody = "(No body)"
                    
                    # Check if function is defined
                    if "bodyend" in loc.keys():
                        # If defined, read the file
                        fromLine = int(loc.attrib["bodystart"])
                        toLine = int(loc.attrib["bodyend"])
                        
                        # Open file
                        fileD = open(f"{inFile}", 'r')
                        fileLines = fileD.readlines()
                        
                        print(f"From {fromLine} to {toLine}")
                        
                        # Slice then join
                        funcBody = '\n'.join(fileLines[fromLine - 1:toLine])
                        print(funcBody)
                        
                        # Close file
                        fileD.close()
                    else:
                        continue
                    
                    # Get name of functions
                    entry = (funcBody, (name.text, definition.text, args.text, refid))
                    
                    # Then append entry to list
                    listOfFunctions.append(entry)
                    
                    print(f"\t\tAdded function {entry[1][0]}")
                    
        print()
        
    return listOfFunctions
    
    
    

def main():
    # Get functions
    functionList = findFunctions("data/index.xml")
    
    # Convert the list into a list of body and stuff
    functions = getFunctionData(functionList)
    
    # Set output file name
    outputName = "output.xml"
    
    # Save functions list
    tree = ET.ElementTree(ET.fromstring('<xml></xml>'))
    root = tree.getroot()
    
    
    # Save output in output.xml
    idx = 0
    for function in functions:
        # Create node for function
        funcNode = ET.SubElement(root, "function")
        funcNode.text = function[0]
        
        # Add index and id
        funcNode.set("index", str(idx))
        funcNode.set("id", function[1][3])
        
        # Save name and args
        funcNode.set("name", function[1][0])
        funcNode.set("args", function[1][2])
        funcNode.set("definition", function[1][1])
    
        # Increase idx
        idx += 1
     
    # This produces the string
    outputXML = ET.tostring(root, encoding='utf8', method='xml').decode("utf-8")
    
    # Save to file
    print(outputXML,  file=open(outputName, 'w'))
    
    # Print out count
    print(f"Found {idx} functions. Output saved as {outputName}")
    


main()
