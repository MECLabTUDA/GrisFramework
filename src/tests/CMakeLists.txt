FUNCTION(_gris_add_test target)
add_subdirectory(${target})
#Use solution folders.
set_target_properties(${target} PROPERTIES FOLDER Tests)
ENDFUNCTION(_gris_add_test)


_gris_add_test(testLogger)
_gris_add_test(testXml)
_gris_add_test(testDynamicProperty)
_gris_add_test(testInvoker)
_gris_add_test(testVector)