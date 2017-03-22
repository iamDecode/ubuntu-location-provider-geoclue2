
#include "ulsProvider.hpp"
#include "geoclueProvider.hpp"

using namespace core::dbus;
using namespace com::ubuntu::location;

void ULSObject::emitPositionChangedSignal(Position pos) {
    auto sig = this->obj->get_signal<WolfpackMemberInterface::Signals::PositionChanged>();
    sig->emit(pos);
}

void ULSObject::emitVelocityChangedSignal(Velocity pos) {
    auto sig = this->obj->get_signal<WolfpackMemberInterface::Signals::VelocityChanged>();
    sig->emit(pos);
}

void ULSObject::emitHeadingChangedSignal(Heading pos) {
    auto sig = this->obj->get_signal<WolfpackMemberInterface::Signals::HeadingChanged>();
    sig->emit(pos);
}

void ULSObject::registerRequiresMethod() {
    //ADD COMMENT HERE
    std::function<void(const core::dbus::Message::Ptr&)> func = [this](const core::dbus::Message::Ptr& msg){

        int r; 
        msg->reader() >> r;
        auto reply = Message::make_method_return(msg);
        reply->writer() << true;

        this->bus->send(reply);
 
    }; //std::bind(&ULSObject::handle_command, this, std::placeholders::_1);
    this->obj->install_method_handler<WolfpackMemberInterface::Method::Requires>(func); /// Pointer to signal
    //this->obj->install_method_handler<WolfpackService::Method::Requires>(); /// Pointer to signal
}

void ULSObject::registerStartHeadingUpdatesMethod() {
    std::function<void(const core::dbus::Message::Ptr&)> func = [this](const core::dbus::Message::Ptr& msg){

        auto reply = Message::make_method_return(msg);

        this->gcobj->startHeadingUpdates();

        this->bus->send(reply);
 
    };
    this->obj->install_method_handler<WolfpackMemberInterface::Method::StartHeadingUpdates>(func);
}

void ULSObject::registerStopHeadingUpdatesMethod() {
    std::function<void(const core::dbus::Message::Ptr&)> func = [this](const core::dbus::Message::Ptr& msg){

        auto reply = Message::make_method_return(msg);

        this->gcobj->stopHeadingUpdates();

        this->bus->send(reply);
 
    };
    this->obj->install_method_handler<WolfpackMemberInterface::Method::StopHeadingUpdates>(func);
}
void ULSObject::registerStartVelocityUpdatesMethod() {
    //ADD COMMENT HERE
    std::function<void(const core::dbus::Message::Ptr&)> func = [this](const core::dbus::Message::Ptr& msg){

        auto reply = Message::make_method_return(msg);

        this->gcobj->startVelocityUpdates();

        this->bus->send(reply);
 
    };
    this->obj->install_method_handler<WolfpackMemberInterface::Method::StartVelocityUpdates>(func);
}

void ULSObject::registerStopVelocityUpdatesMethod() {
    std::function<void(const core::dbus::Message::Ptr&)> func = [this](const core::dbus::Message::Ptr& msg){

        auto reply = Message::make_method_return(msg);

        this->gcobj->stopVelocityUpdates();

        this->bus->send(reply);
 
    };
    this->obj->install_method_handler<WolfpackMemberInterface::Method::StopVelocityUpdates>(func);
}

void ULSObject::registerStartPositionUpdatesMethod() {
    std::function<void(const core::dbus::Message::Ptr&)> func = [this](const core::dbus::Message::Ptr& msg){

        auto reply = Message::make_method_return(msg);

        this->gcobj->startPositionUpdates();

        this->bus->send(reply);
 
    };
    this->obj->install_method_handler<WolfpackMemberInterface::Method::StartPositionUpdates>(func);
}

void ULSObject::registerStopPositionUpdatesMethod() {
    //ADD COMMENT HERE
    std::function<void(const core::dbus::Message::Ptr&)> func = [this](const core::dbus::Message::Ptr& msg){

        auto reply = Message::make_method_return(msg);

        this->gcobj->stopPositionUpdates();

        this->bus->send(reply);
 
    };
    this->obj->install_method_handler<WolfpackMemberInterface::Method::StopPositionUpdates>(func);
}

void ULSObject::registerOnReferenceLocationChangedMethod() {
     //ADD COMMENT HERE
     std::function<void(const core::dbus::Message::Ptr&)> func = [this](const core::dbus::Message::Ptr& msg){

        // Only implemented because it doesn't work otherwise
        auto reply = Message::make_method_return(msg);

        this->bus->send(reply);
 
    }; 
    this->obj->install_method_handler<WolfpackMemberInterface::Method::OnReferenceLocationChanged>(func);
}

void ULSObject::registerEmitSignalMethod() {
    //ADD COMMENT HERE
    std::function<void(const core::dbus::Message::Ptr&)> func = [this](const core::dbus::Message::Ptr& msg){

        Position pos;

        pos.latitude = wgs84::Latitude{1.0 * units::Degrees};
        pos.longitude = wgs84::Longitude{1.0 * units::Degrees};
        this->emitPositionChangedSignal(pos);
        auto reply = Message::make_method_return(msg);

        this->bus->send(reply);
 
    }; 
    this->obj->install_method_handler<WolfpackMemberInterface::Method::EmitSignal>(func); // Pointer to signal
}

ULSObject::ULSObject(std::shared_ptr<core::dbus::Object> obj, std::shared_ptr<GeoclueObject> gcobj, Bus::Ptr bus) {
    this->obj = obj;
    this->gcobj = gcobj;
    this->bus = bus;
}
