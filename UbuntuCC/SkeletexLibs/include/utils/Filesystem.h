#pragma once
#include <fstream>
#include <vector>
#include <codecvt>
#include <string>
#include <string>
#include <list>
#include <sys/stat.h>
#include <experimental/filesystem>

#ifdef _WIN32
#include <Windows.h>
#include <conio.h>
#endif // _WIN32

#include "ExtSTD.h"
#include "FileTypes.h"



//! An essential interface for manipulation with files and directories. Wraps file managing system calls.
namespace fs
{

  //! Checks if the specified file exist.
  inline bool FileExists(const char *filename)
  {
    struct stat info;
    return (stat(filename, &info) == 0 && !(info.st_mode & S_IFDIR));
  }

  //! Checks if the specified file exist
  inline bool FileExists(const std::string &filename)
  {
    return FileExists(filename.c_str());
  }

  //! Makes a copy of a source file to a destination file. Destination directory must exist.
  inline bool FileCopy(const std::string &src, const std::string &dest)
  {
    std::ifstream orig(src, std::ios::binary);
    std::ofstream copy(dest, std::ios::binary);
    if (!orig.good() || !copy.good())
    {
      return false;
    }
    copy << orig.rdbuf();
    return true;
  }

  //! Changes name of a source file to a new name.
  inline bool FileRename(const std::string &src_file, const std::string &new_name)
  {
    return std::rename(src_file.c_str(), new_name.c_str());
  }

  //! Deletes a specified file from the system. Deleted file will not appear in recycle bin.
  inline bool FileRemove(const std::string &file)
  {
    return std::remove(file.c_str()) == 0;
  }

  //! Checks whether a specified directory exists. May contain the last slash but is not required.
  inline bool DirectoryExists(const std::string &dir_path)
  {
    if (dir_path.empty())
    {
      return true;
    }

    struct stat info;

    if (stat(dir_path.c_str(), &info) != 0)
    {
      return false; // cannot access to the directory
    }
    else
    {
      return (info.st_mode & S_IFDIR);
    }
  }

  /*!
    \brief Creates a specified directory.

    This function does not create directory hierarchies. If you wish to create hierarchy, use CreateDirHierarchy function instead.

    \param dir_path Specifies path and name of directory. May contain the last slash but is not required.
  */
  inline bool CreateDir(const std::string &dir_path)
  {
    return std::experimental::filesystem::create_directory(dir_path);
  }

  /*!
    \brief Creates a specified directory hierarchy.

    This function creates directory hierarchies. If parent folder does not exists, function creates it recursively.

    \param file_path Specifies path and name of directory. May contain the last slash but is not required.
  */
  inline bool CreateDirHierarchy(const std::string &dir_path)
  {
    return std::experimental::filesystem::create_directories(dir_path);
  }

  //! Removes input directory if it is empty or if recursive is set to true
  inline bool RemoveDir(const std::string &dir_path, bool recursive = true)
  {
    if (recursive)
    {
      return std::experimental::filesystem::remove_all(dir_path) > 0u;
    }
    return std::experimental::filesystem::remove(dir_path);
  }

  //! Returns string representing file extension. Uses original case and no dot before extension
  inline std::string GetExtension(const std::string &filename)
  {
    auto startpos = filename.find_first_of(".", filename.find_last_of(R"(/\)") + 1) + 1;
    auto endpos = filename.length();
    return filename.substr(startpos, endpos - startpos);
  }

  //! Checks whether the file has a specific extension. An extension parameter is case sensitive and can be specified with or without a dot.
  inline bool HasExtenstion(const std::string &filename, const std::string &extenstion)
  {
    const auto f_result = filename.rfind(extenstion);
    return (f_result != std::string::npos && f_result == (filename.size() - extenstion.size()));
  }

  //! Checks whether the file is of a specified type.
  inline bool IsFileType(const std::string &filename, const file_types::Type type)
  {
    return HasExtenstion(filename, file_types::Ext(type));
  }

  //! Checks whether the file is any of the specified types.
  inline bool IsFileType(const std::string &filename, const std::vector<file_types::Type> &types)
  {
    return std_ext::AnyOf(types, [&filename](const auto & type) { return IsFileType(filename, type); });
  }

  //! Loads contents of a text-based file and stores it in a string.
  inline std::string FileToString(const std::string &file_name)
  {
    std::ifstream ifs(file_name);
    std::string contents;
    std::getline(ifs, contents, (char)ifs.eof());
    return contents;
  }

  //! Exports the string to a text file.
  inline void StringToFile(const std::string &file_name, const std::string &str)
  {
    std::ofstream file;
    file.open(file_name);
    file << str;
    file.close();
  }

  //! Information about single system directory entry.
  struct FileDirContent
  {
    FileDirContent(const std::string &_name, const bool is_dir) : name(_name), is_directory(is_dir) {};
    std::string name;       //! Entry name. If file, contains also file extension.
    bool is_directory;      //! Whether entry is directory.
    std::string full_path;  //! Full system path to the entry.
  };

  namespace
  {
    inline void ListDirectoryContentsRecursive(const std::string &direcory_path, uint32_t depth, std::vector<FileDirContent> *results)
    {
      for (auto &p : std::experimental::filesystem::directory_iterator(direcory_path.c_str()))
      {
        FileDirContent item(p.path().filename().string(), p.status().type() == std::experimental::filesystem::file_type::directory);
        item.full_path = direcory_path + item.name;

        if (item.is_directory && (item.name == "." || item.name == ".."))
        {
          continue;
        }

        if (results->size() == results->capacity())
        {
          results->reserve(results->capacity() * 2);
        }

        results->push_back(item);
        if (depth > 0 && item.is_directory)
        {
          ListDirectoryContentsRecursive(item.full_path + '/', depth - 1, results);
        }
      }
    }
  }

