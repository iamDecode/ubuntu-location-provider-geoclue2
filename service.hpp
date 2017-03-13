#ifndef SERVICEHPP
#define SERVICEHPP

#include <string>
#include <chrono>

#include <core/dbus/bus.h>
#include <core/dbus/traits/service.h>
#include <core/dbus/macros.h>
#include "position.h"

struct WolfpackMemberInterface {
    struct Signals {
        DBUS_CPP_SIGNAL_DEF(PositionChanged, WolfpackMemberInterface, com::ubuntu::location::Position)
        DBUS_CPP_SIGNAL_DEF(HeadingChanged, WolfpackMemberInterface, com::ubuntu::location::Heading)
        DBUS_CPP_SIGNAL_DEF(VelocityChanged, WolfpackMemberInterface, com::ubuntu::location::Velocity)
    };
    struct Method {
         DBUS_CPP_METHOD_DEF(Requires, WolfpackMemberInterface)
         DBUS_CPP_METHOD_DEF(OnReferenceLocationChanged, WolfpackMemberInterface)
         DBUS_CPP_METHOD_DEF(StartPositionUpdates, WolfpackMemberInterface)
         DBUS_CPP_METHOD_DEF(StopPositionUpdates, WolfpackMemberInterface)
         DBUS_CPP_METHOD_DEF(StartVelocityUpdates, WolfpackMemberInterface)
         DBUS_CPP_METHOD_DEF(StopVelocityUpdates, WolfpackMemberInterface)
         DBUS_CPP_METHOD_DEF(StartHeadingUpdates, WolfpackMemberInterface)
         DBUS_CPP_METHOD_DEF(StopHeadingUpdates, WolfpackMemberInterface)
         DBUS_CPP_METHOD_DEF(EmitSignal, WolfpackMemberInterface)
//        struct Requires {
//           typedef WolfpackService Interface;
//           inline static const std::string name() {
//               return "Requires";
//           };
//
//            inline static const std::chrono::milliseconds default_timeout() {
//              return std::chrono::milliseconds{10};
//           };
//        };
    };
};
struct WolfpackService {
};

namespace core {
    namespace dbus {
        namespace traits {
            template<>
                struct Service<WolfpackService> {
                    inline static const std::string& interface_name()
                    {
                        static const std::string s {"com.wolfpack.geoclue2.Service.Provider"};
                        return s;
                    }
                };
            template<>
                struct Service<WolfpackMemberInterface> {
                    inline static const std::string& interface_name()
                    {
                        static const std::string s {"com.ubuntu.remote.Service.Provider"};
                        return s;
                    }
                };
        }
    }
}
#endif // SERVICEHPP
