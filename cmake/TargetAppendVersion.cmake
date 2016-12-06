macro(_gris_append_version target version)
  set_property(TARGET ${target} PROPERTY VERSION      ${version})
  set_property(TARGET ${target} PROPERTY SOVERSION    ${version})
  set_property(TARGET ${target} PROPERTY OUTPUT_NAME  "${target}-${version}")
endmacro()

macro(_gris_append_product_version target product)
  _gris_append_version(${target} ${${product}_SHORT_VERSION})
endmacro()