/*
 ==============================================================================
 ValueTree.cpp
 Created: 20 May 2022 12:10:10pm
 Author:  Rithwik Sathyan
 Value Trees are used to collect and organise plugin parameters.
 Saved as xml to allow for easy access in other parts of the plugin.
 ==============================================================================
 */



#include "ValueTree.h"

ValueTree VTree::createTree (const String& name, const int& desc)
{
    ValueTree t (name);
    t.setProperty (name, desc, nullptr);
    return t;
}

ValueTree VTree::loadValueTreeFromXml(const File & file)
{
    File files = File::getSpecialLocation (File::currentApplicationFile).getChildFile ("params.xml");
    const String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
    File ptfFile = pathToFile ;
    std::unique_ptr<XmlElement> xml = XmlDocument(ptfFile.loadFileAsString()).getDocumentElement();
    
    if (!xml)
        return ValueTree{};
    
    auto tree = ValueTree::fromXml(*xml);
    return tree;
}

void VTree::saveValueTreeToXml(ValueTree vtr, const File & file)
{
    File files = File::getSpecialLocation (File::currentApplicationFile).getChildFile ("params.xml");
    const String pathToFile = File::getSpecialLocation(File::currentApplicationFile).getChildFile ("params.xml").getFullPathName();
    File ptfFile = pathToFile ;
    std::unique_ptr<XmlElement> xml (vtr.createXml());
    xml->writeTo (ptfFile);
}

//Useful for Debugging
void VTree::printVT(ValueTree toPrint)
{
    static const Identifier printTest("xxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
    toPrint.setProperty(printTest, "x", nullptr);
    toPrint.removeProperty(printTest, nullptr);
}
