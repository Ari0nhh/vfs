#pragma once
#include "vfs.h"

namespace vfs {
	namespace filesystem {
		namespace native {
			
			class Entity : public virtual vfs::IEntity {
			public:
				explicit Entity(const std::filesystem::path& root, const std::filesystem::path& path);
				explicit Entity(const std::filesystem::path& root, const std::filesystem::path& path, const bool& dir);
			protected: //IEntity
				bool is_dir() override;
				std::filesystem::path name() override;
				It begin() override;
				It end() override;
				bool empty() override;
			protected:
				bool m_dir = false;
				std::filesystem::path m_name;
				std::filesystem::path m_path;
				std::vector<std::filesystem::path> m_children;
			};

			class File : public virtual Entity, public virtual vfs::IFile {
			public:
				explicit File(const std::filesystem::path& root, const std::filesystem::path& path) : Entity(root, path) {};
				explicit File(const std::filesystem::path& root, const std::filesystem::path& path, const bool& dir) : Entity(root, path, dir) {};
			protected: //IFile
                size_t size() override;
                size_t read(char* buffer, const size_t& size, const size_t& pos) override;
                size_t write(char* buffer, const size_t& size, const size_t& pos) override;
                bool copy(const vfs::IFile::Ptr& file) override;
			};

			class FileSystem : public IFileSystem {
			public:
				explicit FileSystem(const Options& opt);
			protected: //IFileSystem
                IEntity::Ptr create(const std::filesystem::path& path, const bool& folder) override;
                IEntity::Ptr open(const std::filesystem::path& path) override;
                bool copy(const std::filesystem::path& src, const std::filesystem::path& dst) override;
                bool move(const std::filesystem::path& src, const std::filesystem::path& dst) override;
                bool remove(const std::filesystem::path& path) override;
			private:
				std::filesystem::path m_root;
			};

		}
	}
}