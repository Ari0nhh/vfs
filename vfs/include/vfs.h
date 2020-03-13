#pragma once

#include <memory>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <functional>
#include <map>
#include <mutex>

#ifndef interface
#define interface struct
#endif

namespace vfs {
	
	interface IEntity {
		using Ptr = std::shared_ptr<IEntity>;
		using It = std::vector<std::filesystem::path>::const_iterator;
	public:
		virtual bool is_dir() = 0;
		virtual std::filesystem::path name();
		virtual It begin() = 0;
		virtual It end() = 0;
		virtual bool empty() = 0;
		virtual ~IEntity() = default;
	};
	
	interface IFile : public IEntity {
		virtual ~IFile() = default;
	public:
		using Ptr = std::shared_ptr<IFile>;
	};

	interface IFileSystem {
		virtual ~IFileSystem() = default;
	public:
		using Ptr = std::shared_ptr<IFileSystem>;
	};

	interface IVolume {
		virtual ~IVolume() = default;
	public:
		using Ptr = std::shared_ptr<IVolume>;
	};
}
