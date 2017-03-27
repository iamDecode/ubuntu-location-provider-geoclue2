#ifndef GEOCLUE2_H
#define GEOCLUE2_H
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
                DBUS_CPP_METHOD_DEF(GetClient, Manager)
            };

            struct Client {
                inline static const std::string &name() {
                    static const std::string s{"org.freedesktop.GeoClue2.Client"};
                    return s;
                };

                DBUS_CPP_METHOD_DEF(Start, Client)

                DBUS_CPP_METHOD_DEF(Stop, Client)

		// LocationUpdated Signal
		struct LocationUpdated 
		{
		    inline static const std::string &name()
		    { 
                        static const std::string s{"LocationUpdated"};
			return s;
		    };
		    typedef Client Interface;
		    typedef std::tuple<core::dbus::types::ObjectPath,core::dbus::types::ObjectPath> ArgumentType; 
		}; 

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
#endif  //GEOCLUE2_H
