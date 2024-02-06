#pragma once

#include "Marco.h"
#include <json.hpp>

namespace ToolEngine
{
    /// <summary>
    /// A path utils by filesytem
    /// All path is end with '\\' and is absolute path
    /// </summary>
    class Path final
    {
    public:
        static Path& getInstance()
        {
            static Path instance;
            return instance;
        }
        void setCurrentPath(std::string path);
        std::string getCurrentPath() const { return m_exe_path.parent_path().string(); }
        std::string getAssetPath();
        std::string getShaderPath();

        bool isExist(const std::string& path);
        bool isFile(const std::string& path);
        bool isDirectory(const std::string& path);
        bool createPath(const std::string& path);
        bool deletePath(const std::string& path);
        bool copy(const std::string& src, const std::string& dst);
        bool save(const std::string& path, const std::vector<char>& data);
        bool saveJson(const std::string& path, const nlohmann::json& data);
        std::vector<char> read(const std::string& path);
        nlohmann::json readJson(const std::string& path);
        std::string getFileName(const std::string& path);
        std::string getFileNameWithoutExtension(const std::string& path);
        std::string getFileExtension(const std::string& path);
        std::string getFileDirectory(const std::string& path);
        std::string getDirectoryParentDirectory(const std::string& path);
        std::vector<std::string> getAllFilesInDirectory(const std::string& path);
        std::vector<std::string> getAllDirectoriesInDirectory(const std::string& path);
    private:
        Path();
        ~Path();
        std::filesystem::path m_exe_path;
    };
}