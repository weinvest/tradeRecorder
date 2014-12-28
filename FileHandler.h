#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <iterator>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/function.hpp>
#include <boost/xpressive/xpressive_dynamic.hpp>
namespace fs = boost::filesystem;
namespace re = boost::xpressive;

typedef boost::function<bool (const fs::path& path,bool isDir)> Filter;
typedef std::vector<fs::path> PathVec;

re::sregex Wildcard2Regex(std::string wildcardPattern);
void FindFiles(const fs::path& dirPath,const Filter& filter,PathVec& out);
void FindFiles(const fs::path& dirPath,const std::string pattern,PathVec& out);
void FindFiles(const fs::path& dirPath,const re::sregex& pattern,PathVec& out);

template<typename Visit>
void TraverseFile(const fs::path& filePath,Visit visit);

void FindAndReplace(const fs::path& filePath,const std::string& strFrom,const std::string& strTo);
void FindAndReplace(const fs::path& filePath,const re::sregex& regFrom,const std::string& strTo);

void CopyFiles(const fs::path& fromDir,const fs::path& toDir,const re::sregex& pattern);

#include "FileHandler.hpp"

#endif // FILEHANDLER_H
