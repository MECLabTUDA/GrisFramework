macro(_gris_find_inc var path)
  file(RELATIVE_PATH ${var} ${CMAKE_CURRENT_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/include/${path}/")
endmacro()