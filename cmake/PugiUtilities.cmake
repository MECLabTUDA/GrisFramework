# pugi does not provide the path to the include directories in its configuration file and therefore 
# has to be fixed. This issue 
# this error has been filed as issue #126
# https://github.com/zeux/pugixml/issues/126

function(_gris_pugi_fix_imported_include_dir _IMPORT_PREFIX)
# _gris_pugi_fix_imported_include_dir adds the INTERFACE_INCLUDE_DIRECTORIES property to the
# imported pugixml target.
  get_filename_component(_IMPORT_PREFIX "${_IMPORT_PREFIX}" DIRECTORY)
  get_filename_component(_IMPORT_PREFIX "${_IMPORT_PREFIX}" DIRECTORY)
  get_filename_component(_IMPORT_PREFIX "${_IMPORT_PREFIX}" DIRECTORY)
  set_property(TARGET pugixml PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${_IMPORT_PREFIX}/include")
endfunction()

macro(gris_pugixml_find_and_fix)
# find_package and fix issue #126
  find_package(pugixml ${ARGN})
  _gris_pugi_fix_imported_include_dir(${pugixml_DIR})
endmacro()
