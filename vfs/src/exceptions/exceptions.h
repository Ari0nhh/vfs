#pragma once
#include "vfs.h"

namespace vfs {
	namespace exception {

		class EntityNotExist : public std::runtime_error {
		public:
			explicit EntityNotExist(const std::filesystem::path& name) : std::runtime_error("Error: file " + name.string() + " not exists") {}
		};

		class DirectoryExists : public std::runtime_error {
		public:
			explicit DirectoryExists(const std::filesystem::path& name) : std::runtime_error("Error: directory " + name.string() + " already exists") {}
		};
	}
}