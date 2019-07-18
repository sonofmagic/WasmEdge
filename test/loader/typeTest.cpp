//===-- ssvm/test/loader/typeTest.cpp - AST type unit tests ---------------===//
//
// Part of the SSVM Project.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file contents unit tests of AST type nodes.
///
//===----------------------------------------------------------------------===//

#include "loader/type.h"
#include "filemgrTest.h"
#include "gtest/gtest.h"

namespace {

FileMgrTest Mgr;

TEST(TypeTest, LoadLimit) {
  /// 1. Test load limit.
  ///
  ///   1.  Load invalid empty limit.
  ///   2.  Load invalid types of limit.
  ///   3.  Load limit with only min.
  ///   4.  Load invalid limit with fail of loading max.
  ///   5.  Load limit with min and max.
  Mgr.clearBuffer();
  AST::Limit Lim1;
  EXPECT_FALSE(Lim1.loadBinary(Mgr));

  Mgr.clearBuffer();
  std::vector<unsigned char> Vec2 = {
      0x02U, /// Unknown limit type
      0x00U  /// Min = 0
  };
  Mgr.setVector(Vec2);
  AST::Limit Lim2;
  EXPECT_FALSE(Lim2.loadBinary(Mgr));

  Mgr.clearBuffer();
  std::vector<unsigned char> Vec3 = {
      0x00U,                            /// Only has min
      0xFFU, 0xFFU, 0xFFU, 0xFFU, 0x0FU /// Min = 4294967295
  };
  Mgr.setVector(Vec3);
  AST::Limit Lim3;
  EXPECT_TRUE(Lim3.loadBinary(Mgr) && Mgr.getQueueSize() == 0);

  Mgr.clearBuffer();
  std::vector<unsigned char> Vec4 = {
      0x01U,                            /// Has min and max
      0xFFU, 0xFFU, 0xFFU, 0xFFU, 0x0FU /// Min = 4294967295
  };
  Mgr.setVector(Vec4);
  AST::Limit Lim4;
  EXPECT_FALSE(Lim4.loadBinary(Mgr));

  Mgr.clearBuffer();
  std::vector<unsigned char> Vec5 = {
      0x01U,                             /// Has min and max
      0xF1U, 0xFFU, 0xFFU, 0xFFU, 0x0FU, /// Min = 4294967281
      0xFFU, 0xFFU, 0xFFU, 0xFFU, 0x0FU  /// Max = 4294967295
  };
  Mgr.setVector(Vec5);
  AST::Limit Lim5;
  EXPECT_TRUE(Lim5.loadBinary(Mgr) && Mgr.getQueueSize() == 0);
}

TEST(TypeTest, LoadFunctionType) {
  /// 2. Test load function type.
  ///
  ///   1.  Load invalid empty function type.
  ///   2.  Load invalid types of function type.
  ///   3.  Load void parameter and result function type.
  ///   4.  Load non-void parameter function type.
  ///   5.  Load non-void result function type.
  ///   6.  Load function type with parameters and result.
  Mgr.clearBuffer();
  AST::FunctionType Fun1;
  EXPECT_FALSE(Fun1.loadBinary(Mgr));

  Mgr.clearBuffer();
  std::vector<unsigned char> Vec2 = {0xFFU, /// Invalid function type header
                                     0x00U, 0x00U};
  Mgr.setVector(Vec2);
  AST::FunctionType Fun2;
  EXPECT_FALSE(Fun2.loadBinary(Mgr));

  Mgr.clearBuffer();
  std::vector<unsigned char> Vec3 = {
      0x60U, /// Function type header
      0x00U, /// Parameter length = 0
      0x00U  /// Result length = 0
  };
  Mgr.setVector(Vec3);
  AST::FunctionType Fun3;
  EXPECT_TRUE(Fun3.loadBinary(Mgr) && Mgr.getQueueSize() == 0);

  Mgr.clearBuffer();
  std::vector<unsigned char> Vec4 = {
      0x60U,                      /// Function type header
      0x04U,                      /// Parameter length = 4
      0x7CU, 0x7DU, 0x7EU, 0x7FU, /// Parameter list
      0x00U                       /// Result length = 0
  };
  Mgr.setVector(Vec4);
  AST::FunctionType Fun4;
  EXPECT_TRUE(Fun4.loadBinary(Mgr) && Mgr.getQueueSize() == 0);

  Mgr.clearBuffer();
  std::vector<unsigned char> Vec5 = {
      0x60U, /// Function type header
      0x00U, /// Parameter length = 0
      0x01U, /// Result length = 1 (must be <= 1 now)
      0x7CU  /// Result list
  };
  Mgr.setVector(Vec5);
  AST::FunctionType Fun5;
  EXPECT_TRUE(Fun5.loadBinary(Mgr) && Mgr.getQueueSize() == 0);

  Mgr.clearBuffer();
  std::vector<unsigned char> Vec6 = {
      0x60U,                      /// Function type header
      0x04U,                      /// Parameter length = 4
      0x7CU, 0x7DU, 0x7EU, 0x7FU, /// Parameter list
      0x01U,                      /// Result length = 1 (must be <= 1 now)
      0x7CU                       /// Result list
  };
  Mgr.setVector(Vec6);
  AST::FunctionType Fun6;
  EXPECT_TRUE(Fun6.loadBinary(Mgr) && Mgr.getQueueSize() == 0);
}

TEST(TypeTest, LoadMemoryType) {
  /// 1. Test load memory type, which is limit.
  ///
  ///   1.  Load invalid empty limit.
  ///   2.  Load invalid types of limit.
  ///   3.  Load limit with only min.
  ///   4.  Load invalid limit with fail of loading max.
  ///   5.  Load limit with min and max.
  Mgr.clearBuffer();
  AST::MemoryType Mem1;
  EXPECT_FALSE(Mem1.loadBinary(Mgr));

  Mgr.clearBuffer();
  std::vector<unsigned char> Vec2 = {
      0x02U, /// Unknown limit type
      0x00U  /// Min = 0
  };
  Mgr.setVector(Vec2);
  AST::MemoryType Mem2;
  EXPECT_FALSE(Mem2.loadBinary(Mgr));

  Mgr.clearBuffer();
  std::vector<unsigned char> Vec3 = {
      0x00U,                            /// Only has min
      0xFFU, 0xFFU, 0xFFU, 0xFFU, 0x0FU /// Min = 4294967295
  };
  Mgr.setVector(Vec3);
  AST::MemoryType Mem3;
  EXPECT_TRUE(Mem3.loadBinary(Mgr) && Mgr.getQueueSize() == 0);

  Mgr.clearBuffer();
  std::vector<unsigned char> Vec4 = {
      0x01U,                            /// Has min and max
      0xFFU, 0xFFU, 0xFFU, 0xFFU, 0x0FU /// Min = 4294967295
  };
  Mgr.setVector(Vec4);
  AST::MemoryType Mem4;
  EXPECT_FALSE(Mem4.loadBinary(Mgr));

  Mgr.clearBuffer();
  std::vector<unsigned char> Vec5 = {
      0x01U,                             /// Has min and max
      0xF1U, 0xFFU, 0xFFU, 0xFFU, 0x0FU, /// Min = 4294967281
      0xFFU, 0xFFU, 0xFFU, 0xFFU, 0x0FU  /// Max = 4294967295
  };
  Mgr.setVector(Vec5);
  AST::MemoryType Mem5;
  EXPECT_TRUE(Mem5.loadBinary(Mgr) && Mgr.getQueueSize() == 0);
}

TEST(TypeTest, LoadTableType) {
  /// 1. Test load table type, which is element type and limit.
  ///
  ///   1.  Load invalid empty table type.
  ///   2.  Load invalid element type.
  ///   3.  Load invalid types of limit in table type.
  ///   4.  Load limit with only min.
  ///   5.  Load invalid limit with fail of loading max.
  ///   6.  Load limit with min and max.
  Mgr.clearBuffer();
  AST::TableType Tab1;
  EXPECT_FALSE(Tab1.loadBinary(Mgr));

  Mgr.clearBuffer();
  std::vector<unsigned char> Vec2 = {
      0xFFU, /// Unknown element type
      0x00U, /// Limit with only has min
      0x00U  /// Min = 0
  };
  Mgr.setVector(Vec2);
  AST::TableType Tab2;
  EXPECT_FALSE(Tab2.loadBinary(Mgr));

  Mgr.clearBuffer();
  std::vector<unsigned char> Vec3 = {
      0x70U, /// Element type
      0x02U, /// Unknown limit type
      0x00U  /// Min = 0
  };
  Mgr.setVector(Vec3);
  AST::TableType Tab3;
  EXPECT_FALSE(Tab3.loadBinary(Mgr));

  Mgr.clearBuffer();
  std::vector<unsigned char> Vec4 = {
      0x70U,                            /// Element type
      0x00U,                            /// Only has min
      0xFFU, 0xFFU, 0xFFU, 0xFFU, 0x0FU /// Min = 4294967295
  };
  Mgr.setVector(Vec4);
  AST::TableType Tab4;
  EXPECT_TRUE(Tab4.loadBinary(Mgr) && Mgr.getQueueSize() == 0);

  Mgr.clearBuffer();
  std::vector<unsigned char> Vec5 = {
      0x70U,                            /// Element type
      0x01U,                            /// Has min and max
      0xFFU, 0xFFU, 0xFFU, 0xFFU, 0x0FU /// Min = 4294967295
  };
  Mgr.setVector(Vec5);
  AST::TableType Tab5;
  EXPECT_FALSE(Tab5.loadBinary(Mgr));

  Mgr.clearBuffer();
  std::vector<unsigned char> Vec6 = {
      0x70U,                             /// Element type
      0x01U,                             /// Has min and max
      0xF1U, 0xFFU, 0xFFU, 0xFFU, 0x0FU, /// Min = 4294967281
      0xFFU, 0xFFU, 0xFFU, 0xFFU, 0x0FU  /// Max = 4294967295
  };
  Mgr.setVector(Vec6);
  AST::TableType Tab6;
  EXPECT_TRUE(Tab6.loadBinary(Mgr) && Mgr.getQueueSize() == 0);
}

TEST(TypeTest, LoadGlobalType) {
  /// 1. Test load global type.
  ///
  ///   1.  Load invalid empty global type.
  ///   2.  Load invalid global type without mutation.
  ///   3.  Load invalid mutation of global type.
  ///   4.  Load valid global type.
  Mgr.clearBuffer();
  AST::GlobalType Glb1;
  EXPECT_FALSE(Glb1.loadBinary(Mgr));

  Mgr.clearBuffer();
  std::vector<unsigned char> Vec2 = {
      0x7CU /// F64 value type
  };
  Mgr.setVector(Vec2);
  AST::GlobalType Glb2;
  EXPECT_FALSE(Glb2.loadBinary(Mgr));

  Mgr.clearBuffer();
  std::vector<unsigned char> Vec3 = {
      0x7CU, /// F64 value type
      0xFFU  /// Invalid mutation type
  };
  Mgr.setVector(Vec3);
  AST::GlobalType Glb3;
  EXPECT_FALSE(Glb3.loadBinary(Mgr));

  Mgr.clearBuffer();
  std::vector<unsigned char> Vec4 = {
      0x7CU, /// F64 value type
      0x00U  /// Const mutation
  };
  Mgr.setVector(Vec4);
  AST::GlobalType Glb4;
  EXPECT_TRUE(Glb4.loadBinary(Mgr) && Mgr.getQueueSize() == 0);
}

} // namespace