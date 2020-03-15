#include "volume.h"

namespace vfs {
	IVolume::Ptr create() {
		return std::make_shared<vfs::Volume>();
	}
}

bool vfs::Volume::mount(const std::filesystem::path& root, const vfs::Kind& kind, const Options& opt)
{
	std::lock_guard<std::recursive_mutex> lock(m_mtxLock);
	
	auto it = m_mMounts.find(root);
	if (m_mMounts.end() != it)
		return false;

	auto fs = make(kind, opt);
	if (!fs)
		return false;

	m_mMounts[root] = fs;
	
	return true;
}

bool vfs::Volume::unmount(const std::filesystem::path& root)
{
	std::lock_guard<std::recursive_mutex> lock(m_mtxLock);
	auto path = mount_path(root);
	if (!path)
		return false;

	m_mMounts.erase(*path);
	return true;
}

vfs::IEntity::Ptr vfs::Volume::create(const std::filesystem::path& path, const bool& folder)
{
	std::lock_guard<std::recursive_mutex> lock(m_mtxLock);
	auto fs = mounted(path);
	return fs ? fs->create(path, folder) : nullptr;
}

vfs::IEntity::Ptr vfs::Volume::open(const std::filesystem::path& path)
{
	std::lock_guard<std::recursive_mutex> lock(m_mtxLock);
	auto fs = mounted(path);
	return fs ? fs->open(path) : nullptr;
}

bool vfs::Volume::copy(const std::filesystem::path& src, const std::filesystem::path& dest)
{
	std::lock_guard<std::recursive_mutex> lock(m_mtxLock);
	auto fsSrc = mounted(src);
	auto fsDest = mounted(dest);

	if (!fsSrc || !fsDest)
		return false;

	if (fsSrc == fsDest)
		return fsSrc->copy(src, dest);
	
	auto entity = fsSrc->open(src);
	if (!entity)
		return false;

	return copy_entity(entity, fsSrc, fsDest, trim_to_fs(dest));
}

bool vfs::Volume::move(const std::filesystem::path& src, const std::filesystem::path& dest)
{
	std::lock_guard<std::recursive_mutex> lock(m_mtxLock);
	auto fsSrc = mounted(src);
	auto fsDest = mounted(dest);

	if (!fsSrc || !fsDest)
		return false;

	if (fsSrc == fsDest)
		return fsSrc->move(src, dest);

	auto entity = fsSrc->open(src);
	if (!entity)
		return false;

	if (!copy_entity(entity, fsSrc, fsDest, trim_to_fs(dest)))
		return false;

	fsSrc->remove(src);
	return true;
}

bool vfs::Volume::remove(const std::filesystem::path& path)
{
	std::lock_guard<std::recursive_mutex> lock(m_mtxLock);
	auto fs = mounted(path);
	return fs ? fs->remove(path) : false;
}

vfs::IFileSystem::Ptr vfs::Volume::mounted(const std::filesystem::path& path)
{
	auto res = mount_path(path);
	if (!res)
		return nullptr;

	return m_mMounts[*res];
}

bool vfs::Volume::copy_entity(vfs::IEntity::Ptr ent, vfs::IFileSystem::Ptr fsSrc, vfs::IFileSystem::Ptr fsDest, const std::filesystem::path& path)
{
	if (!ent->is_dir()) {
		auto dst = fsDest->create(path, false);
		if (!dst)
			return false;

		auto srcFile = std::dynamic_pointer_cast<vfs::IFile>(ent);
		auto dstFile = std::dynamic_pointer_cast<vfs::IFile>(dst);
		return dstFile->copy(srcFile);
	}

	auto dst = fsDest->create(path, true);
	if (!dst)
		return false;

	for (auto itm : *dst) {
		auto src_ent = fsSrc->open(itm);
		if (!src_ent)
			return false;
		
		if(!copy_entity(src_ent, fsSrc, fsDest, trim_to_fs(itm)))
			return false;
	}

	return true;
}

std::filesystem::path vfs::Volume::trim_to_fs(const std::filesystem::path& src)
{
	auto mnt = mount_path(src);
	if (!mnt)
		return {};

	auto root_dist = std::distance(mnt->begin(), mnt->end());
	auto src_dist = std::distance(src.begin(), src.end());

	if (src_dist < root_dist)
		return {};

	std::filesystem::path res = "/";
	
	auto bg = src.begin();
	std::advance(bg, root_dist);
	while (bg != src.end()) {
		res /= *bg;
		++bg;
	}
	
	return res;
}

std::optional<std::filesystem::path> vfs::Volume::mount_path(const std::filesystem::path& src)
{
	int min_dist = -1;
	std::filesystem::path res;
	for (auto it : m_mMounts) {
		auto dist = distance(it.first, src);

		if (dist < 0)
			continue;

		if ((min_dist < 0) || (min_dist > dist)) {
			min_dist = dist;
			res = it.first;
		}
	}

	if (min_dist < 0)
		return {};

	return res;
}

int vfs::Volume::distance(const std::filesystem::path& root, const std::filesystem::path& path)
{
	auto root_len = std::distance(root.begin(), root.end());
	auto path_len = std::distance(path.begin(), path.end());
	if (path_len < root_len)
		return -1;

	if (!std::equal(root.begin(), root.end(), path.begin()))
		return -1;
	
	return int(path_len - root_len);
}

vfs::IFileSystem::Ptr vfs::Volume::make(const Kind&, const Options&)
{
	return nullptr;
}
