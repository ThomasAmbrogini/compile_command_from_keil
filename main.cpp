#include <filesystem>
#include <iostream>
#include <string_view>

#include "tinyxml2.h"

namespace fs = std::filesystem;

/*
 *
 *
 *         //TODO: do i only need these?
 *           <VariousControls>
 *             <MiscControls>--info=stack</MiscControls>
 *             <Define>NDEBUG, __SAME70Q21__,CMSIS, SLS_NO_FS, SLS_NO_FS1, FIT_VERSION, SLS_SYSCTL, SLS_PRINTF=1, TRACE_LEVEL=TRACE_LEVEL_ERROR, __SLS_CUSTOM_GMAC_DRIVER__, SLS_PLATFORM_ENHANCED</Define>
 *             <Undefine></Undefine>
 *             <IncludePath>.\;.\PrecompiledHeaders;..\..\sources\headers;..\..\..\SLS_Common\sources\headers;..\..\sources\Dolmen\common_sources;..\..\sources\Dolmen\common_sources\Support;..\..\sources\Dolmen\common_sources\Support\SystemFactory;..\..\sources\Dolmen\common_sources\Support\General;..\..\sources\Dolmen\common_sources\Support\Heap;..\..\sources\Dolmen\common_sources\OSAL;..\..\sources\Dolmen\common_sources\Support\Property;..\..\sources\Dolmen\common_sources\Support\Trace;..\..\sources\Dolmen\common_sources\Support\MsgQueue;..\..\sources\Dolmen\common_sources\Support\StateMachine;..\..\sources\Dolmen\common_sources\Support\Status;..\..\sources\Dolmen\common_sources\Support\Settings;..\..\sources\Dolmen\common_sources\Support\Protocol\LoaderUpdater;..\..\sources\Dolmen\common_sources\VirtualDevice\FlashMgr;..\..\sources\Dolmen\common_sources\Support\GenericManager;..\..\sources\Dolmen\platform\Quadros_RTXc\OS\RTXC_CORE\Include;..\..\sources\Dolmen\platform\Quadros_RTXc\OS\RTXC_GEN\inc;..\..\sources\Dolmen\platform\Quadros_RTXc\OS\RTXC_CORE\CortexM3Core;..\..\sources\Dolmen\common_sources\VirtualDevice\Sensor;..\..\sources\Dolmen\common_sources\Support\DATA\Picture;..\..\sources\Dolmen\common_sources\VirtualDevice\Amplifier;..\..\sources\Dolmen\common_sources\HiService\Capture;..\..\sources\Dolmen\common_sources\Support\DATA\FrameAnalysis;..\..\sources\Dolmen\common_sources\Support\DATA\Movie;..\..\sources\Dolmen\common_sources\VirtualDevice\Illuminator;..\..\sources\Dolmen\common_sources\LoService\Communicate;..\..\sources\Dolmen\common_sources\Application\Scanner;..\..\sources\Dolmen\common_sources\VirtualDevice\Trigger;..\..\sources\Dolmen\common_sources\HiService\Processing;..\..\sources\Dolmen\common_sources\HiService\Processing\Decoder;..\..\sources\Dolmen\common_sources\Support\DATA\Result;..\..\sources\Dolmen\common_sources\LoService\SecurityMgr;..\..\sources\Dolmen\common_sources\VirtualDevice\ChannelBridge;..\..\sources\Dolmen\common_sources\LoService\Communicate\ImageChannel;..\..\sources\VirtualDevice\Sensor;..\..\sources\Application;..\..\sources\Dolmen\common_sources\SOC\Timer;..\..\sources\Dolmen\common_sources\SOC;..\..\sources\Dolmen\common_sources\LoService\LowPowerManager;..\..\sources\Dolmen\common_sources\SOC\GPIO;..\..\sources\SOC;..\..\sources\Dolmen\common_sources\SOC\USART;..\..\sources\Dolmen\common_sources\SOC\CommunicationSOC;..\..\sources\Dolmen\common_sources\Support\CircularBuffer;..\..\sources\platform;..\..\sources\OSAL\OS;..\..\sources\Dolmen\common_sources\OSAL\OS;..\..\sources\RTX\INC;..\..\..\SLS_Common\sources\VendorLib\libraries\libchip_samv7;..\..\..\SLS_Common\sources\VendorLib\libraries\libchip_samv7\include;..\..\..\SLS_Common\sources\VendorLib\libraries\libchip_samv7\include\samv7;..\..\..\SLS_Common\sources\VendorLib\libraries\libboard_samv7-ek;..\..\sources\Dolmen\common_sources\LoService\ServiceMgr;..\..\sources\Dolmen\common_sources\Support\Environment;..\..\sources\Dolmen\common_sources\LoService\Communicate\ServiceChannel;..\..\sources\Application\Compute\Processing;..\..\sources\Dolmen\common_sources\SOC\DMA;..\..\sources\Dolmen\common_sources\SOC\SPI;..\..\sources\Dolmen\common_sources\Support\DATA\Movie\src;..\..\sources\Dolmen\common_sources\LoService\DumpFrame;..\..\sources\HAL;..\..\sources\Dolmen\common_sources\HAL;..\..\sources\VirtualDevice\MotorEncoder;..\..\sources\Dolmen\common_sources\SOC\I2C;..\..\sources\Application\Diagnostic;..\..\sources\Application\Supervisor;..\..\sources\Application\Compute;..\..\sources\Dolmen\common_sources\HiService\PostProcessing;..\..\sources\Status;..\..\sources\Application\Config;..\..\sources\Dolmen\common_sources\HiService\ConfigManager;..\..\sources\Dolmen\common_sources\VirtualDevice\ChannelBridge\src;..\..\sources\VirtualDevice\Communication;..\..\..\SLS_Common\sources\VendorLib\libraries\libchip_samv7\include\samv7\component;..\..\..\SLS_Common\sources\VendorLib\libraries\libchip_samv7\include\samv7\instance;..\..\..\SLS_Common\sources\VendorLib\libraries\libchip_samv7\include\samv7\pio;..\..\..\SLS_Common\sources\VendorLib\libraries\libboard_samv7-ek\include;..\..\sources\VirtualDevice\Watchdog;..\..\sources\VirtualDevice\TemperatureSensorAPD;..\..\sources\include;.\RTE;..\..\sources\VirtualDevice\Display;..\..\sources\VirtualDevice\QuadratureEncoder;..\..\sources\VirtualDevice\SLS_RTC;..\..\sources\Application\Log_Manager;..\..\sources\VirtualDevice\Inputs;..\..\sources\VirtualDevice\DeviceLifetimeManager;..\..\sources\Motor\include;..\..\sources\VirtualDevice\TOF;..\..\sources\lib;..\..\..\SLS_Common\sources;..\..\..\SLS_Common\sources\include;..\..\..\SLS_Common\sources\lib;.\RTE\CMSIS\inc;..\..\sources\Application\GUI;..\..\..\sls_brand\sources\include;..\..\sources\Application\Compute\Decoder;..\..\sources\Application\Capture;..\..\..\sls_common\sources\lib\network;..\..\sources\include\measures</IncludePath>
 *           </VariousControls>
 *
 *
 */

