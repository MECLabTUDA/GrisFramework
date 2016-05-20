
add_subdirectory(testLogger)
add_subdirectory(testXml)
add_subdirectory(testDynamicProperty)

#Use solution folders.

set_target_properties(testLogger PROPERTIES FOLDER Tests)
set_target_properties(testXml PROPERTIES FOLDER Tests)
set_target_properties(testDynamicProperty PROPERTIES FOLDER Tests)





