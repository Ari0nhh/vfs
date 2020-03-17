#pragma once
#include "vfs.h"

namespace vfs {
	namespace filesystem {
		namespace native {
			
			class Entity : public IEntity {
			public:
				explicit Entity(const std::filesystem::path& root, const std::filesystem::path& path);
				explicit Entity(const std::filesystem::path& root, const std::filesystem::path& path, const bool& dir);
			protected: //IEntity
				bool is_dir() override;
				std::filesystem::path name() override;
				It begin() override;
				It end() override;
				bool empty() override;
			private:
				bool m_dir = false;
				std::filesystem::path m_name;
				std::filesystem::path m_path;
				std::vector<std::filesystem::path> m_children;
			};

		}
	}
}