#include "Path.h"

namespace Engine
{
    Path::Path()
    {

    }
    Path::~Path()
    {

    }
    void Path::setCurrentPath(std::string path)
    {
        m_exe_path = path;
    }
    bool Path::isExist(const std::string& path)
    {
        return std::filesystem::exists(path);
    }
    bool Path::isFile(const std::string& path)
    {
        return std::filesystem::exists(path) && std::filesystem::is_regular_file(path);
    }
    bool Path::isDirectory(const std::string& path)
    {
        return std::filesystem::exists(path) && std::filesystem::is_directory(path);
    }
    bool Path::createPath(const std::string& path)
    {
        if (std::filesystem::exists(path) || std::filesystem::create_directories(path))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    bool Path::deletePath(const std::string& path)
    {
        if (std::filesystem::exists(path) && std::filesystem::remove_all(path))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    bool Path::copy(const std::string& src, const std::string& dst)
    {
        if (src == dst)
        {
            return true;
        }
        if (!isExist(getParentPath(dst)))
        {
            createPath(getParentPath(dst));
        }
        if (std::filesystem::copy_file(src, dst, std::filesystem::copy_options::overwrite_existing))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    bool Path::save(const std::string& path, const std::vector<char>& data)
    {
        std::ofstream file(path, std::ios::binary);
        if (!file.is_open())
        {
            return false;
        }
        file.write(data.data(), data.size());
        file.close();
        return true;
    }
    std::vector<char> Path::read(const std::string& path)
    {
        std::ifstream file(path, std::ios::ate | std::ios::binary);
        if (!file.is_open())
        {
            return {};
        }
        size_t file_size = (size_t)file.tellg();
        std::vector<char> buffer(file_size);
        file.seekg(0);
        file.read(buffer.data(), file_size);
        file.close();
        return buffer;
    }
    std::string Path::getFileName(const std::string& path)
    {
        return std::filesystem::u8path(path).filename().generic_string();
    }
    std::string Path::getFileNameWithoutExtension(const std::string& path)
    {
        std::string filename = getFileName(path);
        size_t last_index = filename.find_last_of('.');
        if (last_index != std::string::npos)
        {
            return filename.substr(0, last_index);
        }
        return filename;
    }
    std::string Path::getExtension(const std::string& path)
    {
        return std::filesystem::u8path(path).extension().generic_string();
    }
    std::string Path::getParentPath(const std::string& path)
    {
        std::filesystem::path file_path = std::filesystem::u8path(path);
        return file_path.parent_path().string();
    }
}