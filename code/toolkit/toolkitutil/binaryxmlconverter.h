#pragma once
//------------------------------------------------------------------------------
/**
    @class Toolkit::BinaryXmlConverter
    
    Convert XML files into generic binary format.
    
    (C) 2009 Radon Labs GmbH
*/
#include "toolkitutil/platform.h"
#include "toolkitutil/logger.h"
#include "util/string.h"
#include "util/stringatom.h"
#include "io/xmlreader.h"
#include "io/util/bxmlfilestructs.h"

//------------------------------------------------------------------------------
namespace ToolkitUtil
{
class BinaryXmlConverter
{
public:
    /// constructor
    BinaryXmlConverter();
    /// destructor
    ~BinaryXmlConverter();

    /// set target platform
    void SetPlatform(Platform::Code platform);
    /// set source directory
    void SetSrcDir(const Util::String& srcDir);
    /// set destination directory
    void SetDstDir(const Util::String& dstDir);

    /// recursive convert all XML files in source directory
    bool Convert(Logger& logger);

private:
    static const ushort InvalidNodeIndex = 0xffff;
    static const ushort InvalidAttrIndex = 0xffff;

    struct Attr
    {
        Util::StringAtom name;
        Util::StringAtom value;
    };

    struct Node
    {
        Util::StringAtom name;
        Util::Array<Attr> attrs;
        Util::Array<Node> childNodes;
    };

    /// recursively convert files in directory
    bool RecurseConvert(const Util::String& srcDir, const Util::String& dstDir, Logger& logger);
    /// convert a single file
    bool ConvertFile(const Util::String& srcFile, const Util::String& dstFile, Logger& logger);

    /// load an XML file into the internal representation
    bool LoadFile(const Util::String& path, Logger& logger);
    /// validate the parsed data
    bool ValidateInternalRepresentation(const Util::String& srcFile, Logger& logger);
    /// save the internal representation into a binary XML file
    bool SaveFile(const Util::String& path, Logger& logger);
    /// clear string in unique-string table
    void ClearUniqueStrings();
    /// add a unique string to the string table
    void AddUniqueString(const Util::StringAtom& str);
    /// recursively parse XML file, building internal representation on the way
    bool RecurseParseXml(const Ptr<IO::XmlReader>& xmlReader, Node& curNode, Logger& logger);
    /// recursively count attributes in internal representation
    SizeT RecurseCountAttrs(const Node& node, SizeT numAttrs);
    /// recursively count nodes in internal representation
    SizeT RecurseCountNodes(const Node& node, SizeT numNodes);
    /// build file structure arrays
    void BuildFileArrays();
    /// recursively build file arrays (add one node)
    ushort RecurseAddFileNode(const Node& node, ushort prevSiblingIndex, ushort parentNodeIndex);

    Platform::Code platform;
    Util::String srcDir;
    Util::String dstDir;
    Util::Array<Util::StringAtom> stringTable;
    Node rootNode;
    Util::Array<IO::BXMLFileAttr> fileAttrArray;
    Util::Array<IO::BXMLFileNode> fileNodeArray;
};

//------------------------------------------------------------------------------
/**
*/
inline void
BinaryXmlConverter::SetPlatform(Platform::Code p)
{
    this->platform = p;
}

//------------------------------------------------------------------------------
/**
*/
inline void
BinaryXmlConverter::SetSrcDir(const Util::String& s)
{
    this->srcDir = s;
}

//------------------------------------------------------------------------------
/**
*/
inline void
BinaryXmlConverter::SetDstDir(const Util::String& s)
{
    this->dstDir = s;
}

} // namespace ToolkitUtil
//------------------------------------------------------------------------------
