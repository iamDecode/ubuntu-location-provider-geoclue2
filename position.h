/*
 * Copyright © 2012-2013 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License version 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Thomas Voß <thomas.voss@canonical.com>
 */
#ifndef LOCATION_SERVICE_COM_UBUNTU_LOCATION_POSITION_H_
#define LOCATION_SERVICE_COM_UBUNTU_LOCATION_POSITION_H_

#include "altitude.h"
#include "latitude.h"
#include "longitude.h"

#include "optional.h"
#include "units.h"

#include <bitset>
#include <ostream>

#include <core/dbus/codec.h>

namespace com
{
    namespace ubuntu
    {
        namespace location
        {
            /**
             * @brief The Position struct models a position in the wgs84 coordinate system.
             */
            struct Position
            {
                struct Accuracy
                {
                    typedef units::Quantity<units::Length> Horizontal;
                    typedef units::Quantity<units::Length> Vertical;

                    Optional<Horizontal> horizontal{};
                    Optional<Vertical> vertical{};
                };

                wgs84::Latitude latitude = wgs84::Latitude{};
                wgs84::Longitude longitude = wgs84::Longitude{};
                Optional<wgs84::Altitude> altitude = Optional<wgs84::Altitude>{};
                Accuracy accuracy{};
            };
            typedef units::Quantity<units::PlaneAngle> Heading;
            typedef units::Quantity<units::Velocity> Velocity;
        }
    }
}

using namespace com::ubuntu;

namespace core
    {
    namespace dbus
    {
        namespace helper
        {
            template<typename T>
            struct TypeMapper<com::ubuntu::location::units::Quantity<T>>
            {
                constexpr static ArgumentType type_value()
                {
                    return ArgumentType::floating_point;
                }

                constexpr static bool is_basic_type()
                {
                    return true;
                }
                constexpr static bool requires_signature()
                {
                    return false;
                }

                static std::string signature()
                {
                    static const std::string s = TypeMapper<double>::signature();
                    return s;
                }
            };
        }

    template<typename T>
    struct Codec<com::ubuntu::location::Optional<T>>
    {
        static void encode_argument(Message::Writer& writer, const com::ubuntu::location::Optional<T>& in)
        {
            bool has_value{in};
            Codec<bool>::encode_argument(writer, has_value);
            if (has_value)
                Codec<typename com::ubuntu::location::Optional<T>::value_type>::encode_argument(writer, *in);
        }

        static void decode_argument(Message::Reader& reader, com::ubuntu::location::Optional<T>& in)
        {
            bool has_value{false};
            Codec<bool>::decode_argument(reader, has_value);
            if (has_value)
                {
                    typename com::ubuntu::location::Optional<T>::value_type value;
                    Codec<typename com::ubuntu::location::Optional<T>::value_type>::decode_argument(reader, value);
                    in = value;
                } else
                {
                    in.reset();
                }
        }
    };

    template<typename T>
    struct Codec<com::ubuntu::location::units::Quantity<T>>
    {
        static void encode_argument(Message::Writer& writer, const com::ubuntu::location::units::Quantity<T>& in)
        {
            Codec<typename com::ubuntu::location::units::Quantity<T>::value_type>::encode_argument(writer, in.value());
        }

        static void decode_argument(Message::Reader& reader, com::ubuntu::location::units::Quantity<T>& in)
        {
            typename com::ubuntu::location::units::Quantity<T>::value_type value;
            Codec<typename com::ubuntu::location::units::Quantity<T>::value_type>::decode_argument(reader, value);
            in = com::ubuntu::location::units::Quantity<T>::from_value(value);
        }
    };

    template<typename T, typename U>
    struct Codec<com::ubuntu::location::wgs84::Coordinate<T,U>>
    {
        static void encode_argument(Message::Writer& writer, const com::ubuntu::location::wgs84::Coordinate<T, U>& in)
        {
            Codec<com::ubuntu::location::units::Quantity<U>>::encode_argument(writer, in.value);
        }

        static void decode_argument(Message::Reader& reader, com::ubuntu::location::wgs84::Coordinate<T, U>& in)
        {
            Codec<com::ubuntu::location::units::Quantity<U>>::decode_argument(reader, in.value);
        }
    };

        template<>
        struct Codec<com::ubuntu::location::Position>
        {
            typedef com::ubuntu::location::Position::Accuracy::Horizontal HorizontalAccuracy;
            typedef com::ubuntu::location::Position::Accuracy::Vertical VerticalAccuracy;

            static void encode_argument(Message::Writer& writer, const com::ubuntu::location::Position& in)
            {
                Codec<com::ubuntu::location::wgs84::Latitude>::encode_argument(writer, in.latitude);
                Codec<com::ubuntu::location::wgs84::Longitude>::encode_argument(writer, in.longitude);
                Codec<com::ubuntu::location::Optional<com::ubuntu::location::wgs84::Altitude>>::encode_argument(writer, in.altitude);

                Codec<com::ubuntu::location::Optional<HorizontalAccuracy>>::encode_argument(writer, in.accuracy.horizontal);
                Codec<com::ubuntu::location::Optional<VerticalAccuracy>>::encode_argument(writer, in.accuracy.vertical);
            }

            static void decode_argument(Message::Reader& reader, com::ubuntu::location::Position& in)
            {
                Codec<com::ubuntu::location::wgs84::Latitude>::decode_argument(reader, in.latitude);
                Codec<com::ubuntu::location::wgs84::Longitude>::decode_argument(reader, in.longitude);
                Codec<com::ubuntu::location::Optional<com::ubuntu::location::wgs84::Altitude>>::decode_argument(reader, in.altitude);

                Codec<com::ubuntu::location::Optional<HorizontalAccuracy>>::decode_argument(reader, in.accuracy.horizontal);
                Codec<com::ubuntu::location::Optional<VerticalAccuracy>>::decode_argument(reader, in.accuracy.vertical);
            }
        };
    }
}
#endif // LOCATION_SERVICE_COM_UBUNTU_LOCATION_POSITION_H_
