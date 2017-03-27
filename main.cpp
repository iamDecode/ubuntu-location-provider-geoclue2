#include "ulsProvider.hpp"
#include "geoclueProvider.hpp"
#include "geoclue2.h"
using namespace core::dbus;
using namespace com::ubuntu::location;
int main(int, char**){
    // Get the right bus
    Bus::Ptr BUS = std::make_shared<Bus>(WellKnownBus::system);
    BUS->install_executor(asio::make_executor(BUS));

    // Create service on the bus
    auto service = Service::add_service<WolfpackService>(BUS);
    auto gcService = Service::use_service<org::freedesktop::Geoclue2>(BUS);

    // Add object
    auto obj = service->add_object_for_path(types::ObjectPath("/com/wolfpack/geoclue2/Service/Provider"));
    auto gcobj = gcService->object_for_path(types::ObjectPath("/org/freedesktop/GeoClue2/Manager"));

    auto gcObj = std::make_shared<GeoclueObject>(gcobj, BUS, gcService);
    auto ulsObj = std::make_shared<ULSObject>(obj, gcObj, BUS);
    gcObj->uobj = ulsObj;
    ulsObj->registerRequiresMethod();
    ulsObj->registerStartPositionUpdatesMethod();
    ulsObj->registerStopPositionUpdatesMethod();
    ulsObj->registerStartVelocityUpdatesMethod();
    ulsObj->registerStopVelocityUpdatesMethod();
    ulsObj->registerStartHeadingUpdatesMethod();
    ulsObj->registerStopHeadingUpdatesMethod();
    ulsObj->registerOnReferenceLocationChangedMethod();
    gcObj->prepareClient();
    gcObj->connectPositionChangedSignal();
    gcObj->authorize();
    BUS->run();
}
