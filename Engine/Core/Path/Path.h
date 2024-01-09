#pragma once

#include "Marco.h"

namespace Engine
{
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

        bool isExist(const std::string& path);
        bool isFile(const std::string& path);
        bool isDirectory(const std::string& path);
        bool createPath(const std::string& path);
        bool deletePath(const std::string& path);
        bool copy(const std::string& src, const std::string& dst);
        bool save(const std::string& path, const std::vector<char>& data);
        std::vector<char> read(const std::string& path);
        std::string getFileName(const std::string& path);
        std::string getFileNameWithoutExtension(const std::string& path);
        std::string getExtension(const std::string& path);
        std::string getParentPath(const std::string& path);
    private:
        Path();
        ~Path();
        std::filesystem::path m_exe_path;
    };
}