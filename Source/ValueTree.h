/*
 ==============================================================================
 ValueTree.h
 Created: 20 May 2022 12:10:10pm
 Author:  Rithwik Sathyan
 Value Trees are used to collect and organise plugin parameters.
 Saved as xml to allow for easy access in other parts of the plugin.
 ==============================================================================
 */


#pragma once

#include <JuceHeader.h>

class VTree {
public:
    VTree() = default;
    ValueTree createTree (const String& name, const int& desc);
    ValueTree createRootValueTree();
    ValueTree loadValueTreeFromXml(const File & file);
    void saveValueTreeToXml(ValueTree vtr, const File & file);
    void printVT(ValueTree toPrint);
private:
    
};
