#include "geoclueProvider.hpp"
#include "geoclue2.h"
#include "service.hpp"
#include "ulsProvider.hpp"
#include <core/dbus/property.h>


using namespace core::dbus;
using namespace com::ubuntu::location;
GeoclueObject::GeoclueObject(std::shared_ptr<Object> obj, Bus::Ptr bus, std::shared_ptr<core::dbus::Service> service) {
    this->obj = obj;
    this->bus = bus;
    this->service = service;
}

Heading GeoclueObject::getHeadingFromLocation(std::shared_ptr<core::dbus::Object> lobj) {
    double heading = lobj->get_property<org::freedesktop::Geoclue2::Location::Heading>()->get();
    return heading * units::Degrees;
}
Velocity GeoclueObject::getVelocityFromLocation(std::shared_ptr<core::dbus::Object> lobj) {
    double velocity = lobj->get_property<org::freedesktop::Geoclue2::Location::Speed>()->get();
    return velocity * units::MetersPerSecond;
}
Position GeoclueObject::getPositionFromLocation(std::shared_ptr<core::dbus::Object> lobj) {
    Position pos;
    double lat = lobj->get_property<org::freedesktop::Geoclue2::Location::Latitude>()->get();
    std::cerr << "test";
    double longitude = lobj->get_property<org::freedesktop::Geoclue2::Location::Longitude>()->get();
    double altitude = lobj->get_property<org::freedesktop::Geoclue2::Location::Altitude>()->get();
    pos.latitude = wgs84::Latitude{ lat * units::Degrees};
    pos.longitude = wgs84::Longitude{ longitude * units::Degrees};
    if (altitude >= -1e+308){
        pos.altitude = wgs84::Altitude{ altitude * units::Meters };
    }
    pos.accuracy.horizontal = lobj->get_property<org::freedesktop::Geoclue2::Location::Accuracy>()->get() * units::Meters;
    pos.accuracy.vertical = lobj->get_property<org::freedesktop::Geoclue2::Location::Accuracy>()->get() * units::Meters;
    return pos;
}

void GeoclueObject::connectPropertyLocationChanged() {
//get_all_properties<org::freedesktop::Geoclue2::Client>().
    auto sig = this->client->get_signal<core::dbus::interfaces::Properties::Signals::PropertiesChanged>();
    sig->connect(
        [this](std::tuple<
                    std::string, 
                    std::map<std::string, types::Variant>,
                    std::vector<std::string>
               > args )
        {
            std::string iname;
            std::map<std::string, types::Variant> changes;
            std::vector<std::string> junk;
            std::tie(iname, changes, junk) = args;
            if (changes.find("Location") == changes.end()) {
                return;
            }
            auto path =  changes["Location"];
            auto objectPath = path.as<types::ObjectPath>();
            std::cerr << objectPath;
            this->updateUsingLocationPath(objectPath);

        }
    );
}
void GeoclueObject::updateUsingLocationPath(types::ObjectPath op) {
    auto lobj = this->service->object_for_path(op);
    if (this->status & this->client_status::position) {
        this->uobj->emitPositionChangedSignal(this->getPositionFromLocation(lobj));
    }
    if (this->status & this->client_status::velocity) {
        this->uobj->emitVelocityChangedSignal(this->getVelocityFromLocation(lobj));
    }
    if (this->status & this->client_status::heading) {
        this->uobj->emitHeadingChangedSignal(this->getHeadingFromLocation(lobj));
    }
    std::cerr << this->status;
}
void GeoclueObject::connectPositionChangedSignal() {
    auto sig = this->client->get_signal<org::freedesktop::Geoclue2::Client::LocationUpdated>();
    sig->connect(
        [this](const types::ObjectPath arg)
        {

            Position pos;

            std::cerr << "test";
            pos.latitude = wgs84::Latitude{1.0 * units::Degrees};
            pos.longitude = wgs84::Longitude{1.0 * units::Degrees};
            this->uobj->emitPositionChangedSignal(pos);
        }
    );
}

void GeoclueObject::prepareClient() {
    this->client = this->GetClient();
    auto property = this->client->get_property<org::freedesktop::Geoclue2::Client::DistanceThreshold>();
    property->set(10);
    auto ral = this->client->get_property<org::freedesktop::Geoclue2::Client::RequestedAccuracyLevel>();
    property->set(8);
}

std::shared_ptr<Object> GeoclueObject::GetClient() {
    auto res = this->obj->invoke_method_synchronously<org::freedesktop::Geoclue2::Manager::GetClient, types::ObjectPath>();
    return this->service->object_for_path(res.value());
}

void GeoclueObject::authorize() {
    auto property = this->client->get_property<org::freedesktop::Geoclue2::Client::DesktopId>();
    property->set("geoclue2-provider");
}

void GeoclueObject::startClient() {
    std::cerr << "start client";
    this->client->invoke_method_asynchronously<org::freedesktop::Geoclue2::Client::Start, void>();
}
void GeoclueObject::stopClient() {
    std::cerr << "stop client";
    this->client->invoke_method_asynchronously<org::freedesktop::Geoclue2::Client::Stop, void>();
}

void GeoclueObject::startVelocityUpdates() {
    // TODO: implement me

    auto status = this->status.fetch_or(this->client_status::velocity);
    if (!status) {
        this->startClient();
    }
    else {
        this->updateUsingLocationPath(this->client->get_property<org::freedesktop::Geoclue2::Client::Location>()->get());
    }
}

void GeoclueObject::stopVelocityUpdates() {
    // TODO: implement me
    auto status = this->status.fetch_and(~this->client_status::velocity);
    if (!(status & ~this->client_status::position)) {
        this->stopClient();
    }
}
void GeoclueObject::startHeadingUpdates() {
    // TODO: implement me

    auto status = this->status.fetch_or(this->client_status::heading);
    if (!status) {
        this->startClient();
    }
    else {
        this->updateUsingLocationPath(this->client->get_property<org::freedesktop::Geoclue2::Client::Location>()->get());
    }
}

void GeoclueObject::stopHeadingUpdates() {
    // TODO: implement me
    auto status = this->status.fetch_and(~this->client_status::heading);
    if (!(status & ~this->client_status::position)) {
        this->stopClient();
    }
}
void GeoclueObject::startPositionUpdates() {
    // TODO: implement me

    auto status = this->status.fetch_or(this->client_status::position);
    std::cerr << this->status;
    if (!status) {
        this->startClient();
    }
    else {
        this->updateUsingLocationPath(this->client->get_property<org::freedesktop::Geoclue2::Client::Location>()->get());
    }
}

void GeoclueObject::stopPositionUpdates() {
    // TODO: implement me
    auto status = this->status.fetch_and(~this->client_status::position);
    if (!(status & ~this->client_status::position)) {
        this->stopClient();
    }
}
