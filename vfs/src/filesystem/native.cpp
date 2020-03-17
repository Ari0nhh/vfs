#include "native.h"
#include "../exceptions/exceptions.h"

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
