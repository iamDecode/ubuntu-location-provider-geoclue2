configure_file(
  ubuntu-location-provider-geoclue2.in ubuntu-location-provider-geoclue2 @ONLY
)

configure_file(
  com.wolfpack.geoclue2.Service.Provider.conf.in com.wolfpack.geoclue2.Service.Provider.conf @ONLY
)

install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/ubuntu-location-provider-geoclue2
  DESTINATION /etc/init.d/
)

install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/com.wolfpack.geoclue2.Service.Provider.conf
  PERMISSIONS OWNER_EXECUTE
  DESTINATION /etc/dbus-1/system.d/
)
