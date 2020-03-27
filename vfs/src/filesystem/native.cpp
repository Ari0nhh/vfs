#include "native.h"
#include "../exceptions/exceptions.h"
#include <fstream>

vfs::filesystem::native::Entity::Entity(const std::filesystem::path& root, const std::filesystem::path& path) : m_name(path)
{
	m_path = root / path;
	if (!std::filesystem::exists(m_path))
		throw vfs::exception::EntityNotExist(m_name);

	m_dir = std::filesystem::is_directory(m_path);
	if (m_dir) {
		for (auto& child : std::filesystem::directory_iterator(m_path))
			m_children.push_back(child);
	}
}

vfs::filesystem::native::Entity::Entity(const std::filesystem::path& root, const std::filesystem::path& path, const bool& dir) : m_name(path), m_dir(dir)
{
	m_path = root / path;
	if (!m_dir)
		return;

	if (std::filesystem::is_directory(m_path))
		throw vfs::exception::DirectoryExists(m_name);

	std::filesystem::create_directory(m_path);
}

bool vfs::filesystem::native::Entity::is_dir()
{
	return m_dir;
}

std::filesystem::path vfs::filesystem::native::Entity::name()
{
	return m_name;
}

vfs::IEntity::It vfs::filesystem::native::Entity::begin()
{
	return m_children.cbegin();
}

vfs::IEntity::It vfs::filesystem::native::Entity::end()
{
	return m_children.cend();
}

bool vfs::filesystem::native::Entity::empty()
{
	return m_children.empty();
}

size_t vfs::filesystem::native::File::size()
{
	return std::filesystem::file_size(m_path);
}

size_t vfs::filesystem::native::File::read(char* buffer, const size_t& size, const size_t& pos)
{
	if (!buffer)
		return 0;

	try {
		std::ifstream ifs(m_path.string(), std::ios_base::binary);
		ifs.seekg(pos, std::ios_base::beg);
		if (ifs.eof())
			return 0;

		return ifs.read(buffer, size) ? size : ifs.gcount();
	}
	catch (const std::exception& e) {
		throw vfs::exception::FileOperationFault(m_name, e.what());
	}
}

size_t vfs::filesystem::native::File::write(char* buffer, const size_t& size, const size_t& pos)
{
    if (!buffer)
        return 0;

    try {
        std::ofstream ofs(m_path.string(), std::ios_base::binary);
        ofs.seekp(pos, std::ios_base::beg);
		ofs.write(buffer, size);
		return size;
    }
    catch (const std::exception& e) {
        throw vfs::exception::FileOperationFault(m_name, e.what());
    }
}

bool vfs::filesystem::native::File::copy(const vfs::IFile::Ptr& file)
{
	try {
		if (std::filesystem::exists(m_path))
			std::filesystem::remove(m_path);

		std::ofstream ofs(m_path, std::ios_base::binary);
		
		size_t size = 50 * 1024 * 1024; //file could be from different filesystem
		size_t pos = 0;

		auto buffer = std::make_unique<char[]>(size);

		while (size) {
			size = file->read(buffer.get(), size, pos);
			if (size)
				ofs.write(buffer.get(), size);
			pos += size;
		}

		return true;
	}
	catch (const std::exception&) {}

	return false;
}
