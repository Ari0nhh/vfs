#pragma once

#include <memory>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <functional>
#include <map>
#include <mutex>
#include <optional>

#ifndef interface
#define interface struct
#endif

namespace vfs {
	
	interface IEntity {
		using Ptr = std::shared_ptr<IEntity>;
		using It = std::vector<std::filesystem::path>::const_iterator;
	public:
		virtual bool is_dir() = 0;
		virtual std::filesystem::path name() = 0;
		virtual It begin() = 0;
		virtual It end() = 0;
		virtual bool empty() = 0;
		virtual ~IEntity() = default;
	};
	
	interface IFile : public virtual IEntity {
	public:
		using Ptr = std::shared_ptr<IFile>;
	public:
		virtual size_t size() = 0;
		virtual size_t read(char*, const size_t&, const size_t&) = 0;
		virtual size_t write(char*, const size_t&, const size_t&) = 0;
		virtual bool copy(const IFile::Ptr&) = 0;
		virtual bool seek(const size_t&) = 0;
		virtual ~IFile() = default;
	};

	interface IFileSystem {
		virtual IEntity::Ptr create(const std::filesystem::path&, const bool&) = 0;
		virtual IEntity::Ptr open(const std::filesystem::path&) = 0;
		virtual bool copy(const std::filesystem::path&, const std::filesystem::path&) = 0;
		virtual bool move(const std::filesystem::path&, const std::filesystem::path&) = 0;
		virtual bool remove(const std::filesystem::path&) = 0;
		virtual ~IFileSystem() = default;
	public:
		using Ptr = std::shared_ptr<IFileSystem>;
	};

	enum class Kind { fsNative = 0, fsMemory, fsZip };
	
	interface IVolume : public IFileSystem {
	public:
		using Ptr = std::shared_ptr<IVolume>;
		using Options = std::map<std::string, std::string>;
	public:
		virtual bool mount(const std::filesystem::path&, const Kind&, const Options&) = 0;
		virtual bool unmount(const std::filesystem::path&) = 0;
		virtual ~IVolume() = default;
	
	};

	IVolume::Ptr create();
}
