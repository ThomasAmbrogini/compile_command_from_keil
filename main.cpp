#include <filesystem>
#include <iostream>
#include <string_view>
#include <vector>
#include <thread>

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

//TODO: this can return an expected.
tinyxml2::XMLElement* searchDF(tinyxml2::XMLElement* element, std::string_view find_el_name, int num_occurrence) {
    using namespace tinyxml2;

    XMLElement* ret {};
    int count {};
    std::vector<XMLElement*> frontier;
    frontier.push_back(element);

    while(frontier.size() != 0) {
        element = frontier.back();
        frontier.pop_back();
        if (element->Name() == find_el_name) {
            ++count;
            if (count == num_occurrence) {
                ret = element;
                break;
            }
        }

        if (element->LastChildElement()) {
            element = element->LastChildElement();
            frontier.push_back(element);
            while(element->PreviousSiblingElement()) {
                element = element->PreviousSiblingElement();
                frontier.push_back(element);
            }
        }
    }

    return element;
}

tinyxml2::XMLElement* searchDFRecursive(tinyxml2::XMLElement* element, std::string_view find_el_name, int& num_occurrence) {
    using namespace tinyxml2;

    if (!element) {
        return nullptr;
    }

    if (element->Name() == find_el_name) {
        --num_occurrence;
        if (num_occurrence == 0) {
            return element;
        }
    }

    if (element->FirstChildElement()) {
        auto ret = searchDFRecursive(element->FirstChildElement(), find_el_name, num_occurrence); 
        if (ret) {
            return ret;
        }
    }

    if (element->NextSiblingElement()) {
        auto ret = searchDFRecursive(element->NextSiblingElement(), find_el_name, num_occurrence); 
        if (ret) {
            return ret;
        }
    }

    return nullptr;
}

tinyxml2::XMLElement* findTarget(tinyxml2::XMLElement* element, std::string_view search_target_name) {
    constexpr std::string_view target_element_name {"Target"};
    tinyxml2::XMLElement* target_el {searchDF(element, target_element_name, 1)};

    while(target_el) {
        std::string_view target_name {target_el->FirstChildElement("TargetName")->FirstChild()->Value()};
        std::cout << target_name << std::endl;
        if (target_name == search_target_name) {
            std::cout << "Found" << std::endl;
            break;
        }
        target_el = target_el->NextSiblingElement();
    }

    return target_el;
}

int main() {
    using namespace tinyxml2;

    std::string_view keil_project_filename { "SLS_Application.uvprojx" };

    fs::path keil_project_file { keil_project_filename };
    fs::path keil_project_file_abs { fs::absolute(keil_project_file).string() };

    XMLDocument doc;
    //TODO: check for error on loadFile()
    doc.LoadFile(keil_project_file_abs.string().c_str());

    bool found {false};
    XMLElement* root = doc.RootElement();
    const char* include_path_value {};
    const char* defines_value {};

    int count {1};
    XMLElement* el {};
    do {
        int occ_needed = count;
        el = searchDFRecursive(root, "VariousControls" , occ_needed);
        if (el) {
            el = el->FirstChildElement("IncludePath");
            if (!el->NoChildren()) {
                std::cout << el->FirstChild()->Value() << std::endl;
                std::cout << count << std::endl;
            }
        }
        ++count;
    } while(el);

//    constexpr std::string_view target_name {"SLS_release_enhanced"};
//    XMLElement* target_element = findTarget(root, target_name);
//    if(!target_element) {
//        std::cout << "The target was not found!" << std::endl;
//        return 1;
//    }
//
//    const char* include_path_string {};
//    int occ {1};
//    while(true) {
//        XMLElement* element = searchDF(target_element, "IncludePath", occ);
//        if (element) {
//            if (!element->NoChildren()) {
//                include_path_string = element->FirstChild()->Value();
//                break;
//            }
//        } else {
//            std::cout << "The element: IncludePath does not exist" << std::endl;
//            return 2;
//        }
//        ++occ;
//    }
//    std::cout << "The include path string for the target is: " << include_path_string << std::endl;
//
//    const char* defines_string {};
//    occ = {1};
//    while(true) {
//        XMLElement* element = searchDF(target_element, "Define", occ);
//        if (element) {
//            if (!element->NoChildren()) {
//                defines_string = element->FirstChild()->Value();
//                break;
//            }
//        } else {
//            std::cout << "The element: Define does not exist" << std::endl;
//            return 3;
//        }
//        ++occ;
//    }
//    std::cout << "The defines string for the target is: " << defines_string << std::endl;

    //TODO: find the list of files for the target specified.
    //Files are distribute in the following way:
    //  * Groups (element) contains all the files.
    //  * Each Group represents a directory inside the keil project (e.g., sources, lib_hal, etc).
    //  * Each Group has inside a Files tag.
    //  * The Files has a File entry for each file of the group, for which the name has to be taken.
}

