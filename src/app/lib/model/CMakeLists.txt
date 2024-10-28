set(ModelFiles equityModel.cpp profileModel.cpp refreshModel.cpp searchModel.cpp settingsModel.cpp signInModel.cpp signUpModel.cpp)
add_library(logicModel STATIC ${ModelFiles})
target_include_directories(logicModel PUBLIC include)
target_link_libraries(logicModel databaseClientInterface stockApiInterface timeApiSTATIC)