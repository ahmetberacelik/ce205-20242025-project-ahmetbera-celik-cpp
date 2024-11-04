#include "gtest/gtest.h"
#include "../header/recipecosting.h"
#include "../header/ingredientmanagement.h" 
#include "../header/userauthentication.h"
#include <cstdio>  
#include <cstring> 

class RecipeCostingTest : public ::testing::Test {
protected:


    void SetUp() override {

    }

    void TearDown() override {

    }

};


int main(int argc, char** argv) {
#ifdef ENABLE_RECIPECOSTING_TEST
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
#else
    return 0;
#endif
}
