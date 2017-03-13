#ifndef ULSPROVIDERHPP
#define ULSPROVIDERHPP
#include <memory>
#include <thread>
#include <signal.h>

#include <core/dbus/dbus.h>
#include <core/dbus/asio/executor.h>
#include <core/dbus/service.h>
#include <core/dbus/types/stl/string.h>
#include <core/dbus/types/object_path.h>
#include "geoclue.h"
#include "service.hpp"

class GeoclueObject;
class ULSObject {
private:
    std::shared_ptr<core::dbus::Object> obj;
    std::shared_ptr<GeoclueObject> gcobj;
    core::dbus::Bus::Ptr bus;
public:
    ULSObject(std::shared_ptr<core::dbus::Object> obj, std::shared_ptr<GeoclueObject> gcobj, core::dbus::Bus::Ptr bus);
    void emitPositionChangedSignal(com::ubuntu::location::Position pos);
    void registerStartPositionUpdatesMethod();
    void registerStopPositionUpdatesMethod();
    void emitHeadingChangedSignal(com::ubuntu::location::Heading pos);
    void registerStartHeadingUpdatesMethod();
    void registerStopHeadingUpdatesMethod();
    void emitVelocityChangedSignal(com::ubuntu::location::Velocity pos);
    void registerStartVelocityUpdatesMethod();
    void registerStopVelocityUpdatesMethod();
    void registerRequiresMethod();
    void registerOnReferenceLocationChangedMethod();
    void registerEmitSignalMethod();
    void handle_command(core::dbus::Message::Ptr&);
};
#endif // ULSPROVIDERHPP
