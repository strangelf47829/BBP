#ifndef BBP_SYSTEM_LICENSE_H
#define BBP_SYSTEM_LICENSE_H

#include "stddef.h"
#include "Strings.h"
#include "Time.h"

// This file defines some structures used for licensing and versioning information.

namespace BBP
{
	namespace system
	{
		
		struct buildInfo
		{
			// Time stamp this build info is valid
			std::string buildTime;

			// Day this build info is valid
			std::string buildDate;

			// Versioning information
			std::byte majorVersion;
			std::byte minorVersion;
			std::byte patchVersion;
			std::byte buildVersion;

			// Extra information
			std::conststring extraInfo;

			// Default constructor
			inline buildInfo() : buildTime(std::String(__TIME__)), buildDate(std::String(__DATE__)), majorVersion(0), minorVersion(1), patchVersion(0), buildVersion(1), extraInfo("Debug (noplatform)") {}
			inline buildInfo(std::byte major, std::byte minor, std::byte patch, std::byte build, std::conststring extra) : buildTime(std::String(__TIME__)), buildDate(std::String(__DATE__)), majorVersion(major), minorVersion(minor), patchVersion(patch), buildVersion(build), extraInfo(extra) {}
		};

		// Holds copyright information
		struct copyrightInfo
		{
			// Year copyright is valid
			std::word copyrightYear;

			// Who is the copright to
			std::conststring copyrightHolder;

			copyrightInfo(std::word year, std::conststring holder) : copyrightYear(year), copyrightHolder(holder) {}
		};

		// License struct
		struct License
		{
			// Name of the license
			std::conststring licenseName;

			// Information for the license
			std::conststring licenseInfo;

			// URL Where license is located
			std::conststring licenseURL;

			License(std::conststring n, std::conststring i, std::conststring u) : licenseName(n), licenseInfo(i), licenseURL(u) {}
		};

		// Holds all buildinformation
		struct appInfo
		{
			buildInfo build;
			copyrightInfo copyright;
			License license;

			appInfo(std::byte maj, std::byte min, std::byte pat, std::byte bui, std::conststring ext, std::word year, std::conststring copy, std::conststring name, std::conststring info, std::conststring url)
				: build(maj, min, pat, bui, ext), copyright(year, copy), license(name, info, url) {}

			appInfo(std::byte maj, std::byte min, std::byte pat, std::byte bui)
				: appInfo(maj, min, pat, bui, "Debug", 2024, "Rafael de Bie", "GNU GPL", "more info", "127.0.0.1") {}

			appInfo()
				: appInfo(0, 1, 0, 1) {}

		};


	}
}


#endif
