/*
   Copyright (c) 2016, The CyanogenMod Project
   Copyright (c) 2019-2020, The LineageOS Project
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.
   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <cstdlib>
#include <fstream>
#include <string.h>
#include <sys/sysinfo.h>
#include <unistd.h>

#include <android-base/file.h>
#include <android-base/properties.h>
#include <android-base/strings.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include "vendor_init.h"
#include "property_service.h"

using android::base::GetProperty;
using android::base::ReadFileToString;

char const *heapstartsize;
char const *heapgrowthlimit;
char const *heapsize;
char const *heapminfree;
char const *heapmaxfree;
char const *heaptargetutilization;

void check_device()
{
    struct sysinfo sys;

    sysinfo(&sys);

    if (sys.totalram > 5072ull * 1024 * 1024) {
        // from - phone-xhdpi-6144-dalvik-heap.mk
        heapstartsize = "16m";
        heapgrowthlimit = "256m";
        heapsize = "512m";
        heaptargetutilization = "0.5";
        heapminfree = "8m";
        heapmaxfree = "32m";
    } else if (sys.totalram > 3072ull * 1024 * 1024) {
        // from - phone-xxhdpi-4096-dalvik-heap.mk
        heapstartsize = "8m";
        heapgrowthlimit = "256m";
        heapsize = "512m";
        heaptargetutilization = "0.6";
        heapminfree = "8m";
        heapmaxfree = "16m";
    } else {
        // from - phone-xhdpi-2048-dalvik-heap.mk
        heapstartsize = "8m";
        heapgrowthlimit = "192m";
        heapsize = "512m";
        heaptargetutilization = "0.75";
        heapminfree = "512k";
        heapmaxfree = "8m";
    }
}

void property_override(char const prop[], char const value[], bool add = true)
{
    auto pi = (prop_info *) __system_property_find(prop);

    if (pi != nullptr) {
        __system_property_update(pi, value, strlen(value));
    } else if (add) {
        __system_property_add(prop, strlen(prop), value, strlen(value));
    }
}

void property_override_dual(char const system_prop[],
        char const vendor_prop[], char const value[])
{
    property_override(system_prop, value);
    property_override(vendor_prop, value);
}

void vendor_load_persist_properties()
{
    std::string product = GetProperty("ro.product.vendor.device", "");
    if (product.find("clover") != std::string::npos) {

    std::string hw_device;

    char const *hw_id_file = "/sys/devices/platform/HardwareInfo/hw_id";

    ReadFileToString(hw_id_file, &hw_device);
    if (hw_device.find("D9P") != std::string::npos) {
        property_override("persist.sys.fp.vendor", "fpc");
        property_override("ro.board.variant", "d9p");
        property_override("vendor.display.lcd_density", "265");
        property_override_dual("ro.product.model", "ro.vendor.product.model", "MI PAD 4 PLUS");

        property_override ("persist.vendor.audio.calfile0","/vendor/etc/acdbdata/QRD/sdm660-snd-card-d9p/QRD_D9P_Bluetooth_cal.acdb");
        property_override ("persist.vendor.audio.calfile1","/vendor/etc/acdbdata/QRD/sdm660-snd-card-d9p/QRD_D9P_General_cal.acdb");
        property_override ("persist.vendor.audio.calfile2","/vendor/etc/acdbdata/QRD/sdm660-snd-card-d9p/QRD_D9P_Global_cal.acdb");
        property_override ("persist.vendor.audio.calfile3","/vendor/etc/acdbdata/QRD/sdm660-snd-card-d9p/QRD_D9P_Handset_cal.acdb");
        property_override ("persist.vendor.audio.calfile4","/vendor/etc/acdbdata/QRD/sdm660-snd-card-d9p/QRD_D9P_Hdmi_cal.acdb");
        property_override ("persist.vendor.audio.calfile5","/vendor/etc/acdbdata/QRD/sdm660-snd-card-d9p/QRD_D9P_Headset_cal.acdb");
        property_override ("persist.vendor.audio.calfile6","/vendor/etc/acdbdata/QRD/sdm660-snd-card-d9p/QRD_D9P_Speaker_cal.acdb");
        property_override ("persist.vendor.audio.calfile7","/vendor/etc/acdbdata/QRD/sdm660-snd-card-d9p/QRD_D9P_workspaceFile.qwsp");
        property_override ("persist.vendor.audio.calfile8","/vendor/etc/acdbdata/adsp_avs_config.acdb");

    } else {
        property_override("persist.sys.fp.vendor", "none");
        property_override("ro.board.variant", "d9");
        property_override("vendor.display.lcd_density", "320");
        property_override_dual("ro.product.model", "ro.vendor.product.model", "MI PAD 4");

        property_override ("persist.vendor.audio.calfile0","/vendor/etc/acdbdata/QRD/sdm660-snd-card-skush/QRD_SKUSH_Bluetooth_cal.acdb");
        property_override ("persist.vendor.audio.calfile1","/vendor/etc/acdbdata/QRD/sdm660-snd-card-skush/QRD_SKUSH_General_cal.acdb");
        property_override ("persist.vendor.audio.calfile2","/vendor/etc/acdbdata/QRD/sdm660-snd-card-skush/QRD_SKUSH_Global_cal.acdb");
        property_override ("persist.vendor.audio.calfile3","/vendor/etc/acdbdata/QRD/sdm660-snd-card-skush/QRD_SKUSH_Handset_cal.acdb");
        property_override ("persist.vendor.audio.calfile4","/vendor/etc/acdbdata/QRD/sdm660-snd-card-skush/QRD_SKUSH_Hdmi_cal.acdb");
        property_override ("persist.vendor.audio.calfile5","/vendor/etc/acdbdata/QRD/sdm660-snd-card-skush/QRD_SKUSH_Headset_cal.acdb");
        property_override ("persist.vendor.audio.calfile6","/vendor/etc/acdbdata/QRD/sdm660-snd-card-skush/QRD_SKUSH_Speaker_cal.acdb");
        property_override ("persist.vendor.audio.calfile7","/vendor/etc/acdbdata/QRD/sdm660-snd-card-skush/QRD_SKUSH_workspaceFile.qwsp");
        property_override ("persist.vendor.audio.calfile8","/vendor/etc/acdbdata/adsp_avs_config.acdb");
        
    }
  }
}
void vendor_load_properties()
{
    check_device();
    
    property_override("dalvik.vm.heapstartsize", heapstartsize);
    property_override("dalvik.vm.heapgrowthlimit", heapgrowthlimit);
    property_override("dalvik.vm.heapsize", heapsize);
    property_override("dalvik.vm.heaptargetutilization", heaptargetutilization);
    property_override("dalvik.vm.heapminfree", heapminfree);
    property_override("dalvik.vm.heapmaxfree", heapmaxfree);
    
    property_override_dual("ro.build.fingerprint", "ro.vendor.build.fingerprint", "xiaomi/wayne/wayne:8.1.0/OPM1.171019.011/V9.5.11.0.ODCCNFA:user/release-keys");
  
   std::string product = GetProperty("ro.product.vendor.device", "");	
   if (product.find("whyred") != std::string::npos)
   {
  	std::string region = GetProperty("ro.boot.hwc", "");

    if (region.find("CN") != std::string::npos || region.find("Global") != std::string::npos || region.find("GLOBAL") != std::string::npos)
	{
        property_override_dual("ro.product.model", "ro.vendor.product.model", "Redmi Note 5");
        property_override_dual("ro.product.odm.model", "ro.product.system.model", "Redmi Note 5");
        property_override_dual("ro.product.vendor.model", "persist.vendor.camera.exif.model", "Redmi Note 5");
	}
	else
	{
        property_override_dual("ro.product.model", "ro.vendor.product.model", "Redmi Note 5 Pro");
        property_override_dual("ro.product.odm.model", "ro.product.system.model", "Redmi Note 5 Pro");
        property_override_dual("ro.product.vendor.model", "persist.vendor.camera.exif.model", "Redmi Note 5 Pro");
	}
  }
}
