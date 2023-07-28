/*
 * https://github.com/SamuelTulach/OverlayCord
 */

#ifndef OVERLAYCORD_H
#define OVERLAYCORD_H

#include <Windows.h>
#include <iostream>
#include <string>

namespace OverlayCord
{
	typedef struct _Header
	{
		UINT Magic;
		UINT FrameCount;
		UINT NoClue;
		UINT Width;
		UINT Height;
		BYTE Buffer[1];
	} Header;

	typedef struct _ConnectedProcessInfo
	{
		UINT ProcessId;
		HANDLE File;
		Header* MappedAddress;
	} ConnectedProcessInfo;

	inline bool ConnectToProcess(ConnectedProcessInfo& processInfo)
	{
		std::string mappedFilename = "DiscordOverlay_Framebuffer_Memory_" + std::to_string(processInfo.ProcessId);
		processInfo.File = OpenFileMappingA(FILE_MAP_ALL_ACCESS, false, mappedFilename.c_str());
		if (!processInfo.File || processInfo.File == INVALID_HANDLE_VALUE)
			return false;

		processInfo.MappedAddress = static_cast<Header*>(MapViewOfFile(processInfo.File, FILE_MAP_ALL_ACCESS, 0, 0, 0));
		return processInfo.MappedAddress;
	}

	inline void DisconnectFromProcess(ConnectedProcessInfo& processInfo)
	{
		UnmapViewOfFile(processInfo.MappedAddress);
		processInfo.MappedAddress = nullptr;

		CloseHandle(processInfo.File);
		processInfo.File = nullptr;
	}

	inline void SendFrame(ConnectedProcessInfo& processInfo, UINT width, UINT height, void* frame, UINT size)
	{
		// frame is in B8G8R8A8 format
		// size can be nearly anything since it will get resized
		// for the screen appropriately, although the maximum size is
		// game window width * height * 4 (BGRA)
		processInfo.MappedAddress->Width = width;
		processInfo.MappedAddress->Height = height;

		memcpy(processInfo.MappedAddress->Buffer, frame, size);

		processInfo.MappedAddress->FrameCount++; // this will cause the internal module to copy over the framebuffer
	}
}

#endif