  //! Returns list of all contents in a specified directory.
  inline std::vector<FileDirContent> ListDirectoryContents(std::string direcory_path, const bool recursive = false)
  {
    std::vector<FileDirContent> results;
    results.reserve(32);

    direcory_path.erase(direcory_path.find_last_not_of("\\/") + 1, std::string::npos);
    direcory_path += '/';

    ListDirectoryContentsRecursive(direcory_path, (recursive) ? UINT32_MAX : 0, &results);

    return results;
  }

  //! Returns list of all file contents in a specified directory (ignores directories). Providing extension parameter returns only files with specified extension.
  inline std::vector<std::string> ListDirectoryFileNames(const std::string &direcory_path, const std::string &extension = "")
  {
    const std::vector<FileDirContent> content = ListDirectoryContents(direcory_path);
    std::vector<std::string> result;
    result.reserve(content.size());

    for (auto &item : content)
    {
      if (!item.is_directory)
      {
        if (extension == "" || HasExtenstion(item.name, extension))
        {
          result.emplace_back(item.name);
        }
      }
    }
    result.shrink_to_fit();
    return result;
  }

  //! Returns list of all file contents in a specified directory (ignores directories) with extension of the specified file type.
  inline std::vector<std::string> ListDirectoryFileNames(const std::string &direcory_path, const file_types::Type file_type)
  {
    return ListDirectoryFileNames(direcory_path, file_types::Ext(file_type));
  }

  //! Checks specified directory if it contains file(s) of specified type.
  inline bool DirectoryContainsAny(const std::string &direcory_path, const file_types::Type file_type)
  {
    return !ListDirectoryFileNames(direcory_path, file_type).empty();
  }

  //! Checks specified directory if it contains file(s) of any of the specified types.
  inline bool DirectoryContainsAny(const std::string &direcory_path, const std::vector<file_types::Type> &file_types)
  {
    return std_ext::AnyOf(file_types, [&](const auto & file_type)->bool { return !ListDirectoryFileNames(direcory_path, file_type).empty(); });
  }

  //! Takes specified file path and returns its directory path. Strips everything after the last directory separator.
  inline std::string GetDirectoryPathOfFile(const std::string &file_name_with_path)
  {
    auto pos = file_name_with_path.find_last_of(R"(/\)");
    return (pos != std::string::npos) ? file_name_with_path.substr(0, pos) : "";
  }

  //! Strips everything before the last directory separator, including the separator. If parameter strip_extension is true, strips also extension from resulting file name.
  inline std::string GetFileName(const std::string &file_name_with_path, bool strip_extension = false)
  {
    auto startpos = file_name_with_path.find_last_of(R"(/\)") + 1;
    auto endpos = file_name_with_path.length();
    if (strip_extension)
    {
      endpos = file_name_with_path.find_first_of(".", startpos);
    }
    return file_name_with_path.substr(startpos, endpos - startpos);
  }

  //! Retrieves full system path to a specified local file or directory. Specified local path may contain non existing directories and files.
  inline std::string GetAbsolutePath(const std::string &local_path)
  {
    return std::experimental::filesystem::absolute(local_path.c_str()).string();
  }

  /*!
    \brief Executes command as if it was called from the command line.
    \param command Command to be called.
    \param buffer_size Determines size of temporal buffer used to obtain output.
    \return Standard output generated by the executed command.
  */
  inline std::string Execute(const char *command, const size_t buffer_size = 128)
  {
    std::vector<char> buffer(buffer_size);
    std::string result;
#ifdef _WIN32
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(command, "r"), _pclose);
#elif __linux__
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command, "r"), pclose);
#else
#error "platform not supported"
#endif
    if (!pipe)
    {
      throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr)
    {
      result += buffer.data();
    }
    return result;
  }

  //! Runs executable in command line
  inline void RunExe(const std::string &exe_path, const std::string &cmd_arguments = "")
  {
#ifdef _WIN32
    // additional information
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // set the size of the structures
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    const std::wstring exe_pathW = std_ext::ToWString(exe_path);
    const std::wstring cmd_argumentsW = std_ext::ToWString(cmd_arguments);

    // start the program up
    CreateProcess(exe_pathW.c_str(),                                        // the path
      const_cast<wchar_t *>((exe_pathW + L" " + cmd_argumentsW).c_str()),        // Command line
      NULL,                                                                // Process handle not inheritable
      NULL,                                                                // Thread handle not inheritable
      FALSE,                                                               // Set handle inheritance to FALSE
      0,                                                                   // No creation flags
      NULL,                                                                // Use parent's environment block
      NULL,                                                                // Use parent's starting directory
      &si,                                                                 // Pointer to STARTUPINFO structure
      &pi                                                                  // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
    );

    WaitForSingleObject(pi.hProcess, INFINITE);

    // Close process and thread handles.
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
#elif __linux__
    int commandReturnValue = -1;
    if (exe_path[0] == '/')
    {
      commandReturnValue = system((exe_path + " " + cmd_arguments).c_str());
    }
    else
    {
      commandReturnValue = system(("./" + exe_path + " " + cmd_arguments).c_str());
    }

    if (commandReturnValue < 0)
    {
      std::cout << "Command:  " << exe_path << " " << cmd_arguments << "  failed." << std::endl;
    }
#else
#error "platform not supported"
#endif
  }

}
