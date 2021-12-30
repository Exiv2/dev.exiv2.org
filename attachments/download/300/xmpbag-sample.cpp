#include <exiv2/exiv2.hpp>

#include <iostream>
#include <iomanip>
#include <cassert>

void printTags(const Exiv2::XmpData& xmpData);

int main()
try {
    Exiv2::XmpData xmpData;

    std::cout << "--------- Setting Xmp.dc.subject to 'the subject' ----------\n";
    xmpData["Xmp.dc.subject"] = "the subject";
    printTags(xmpData);

    std::cout << "--------- Append 'new value' ----------\n";
    xmpData["Xmp.dc.subject"] = "new value";
    printTags(xmpData);

    std::cout << "--------- Erasing Xmp.dc.subject, setting value to 'Erased completely' ----------\n";
    Exiv2::XmpData::iterator pos = xmpData.findKey(Exiv2::XmpKey("Xmp.dc.subject"));
    assert(pos != xmpData.end());
    xmpData.erase(pos);
    xmpData["Xmp.dc.subject"] = "Erased completely";
    printTags(xmpData);

    std::cout << "--------- Direct access to set the value to 'full access to the raw value' ----------\n";
    pos = xmpData.findKey(Exiv2::XmpKey("Xmp.dc.subject"));
    assert(pos != xmpData.end());
    Exiv2::Value::AutoPtr val = pos->getValue();
    Exiv2::XmpArrayValue* xmpVal = dynamic_cast<Exiv2::XmpArrayValue*>(val.release());
    assert(xmpVal != 0);
    // The raw value is a simple std::vector<std::string> in this case (see class doc)
    Exiv2::XmpArrayValue::ValueType& rawVal = xmpVal->value_;
    rawVal.clear();
    rawVal.push_back("full access to the raw value");
    xmpData.erase(pos);
    xmpData["Xmp.dc.subject"].setValue(xmpVal);
    printTags(xmpData);

    // Cleanup
    Exiv2::XmpParser::terminate();

    return 0;
}
catch (Exiv2::AnyError& e) {
    std::cout << "Caught Exiv2 exception '" << e << "'\n";
    return -1;
}

void printTags(const Exiv2::XmpData& xmpData)
{
    // -------------------------------------------------------------------------
    // Output XMP properties
    for (Exiv2::XmpData::const_iterator md = xmpData.begin(); 
         md != xmpData.end(); ++md) {
        std::cout << std::setfill(' ') << std::left
                  << std::setw(44)
                  << md->key() << " "
                  << std::setw(9) << std::setfill(' ') << std::left
                  << md->typeName() << " "
                  << std::dec << std::setw(3)
                  << std::setfill(' ') << std::right
                  << md->count() << "  "
                  << std::dec << md->value()
                  << std::endl;
    }
}
