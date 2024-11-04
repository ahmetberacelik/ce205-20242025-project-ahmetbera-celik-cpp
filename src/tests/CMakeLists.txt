# tests/CMakeLists.txt

# Download and include GoogleTest
 include(FetchContent)
 
# OFFLINE 
# ----------------------------------------------------------------------------------
# Include GoogleTest
set(GTEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/googletest)

# Add GoogleTest as a subdirectory
add_subdirectory(${GTEST_DIR} ${CMAKE_CURRENT_BINARY_DIR}/googletest-build EXCLUDE_FROM_ALL)

# Include the Google Test framework
include_directories(${GTEST_DIR}/googletest/include)
# ----------------------------------------------------------------------------------
 
# ONLINE 
# ----------------------------------------------------------------------------------
# FetchContent_Declare(
#   googletest
#   URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
#   DOWNLOAD_EXTRACT_TIMESTAMP true
# )
# 
# # For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(googletest)
# ----------------------------------------------------------------------------------

# Include the Google Test framework
# include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# USERAUTHENTICATION tests
if(ENABLE_USERAUTHENTICATION)
	add_subdirectory(userauthentication)
endif()

# INGREDIENTMANAGEMENT tests
if(ENABLE_INGREDIENTMANAGEMENT)
	add_subdirectory(ingredientmanagement)
endif()

# RECIPECOSTING tests
if(ENABLE_RECIPECOSTING)
	add_subdirectory(recipecosting)
endif()

# PRICEADJUSTMENT tests
if(ENABLE_PRICEADJUSTMENT)
	add_subdirectory(priceadjustment)
endif()

# BUDGETPLANNER tests
if(ENABLE_BUDGETPLANNER)
	add_subdirectory(budgetplanner)
endif()
