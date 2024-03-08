/*

Copyright 2023 PCiD

This file is part of BBP.

BBP is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General
Public License as published by the Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

BBP is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with BBP. If not, see
<https://www.gnu.org/licenses/>.

Author: Rafael de Bie

*/

#include "../include/Kernel.h"

void POST(BBP::Graphics::window *window, BBP::IO::File::VOLUME_INFO bootVolume)
{
	/*
	BBP BIOS (C) One Blast
	Running TSauce+ OS 0.1.0
	MCU : STM32L0

	Boot found on volume 'V' (SDMMC)
	Press 'enter' to enter BIOS

	(C) PCiD 2023.
	xxx-xxx-xxx-xxx
	*/


	BBP::Graphics::R2D::noStroke(window);
	BBP::Graphics::R2D::fill(window, 255, 0, 0);
	BBP::Graphics::R2D::Rect(window, 5, 5, 65, 40);

	BBP::Graphics::R2D::setCursorPos(window, 10, 10);
	BBP::Graphics::R2D::fill(window, 0, 0, 0);
	BBP::Graphics::R2D::print(window, "OB ");
	BBP::Graphics::R2D::fill(window, 255, 0, 0);
	BBP::Graphics::R2D::print(window, "BB607");

	char POSTdata[161];
	const char *Vendor =
		"\n\n\n"
		"BBP BIOS(C) One Blast\n"
		"Running " BBP_KERNEL_NAME " OS V" BBP_KERNEL_VERSION "\n"
		"MCU : STM32L0\n\n"

		"Boot found on volume '%c' (SDMMC)\n"
		"Press \'\u000d\' to enter BIOS\n\n"

		"(C) PCiD 2023.\n"
		"xxx - xxx - xxx - xxx\n";

	sprintf(POSTdata, Vendor, bootVolume.label);

	window->fontSize = 1;
	BBP::Graphics::R2D::fill(window, 255, 255, 255);
	BBP::Graphics::R2D::print(window, POSTdata);
	BBP::Graphics::Driver::drawWindow(window);

	BBP::Services::Delay(5000);
}


KERNEL_STATUS kernel_entry()
{
	// Initialize ROM
	int ROM_status = BBP::IO::Memory::ROM::initializeROM();
	if (ROM_status)
		return KERNEL_BIOSERROR_ROM; // Return with error if ROM wasn't initialized properly

	// Initialize interrupts
	bool Interrupt_status = BBP::Services::Interrupts::initializeInterrupts();
	if (!Interrupt_status)
		return KERNEL_INTERRUPT_ERROR; // Return with error if interrupts weren't initialized properly

	// Initialize SDMMC
	bool SDMMC_status = BBP::IO::SDMMC::initializeSDMMC();
	if (SDMMC_status)
		return KERNEL_BIOSERROR_SDMMC; // Return with error if SDMMC could not be loaded

	// Get the amount of drives available
	int SDMMC_drives = BBP::IO::File::getVolumeCount();
	if (SDMMC_drives == 0)
		return KERNEL_NOVOLUME; // Return with error if no volumes were found

	// Loop until bootable volume found
	int bootable_volume = -1;
	BBP::IO::File::VOLUME_INFO vol;
	for (int i = 0; i < SDMMC_drives; i++)
	{
		// Get volume data for index
		vol = BBP::IO::File::getVolumeInfo(i);

		// If not bootable, continue
		if (!vol.bootable)
			continue;

		// Volume is bootable. Set bootable_volume to current index and break
		bootable_volume = i;
		break;
	}

	// If no bootable medium is found, return with error
	if (bootable_volume == -1)
		return KERNEL_NOBOOT;

	// Since we know the bootable medium is available, fetch the bootable file
	BBP::IO::File::FILE_HANDLE bootableFile = BBP::IO::SDMMC::readFile(BBP_KERNEL_BOOTLOCATION);

	// Check if the bootable file is valid
	if (bootableFile.fileVector == nullptr || bootableFile.byteCount == 0)
		return KERNEL_BOOTINVALID; // Somehow this medium is unbootable, yet was marked bootable. Return error

	// Create a bootable lua file from this
	int bootable_index = BBP::Services::ApplicationServices::createApplicationFromFile(bootableFile);
	BBP::application *bootable = BBP::Services::ApplicationServices::applicationsVector[bootable_index];
	
	// Initialize the display
	int Display_status = BBP::Graphics::Driver::initializeGraphics(&bootable->renderer, 320, 480);
	if (Display_status)
		return KERNEL_BIOSERROR_DISP; // Return with error if display could not be initialized

	// Read the amount of characters stored in ROM
	int amountOfFontsInRom = BBP::IO::Memory::ROM::readROMInt(6);

	// Load the font into graphics
	BBP::Graphics::R2D::GetFontFromROM(&bootable->renderer, 7, amountOfFontsInRom);

	// Show POST
	POST(&bootable->renderer, vol);

	// Execute the main function.
	BBP::Services::ApplicationServices::Lua::Execute(bootable, "main()", true);

	int masterloop = BBP::Services::Interrupts::createInterrupt();
	BBP::Services::Interrupts::Timer::attachTimerInterruptTo(masterloop, 5000, false);

	while (!BBP::Services::Interrupts::Poll(masterloop))
	{
		BBP::Services::Interrupts::updateInterrupts();
	}

	BBP::Services::ApplicationServices::closeApplication(bootable, false);

	BBP::Graphics::Driver::drawWindow(&bootable->renderer);
	BBP::Services::Delay(4000);
	

	return KERNEL_OK;
}



void BBP::Services::Delay(long long delay)
{
	int timer = BBP::Services::Interrupts::createInterrupt();
	int tim = BBP::Services::Interrupts::Timer::attachTimerInterruptTo(timer, delay, false);

	while (!BBP::Services::Interrupts::Poll(timer))
		BBP::Services::Interrupts::updateInterrupts();
}

void BBP::Services::Throw(KERNEL_STATUS status)
{

}