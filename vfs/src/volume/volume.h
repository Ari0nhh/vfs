#pragma once
#include "vfs.h"

namespace vfs {
	class Volume : public IVolume {
	protected: //IVolume
		bool mount(const std::filesystem::path& root, const Kind& kind, const Options& opt) override;
		bool unmount(const std::filesystem::path& root) override;
	protected: //IFileSystem
		IEntity::Ptr create(const std::filesystem::path& path, const bool& folder) override;
		IEntity::Ptr open(const std::filesystem::path& path) override;
		bool copy(const std::filesystem::path& src, const std::filesystem::path& dest) override;
		bool move(const std::filesystem::path& src, const std::filesystem::path& dest) override;
		bool remove(const std::filesystem::path& path) override;
	private:
		std::recursive_mutex m_mtxLock;
		std::map<std::filesystem::path, IFileSystem::Ptr> m_mMounts;
		IFileSystem::Ptr mounted(const std::filesystem::path& path);
		bool copy_entity(vfs::IEntity::Ptr ent, vfs::IFileSystem::Ptr fsSrc, vfs::IFileSystem::Ptr fsDest, const std::filesystem::path& path);
		std::filesystem::path trim_to_fs(const std::filesystem::path& src);
		std::optional<std::filesystem::path> mount_path(const std::filesystem::path& src);
		int distance(const std::filesystem::path& root, const std::filesystem::path& path);
	private:
		IFileSystem::Ptr make(const Kind& kind, const Options& opt);
	};
}