
#include "ulsProvider.hpp"
#include "geoclueProvider.hpp"

using namespace core::dbus;
using namespace com::ubuntu::location;

void ULSObject::emitPositionChangedSignal(Position pos) {
    // Notifies ULS of changed position
    auto sig = this->obj->get_signal<WolfpackMemberInterface::Signals::PositionChanged>();
    sig->emit(pos);
}

void ULSObject::emitVelocityChangedSignal(Velocity pos) {
    // Notifies ULS of changed velocity
    auto sig = this->obj->get_signal<WolfpackMemberInterface::Signals::VelocityChanged>();
    sig->emit(pos);
}

void ULSObject::emitHeadingChangedSignal(Heading pos) {
    // Notifies ULS of changed heading
    auto sig = this->obj->get_signal<WolfpackMemberInterface::Signals::HeadingChanged>();
    sig->emit(pos);
}

void ULSObject::registerRequiresMethod() {
    // We need ot register the require method, so the ULS understands we have data for it
    std::function<void(const core::dbus::Message::Ptr&)> func = [this](const core::dbus::Message::Ptr& msg){

        int r; 
        msg->reader() >> r;
        auto reply = Message::make_method_return(msg);
        reply->writer() << true;

        this->bus->send(reply);
 
    }; 
    this->obj->install_method_handler<WolfpackMemberInterface::Method::Requires>(func); /// Pointer to signal
}

void ULSObject::registerStartHeadingUpdatesMethod() {
    // Enables Heading updates if ULS asks for it
    std::function<void(const core::dbus::Message::Ptr&)> func = [this](const core::dbus::Message::Ptr& msg){
        std::cerr << "Start heading updates" << "\n";

        auto reply = Message::make_method_return(msg);

        this->gcobj->startHeadingUpdates();

        this->bus->send(reply);
 
    };
    this->obj->install_method_handler<WolfpackMemberInterface::Method::StartHeadingUpdates>(func);
}

void ULSObject::registerStopHeadingUpdatesMethod() {
    // Disables Heading updates if ULS asks for it
    std::function<void(const core::dbus::Message::Ptr&)> func = [this](const core::dbus::Message::Ptr& msg){

        std::cerr << "Stop heading updates" << "\n";

        auto reply = Message::make_method_return(msg);

        this->gcobj->stopHeadingUpdates();

        this->bus->send(reply);
 
    };
    this->obj->install_method_handler<WolfpackMemberInterface::Method::StopHeadingUpdates>(func);
}
void ULSObject::registerStartVelocityUpdatesMethod() {
    // Enables Velocity updates if ULS asks for it
    std::function<void(const core::dbus::Message::Ptr&)> func = [this](const core::dbus::Message::Ptr& msg){

        std::cerr << "Start velocity updates" << "\n";
        auto reply = Message::make_method_return(msg);

        this->gcobj->startVelocityUpdates();

        this->bus->send(reply);
 
    };
    this->obj->install_method_handler<WolfpackMemberInterface::Method::StartVelocityUpdates>(func);
}

void ULSObject::registerStopVelocityUpdatesMethod() {
    // Disables Velocity updates if ULS asks for it
    std::function<void(const core::dbus::Message::Ptr&)> func = [this](const core::dbus::Message::Ptr& msg){

        std::cerr << "Stop velocity updates" << "\n";
        auto reply = Message::make_method_return(msg);

        this->gcobj->stopVelocityUpdates();

        this->bus->send(reply);
 
    };
    this->obj->install_method_handler<WolfpackMemberInterface::Method::StopVelocityUpdates>(func);
}

void ULSObject::registerStartPositionUpdatesMethod() {
    // Enables Position updates if ULS asks for it
    std::function<void(const core::dbus::Message::Ptr&)> func = [this](const core::dbus::Message::Ptr& msg){

        std::cerr << "Start position updates" << "\n";
        auto reply = Message::make_method_return(msg);

        this->gcobj->startPositionUpdates();

        this->bus->send(reply);
 
    };
    this->obj->install_method_handler<WolfpackMemberInterface::Method::StartPositionUpdates>(func);
}

void ULSObject::registerStopPositionUpdatesMethod() {
    // Disables Position updates if ULS asks for it
    std::function<void(const core::dbus::Message::Ptr&)> func = [this](const core::dbus::Message::Ptr& msg){

        std::cerr << "Stop position updates" << "\n";
        auto reply = Message::make_method_return(msg);

        this->gcobj->stopPositionUpdates();

        this->bus->send(reply);
 
    };
    this->obj->install_method_handler<WolfpackMemberInterface::Method::StopPositionUpdates>(func);
}

void ULSObject::registerOnReferenceLocationChangedMethod() {
     // This method needs to be implemented, because the ULS asks for it
     // Doesn't do anything at the moment
     std::function<void(const core::dbus::Message::Ptr&)> func = [this](const core::dbus::Message::Ptr& msg){

        // Only implemented because it doesn't work otherwise
        auto reply = Message::make_method_return(msg);

        this->bus->send(reply);
 
    }; 
    this->obj->install_method_handler<WolfpackMemberInterface::Method::OnReferenceLocationChanged>(func);
}

ULSObject::ULSObject(std::shared_ptr<core::dbus::Object> obj, std::shared_ptr<GeoclueObject> gcobj, Bus::Ptr bus) {
    this->obj = obj;
    this->gcobj = gcobj;
    this->bus = bus;
}