enum class SearchingStates {
    SearchingLeaves,
    SearchingLeaves,
};

int main() {
    using namespace tinyxml2;

    std::string_view keil_project_filename { "SLS_Application.uvprojx" };

    fs::path keil_project_file { keil_project_filename };
    fs::path keil_project_file_abs { fs::absolute(keil_project_file).string() };

    XMLDocument doc;
    //TODO: check for error on loadFile()
    doc.LoadFile(keil_project_file_abs.string().c_str());

    const XMLElement* root_element = doc.RootElement();
    std::cout << root_element->Name() << std::endl;
    std::cout << "Num elements: " << root_element->ChildElementCount() << std::endl;

    bool found {false};
    const XMLElement* element = root_element;
    const XMLElement* moving_element = root_element;
    constexpr std::string_view element_to_find { "VariousControls" };

    while(!found) {
        switch (state) {
            case SearchingLeaves:
                if (element->Name() != element_to_find) {
                    if (element->FirstChildElement()) {
                        element = element->FirstChildElement();
                    }
                    else if (element->NextSibling()) {
                        element = element->NextSibling();
                    } else {
                        element = element->Parent();
                        if (element->NextSibling()) {
                            element = element->NextSibling();
                        }
                    }
                } else {
                    found = true;
                    std::cout << "I have found it!" << std::endl;
                }
            break;
        }
    }
}

