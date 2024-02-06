#include "Path.h"

namespace ToolEngine
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
    std::string Path::getAssetPath()
    {
        // TODO: load asset path from config file
        auto path = m_exe_path.parent_path().parent_path().parent_path().parent_path().string();
        return path + "\\Assets\\";
    }
    std::string Path::getShaderPath()
    {
        auto path = m_exe_path.parent_path().parent_path().parent_path().parent_path().string();
        return path + "\\Shader\\SPV\\";
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
        if (!isExist(getFileDirectory(dst)))
        {
            createPath(getFileDirectory(dst));
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
    bool Path::saveJson(const std::string& path, const nlohmann::json& data)
    {
        std::ofstream file(path);
        file << std::setw(4) << data << std::endl;
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
    nlohmann::json Path::readJson(const std::string& path)
    {
        std::ifstream file(path);
        nlohmann::json j;
        if (file.is_open())
        {
			file >> j;
			file.close();
			return j;
		}
        return nlohmann::json();
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
    std::string Path::getFileExtension(const std::string& path)
    {
        return std::filesystem::u8path(path).extension().generic_string();
    }
    std::string Path::getFileDirectory(const std::string& path)
    {
        std::filesystem::path file_path = std::filesystem::u8path(path);
        return file_path.parent_path().string();
    }
    std::string Path::getDirectoryParentDirectory(const std::string& path)
    {
        std::filesystem::path directory_path(path);
        std::string parent_path = directory_path.parent_path().parent_path().string();
        return parent_path + "\\";
    }
    std::vector<std::string> Path::getAllFilesInDirectory(const std::string& path)
    {
        if (!isExist(path) || !isDirectory(path))
        {
            return {};
        }
        std::vector<std::string> file_names;
        for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			if (entry.is_regular_file())
			{
				file_names.push_back(entry.path().string());
			}
		}
        return file_names;
    }
    std::vector<std::string> Path::getAllDirectoriesInDirectory(const std::string& path)
    {
        if (!isExist(path) || !isDirectory(path))
        {
            return {};
        }
        std::vector<std::string> directory_names;
        for (const auto& entry : std::filesystem::directory_iterator(path))
        {
            if (entry.is_directory())
            {
                directory_names.push_back(entry.path().string());
            }
        }
        return directory_names;
    }
}