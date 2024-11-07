#include "common_code/subsystem_parent.h"

SubsystemParent::SubsystemParent(std::string subsystem_name): kSubsystemName(subsystem_name) {}

SubsystemParent::~SubsystemParent() {}

std::string SubsystemParent::get_subsystem_name() const {
    return kSubsystemName;
}