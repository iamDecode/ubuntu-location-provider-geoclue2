#include <core/dbus/service.h>
#include <core/dbus/traits/service.h>
#include <core/dbus/types/struct.h>
#include <core/dbus/types/stl/tuple.h>
#include <core/dbus/types/object_path.h>
#include <chrono>
#include <core/dbus/macros.h>

namespace org {
    namespace freedesktop {
        struct Geoclue2 {
            inline static const std::string &name() {
                static const std::string s{"org.freedesktop.GeoClue2"};
                return s;
            };

            struct Manager {
                inline static const std::string &name() {
                    static const std::string s{"org.freedesktop.GeoClue2.Manager"};
                    return s;
                };
                // Timeout from where-am-i demo
                //DBUS_CPP_METHOD_WITH_TIMEOUT_DEF(GetClient, Manager, 30000)
                DBUS_CPP_METHOD_DEF(GetClient, Manager)
            };

            struct Client {
                inline static const std::string &name() {
                    static const std::string s{"org.freedesktop.GeoClue2.Client"};
                    return s;
                };

                DBUS_CPP_METHOD_DEF(Start, Client)

                DBUS_CPP_METHOD_DEF(Stop, Client)

                DBUS_CPP_SIGNAL_DEF(LocationUpdated, Client, core::dbus::types::ObjectPath)

                DBUS_CPP_WRITABLE_PROPERTY_DEF(DesktopId, Client, std::string)

                DBUS_CPP_WRITABLE_PROPERTY_DEF(DistanceThreshold, Client, unsigned int)

                DBUS_CPP_WRITABLE_PROPERTY_DEF(RequestedAccuracyLevel, Client, unsigned int)

                DBUS_CPP_WRITABLE_PROPERTY_DEF(Location, Client, core::dbus::types::ObjectPath)
            };

            struct Location {
                inline static const std::string &name() {
                    static const std::string s{"org.freedesktop.GeoClue2.Location"};
                    return s;
                };
                // Properties from python code using LOCATION_INTERFACE_NAME
                DBUS_CPP_WRITABLE_PROPERTY_DEF(Latitude, Location, double)
                DBUS_CPP_WRITABLE_PROPERTY_DEF(Longitude, Location, double)
                DBUS_CPP_WRITABLE_PROPERTY_DEF(Accuracy, Location, double)
                DBUS_CPP_WRITABLE_PROPERTY_DEF(Altitude, Location, double)
                DBUS_CPP_WRITABLE_PROPERTY_DEF(Speed, Location, double)
                DBUS_CPP_WRITABLE_PROPERTY_DEF(Heading, Location, double)
                DBUS_CPP_WRITABLE_PROPERTY_DEF(Description, Location, std::string)
            };
        };
    }
}
//namespace core {
//    namespace dbus {
//        namespace traits {
//            template<>
//                struct Service<Manager> {
//                    inline static const std::string& interface_name()
//                    {
//                        static const std::string s {"org.freedesktop.GeoClue2.Manager"};
//                        return s;
//                    }
//                };
//        }
//    }
//}
