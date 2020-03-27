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
			protected: //IFile
                size_t size() override;
                size_t read(char* buffer, const size_t& size, const size_t& pos) override;
                size_t write(char* buffer, const size_t& size, const size_t& pos) override;
                bool copy(const vfs::IFile::Ptr& file) override;
			};

		}
	}
}