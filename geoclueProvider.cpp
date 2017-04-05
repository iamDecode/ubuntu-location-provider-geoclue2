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
    // Transforms a Geoclue Location into a Ubuntu Location Service Heading
    //
    double heading = lobj->get_property<org::freedesktop::Geoclue2::Location::Heading>()->get();
    return heading * units::Degrees;
}

Velocity GeoclueObject::getVelocityFromLocation(std::shared_ptr<core::dbus::Object> lobj) {
    // Transforms a Geoclue Location into a Ubuntu Location Service Velocity

    double velocity = lobj->get_property<org::freedesktop::Geoclue2::Location::Speed>()->get();
    return velocity * units::MetersPerSecond;
}

Position GeoclueObject::getPositionFromLocation(std::shared_ptr<core::dbus::Object> lobj) {
    // Transforms a Geoclue Location into a Ubuntu Location Service Position

    Position pos;

    double lat = lobj->get_property<org::freedesktop::Geoclue2::Location::Latitude>()->get();
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

void GeoclueObject::updateUsingLocationPath(types::ObjectPath op) {
    // If available, updates the position, velocity, and heading
    auto lobj = this->service->object_for_path(op);
    // During testing, GeoClue sometimes claims not to support position even though the documentation said it does. Therefore, to not crash the program, we catch the exception
    try {
        if (this->status & this->client_status::position) {
            this->uobj->emitPositionChangedSignal(this->getPositionFromLocation(lobj));
        }
    }
    catch (...){
        std::cerr << "Geoclue doesn't support position" << "\n";
    }
    
    // During testing, GeoClue sometimes claims not to support speed even though the documentation claims it does. Therefore, to not crash the program, we catch the exception
    try {
        if (this->status & this->client_status::velocity) {
            this->uobj->emitVelocityChangedSignal(this->getVelocityFromLocation(lobj));
        }
    }
    catch (...){
        std::cerr << "Geoclue doesn't support speed" << "\n";
    }
    // During testing, GeoClue sometimes claims not to support heading even though the documentation claims it does. Therefore, to not crash the program, we catch the exception
    try {
        if (this->status & this->client_status::heading) {
            this->uobj->emitHeadingChangedSignal(this->getHeadingFromLocation(lobj));
        }
    }
    catch (...){
        std::cerr << "Geoclue doesn't support heading" << "\n";
    }
}
void GeoclueObject::connectPositionChangedSignal() {
    // Adds a signal handler to handle location updates from Geoclue
    this->luSignal = this->client->get_signal<org::freedesktop::Geoclue2::Client::LocationUpdated>();
    this->luSignal->connect(
        [this](const std::tuple<types::ObjectPath,types::ObjectPath> arg)
        {
            types::ObjectPath before;
            types::ObjectPath after;
            std::tie(before, after) = arg;
            std::cerr << "Geoclue position changed" << "\n";
            this->updateUsingLocationPath(after);
        }
    );
}

void GeoclueObject::prepareClient() {
    /*
     * We set the 'DistanceThreshold' property before starting the client. This property
     * decides how often the 'LocationUpdated' signal is emitted. If the distance moved is below
     * threshold, the signal won't be emitted. We have set this threshold to 10 meters.
     * We set the 'RequestedAccuracy Level' property as well. This property is used to specify
     * the level of accuracy requested by, or allowed by the client. We have set this to 8.
    */

    std::cerr << "Prepare client";
    this->client = this->GetClient();
    std::cerr << client->path();
    auto property = this->client->get_property<org::freedesktop::Geoclue2::Client::DistanceThreshold>();
    property->set(10);
    auto ral = this->client->get_property<org::freedesktop::Geoclue2::Client::RequestedAccuracyLevel>();
    ral->set(8);
}

std::shared_ptr<Object> GeoclueObject::GetClient() {
    // This method returns the path to the newly created client

    auto res = this->obj->invoke_method_synchronously<org::freedesktop::Geoclue2::Manager::GetClient, types::ObjectPath>();
    return this->service->object_for_path(res.value());
}

void GeoclueObject::authorize() {
    // Authorizes the client by setting the DesktopId property

    auto property = this->client->get_property<org::freedesktop::Geoclue2::Client::DesktopId>();
    property->set("geoclue2-provider");
}

void GeoclueObject::startClient() {
    // Start the Geoclue Client
    std::cerr << "Start client" << "\n";
    this->prepareClient();
    this->connectPositionChangedSignal();
    this->authorize();
    this->client->invoke_method_asynchronously<org::freedesktop::Geoclue2::Client::Start, void>();
    auto current_loc = this->client->get_property<org::freedesktop::Geoclue2::Client::Location>()->get();
    // If the location property is already set, use it
    if (current_loc != types::ObjectPath("/")){
        this->updateUsingLocationPath(current_loc);
    }
    
}

void GeoclueObject::stopClient() {
    // Stops the Geoclue client again
    std::cerr << "Stop client" << "\n";
    this->client->invoke_method_synchronously<org::freedesktop::Geoclue2::Client::Stop, void>();
}

void GeoclueObject::startVelocityUpdates() {
    auto status = this->status.fetch_or(this->client_status::velocity);
    if (!status) {
        this->startClient();
    }
    else {
        this->updateUsingLocationPath(this->client->get_property<org::freedesktop::Geoclue2::Client::Location>()->get());
    }
}

void GeoclueObject::stopVelocityUpdates() {
    auto status = this->status.fetch_and(~this->client_status::velocity);
    if (!(status & ~this->client_status::position)) {
        this->stopClient();
    }
}

void GeoclueObject::startHeadingUpdates() {
    auto status = this->status.fetch_or(this->client_status::heading);
    if (!status) {
        this->startClient();
    }
    else {
        this->updateUsingLocationPath(this->client->get_property<org::freedesktop::Geoclue2::Client::Location>()->get());
    }
}

void GeoclueObject::stopHeadingUpdates() {
    auto status = this->status.fetch_and(~this->client_status::heading);
    if (!(status & ~this->client_status::position)) {
        this->stopClient();
    }
}

void GeoclueObject::startPositionUpdates() {
    // Start receiving events about current location

    auto status = this->status.fetch_or(this->client_status::position);
    if (!status) {
        this->startClient();
    }
    else {
        this->updateUsingLocationPath(this->client->get_property<org::freedesktop::Geoclue2::Client::Location>()->get());
    }
}

void GeoclueObject::stopPositionUpdates() {
    auto status = this->status.fetch_and(~this->client_status::position);
    if (!(status & ~this->client_status::position)) {
        this->stopClient();
    }
}
