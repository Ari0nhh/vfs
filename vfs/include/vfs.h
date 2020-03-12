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
	
	interface IFile{
		virtual std::filesystem::path name() = 0;
		virtual ~IFile() = default;
	public:
		using Ptr = std::shared_ptr<IFile>;
		using It = std::vector<Ptr>::const_iterator;
	};

	interface IFiles {
		virtual IFile::It begin() = 0;
		virtual IFile::It end() = 0;
		virtual size_t size() = 0;
		virtual bool empty() = 0;
		virtual ~IFiles() = default;
	public:
		using Ptr = std::shared_ptr<IFiles>;
	};

	/*interface IFolder;
	
	interface IFolders {
		virtual IFolder::It begin() = 0;
		virtual IFolder::It end() = 0;
		virtual bool empty() = 0;
		virtual size_t size() = 0;
		virtual ~IFolders() = default;
	public:
		using Ptr = std::shared_ptr<IFolders>;
	};

	interface IFolder {
	public:
		using Ptr = std::shared_ptr<IFolder>;
		using Ptr_ = std::weak_ptr <IFolder>;
		using It = std::vector<Ptr>::const_iterator;
	public:
		virtual std::filesystem::path name() = 0;
		virtual IFiles::Ptr files();
		virtual IFolder::Ptr parent() = 0;
		virtual IFolders::Ptr folders() = 0;
		virtual ~IFolder() = default;
	};*/

	interface IFileSystem {
		virtual std::string name() = 0;
		virtual IFile::Ptr create_file(const std::filesystem::path&) = 0;
		virtual IFile::Ptr open_file(const std::filesystem::path&) = 0;
		virtual ~IFileSystem() = default;
	public:
		using Ptr = std::shared_ptr<IFileSystem>;
	};

	interface IVolume {
		virtual IFileSystem::Ptr mount(const std::filesystem::path&) = 0;
		virtual ~IVolume() = default;
	public:
		using Ptr = std::shared_ptr<IVolume>;
	};

	IVolume::Ptr get();
}
