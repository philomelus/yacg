#include "Common.hpp"

// TODO:  Create this file during build process.  sed, awk, or python will work.

// To import xcg.xml:
//   1.  Select all lines between braces
//   2.  Select menu item "Edit | Insert File As Text"
//   3.  Select "xcg.xml"
//   4.  Use regular expression search/replace with the following replacements:
//     (a) Repalce /"/ with /\\"/
//     (b) Replace /^\(.\)/ with /"\1/
//     (c) Replace /\(.\)$/ with /\1\\n/
//     (d) Replace /\\t/ with /&tab;/
//     (e) Replace /\t/ with /\\t/
//     (f) Replace /&tab;/ with /\\\\t/

const char* impl::xmlDefault =
{
"<?xml version=\"1.0\"?>\n"
};

// To import xcg.csd:
//   1.  Select all lines between braces
//   2.  Select menu item "Edit | Insert File As Text"
//   3.  Select "xcg.xsd"
//   4.  Use regular expression search/replace with the following replacements:
//     (a) Repalce /"/ with /\\"/
//     (b) Replace /^\(.\)/ with /"\1/
//     (c) Replace /\(.\)$/ with /\1\\n/
//     (d) Replace /\\t/ with /&tab;/
//     (e) Replace /\t/ with /\\t/
//     (f) Replace /&tab;/ with /\\\\t/

const char* impl::xmlSchema =
{
"<?xml version=\"1.0\"?>\n"
};
