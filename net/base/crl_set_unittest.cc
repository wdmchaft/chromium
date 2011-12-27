// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/base/crl_set.h"
#include "testing/gtest/include/gtest/gtest.h"

// These data blocks were generated using a lot of code that is still in
// development. For now, if you need to update them, you have to contact agl.
static const uint8 kGIACRLSet[] = {
  0x4e, 0x00, 0x7b, 0x22, 0x56, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, 0x22, 0x3a,
  0x30, 0x2c, 0x22, 0x43, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74, 0x54, 0x79, 0x70,
  0x65, 0x22, 0x3a, 0x22, 0x43, 0x52, 0x4c, 0x53, 0x65, 0x74, 0x22, 0x2c, 0x22,
  0x53, 0x65, 0x71, 0x75, 0x65, 0x6e, 0x63, 0x65, 0x22, 0x3a, 0x30, 0x2c, 0x22,
  0x44, 0x65, 0x6c, 0x74, 0x61, 0x46, 0x72, 0x6f, 0x6d, 0x22, 0x3a, 0x30, 0x2c,
  0x22, 0x4e, 0x75, 0x6d, 0x50, 0x61, 0x72, 0x65, 0x6e, 0x74, 0x73, 0x22, 0x3a,
  0x31, 0x7d, 0xb6, 0xb9, 0x54, 0x32, 0xab, 0xae, 0x57, 0xfe, 0x02, 0x0c, 0xb2,
  0xb7, 0x4f, 0x4f, 0x9f, 0x91, 0x73, 0xc8, 0xc7, 0x08, 0xaf, 0xc9, 0xe7, 0x32,
  0xac, 0xe2, 0x32, 0x79, 0x04, 0x7c, 0x6d, 0x05, 0x0d, 0x00, 0x00, 0x00, 0x0a,
  0x10, 0x0d, 0x7f, 0x30, 0x00, 0x03, 0x00, 0x00, 0x23, 0xb0, 0x0a, 0x10, 0x0e,
  0x37, 0x06, 0x00, 0x03, 0x00, 0x00, 0x23, 0xb1, 0x0a, 0x16, 0x25, 0x42, 0x54,
  0x00, 0x03, 0x00, 0x00, 0x14, 0x51, 0x0a, 0x16, 0x69, 0xd1, 0xd7, 0x00, 0x03,
  0x00, 0x00, 0x14, 0x52, 0x0a, 0x16, 0x70, 0x8c, 0x22, 0x00, 0x03, 0x00, 0x00,
  0x14, 0x53, 0x0a, 0x16, 0x71, 0x31, 0x2c, 0x00, 0x03, 0x00, 0x00, 0x14, 0x54,
  0x0a, 0x16, 0x7d, 0x75, 0x9d, 0x00, 0x03, 0x00, 0x00, 0x14, 0x55, 0x0a, 0x1f,
  0xee, 0xf9, 0x49, 0x00, 0x03, 0x00, 0x00, 0x23, 0xae, 0x0a, 0x1f, 0xfc, 0xd1,
  0x89, 0x00, 0x03, 0x00, 0x00, 0x23, 0xaf, 0x0a, 0x61, 0xdd, 0xc7, 0x48, 0x00,
  0x03, 0x00, 0x00, 0x18, 0x0e, 0x0a, 0x61, 0xe6, 0x12, 0x64, 0x00, 0x03, 0x00,
  0x00, 0x18, 0x0f, 0x0a, 0x61, 0xe9, 0x46, 0x56, 0x00, 0x03, 0x00, 0x00, 0x18,
  0x10, 0x0a, 0x64, 0x63, 0x49, 0xd2, 0x00, 0x03, 0x00, 0x00, 0x1d, 0x77,
};

static const uint8 kNoopDeltaCRL[] = {
  0xc3, 0x00, 0x7b, 0x22, 0x56, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, 0x22, 0x3a,
  0x30, 0x2c, 0x22, 0x43, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74, 0x54, 0x79, 0x70,
  0x65, 0x22, 0x3a, 0x22, 0x43, 0x52, 0x4c, 0x53, 0x65, 0x74, 0x44, 0x65, 0x6c,
  0x74, 0x61, 0x22, 0x2c, 0x22, 0x53, 0x65, 0x71, 0x75, 0x65, 0x6e, 0x63, 0x65,
  0x22, 0x3a, 0x30, 0x2c, 0x22, 0x4e, 0x65, 0x78, 0x74, 0x55, 0x70, 0x64, 0x61,
  0x74, 0x65, 0x22, 0x3a, 0x31, 0x33, 0x31, 0x31, 0x31, 0x32, 0x33, 0x37, 0x39,
  0x33, 0x2c, 0x22, 0x57, 0x69, 0x6e, 0x64, 0x6f, 0x77, 0x53, 0x65, 0x63, 0x73,
  0x22, 0x3a, 0x34, 0x33, 0x32, 0x30, 0x30, 0x2c, 0x22, 0x44, 0x65, 0x6c, 0x74,
  0x61, 0x46, 0x72, 0x6f, 0x6d, 0x22, 0x3a, 0x30, 0x2c, 0x22, 0x4e, 0x75, 0x6d,
  0x50, 0x61, 0x72, 0x65, 0x6e, 0x74, 0x73, 0x22, 0x3a, 0x31, 0x2c, 0x22, 0x53,
  0x69, 0x67, 0x6e, 0x69, 0x6e, 0x67, 0x50, 0x75, 0x62, 0x6c, 0x69, 0x63, 0x4b,
  0x65, 0x79, 0x22, 0x3a, 0x22, 0x22, 0x2c, 0x22, 0x53, 0x69, 0x67, 0x6e, 0x69,
  0x6e, 0x67, 0x4b, 0x65, 0x79, 0x53, 0x69, 0x67, 0x6e, 0x61, 0x74, 0x75, 0x72,
  0x65, 0x22, 0x3a, 0x22, 0x22, 0x2c, 0x22, 0x50, 0x61, 0x79, 0x6c, 0x6f, 0x61,
  0x64, 0x53, 0x69, 0x67, 0x6e, 0x61, 0x74, 0x75, 0x72, 0x65, 0x22, 0x3a, 0x22,
  0x22, 0x7d, 0x01, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x78, 0x9c, 0x62,
  0x00, 0x04, 0x00, 0x00, 0xff, 0xff, 0x00, 0x01, 0x00, 0x01,
};

static const uint8 kAddCRLDelta[] = {
  0xc3, 0x00, 0x7b, 0x22, 0x56, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, 0x22, 0x3a,
  0x30, 0x2c, 0x22, 0x43, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74, 0x54, 0x79, 0x70,
  0x65, 0x22, 0x3a, 0x22, 0x43, 0x52, 0x4c, 0x53, 0x65, 0x74, 0x44, 0x65, 0x6c,
  0x74, 0x61, 0x22, 0x2c, 0x22, 0x53, 0x65, 0x71, 0x75, 0x65, 0x6e, 0x63, 0x65,
  0x22, 0x3a, 0x30, 0x2c, 0x22, 0x4e, 0x65, 0x78, 0x74, 0x55, 0x70, 0x64, 0x61,
  0x74, 0x65, 0x22, 0x3a, 0x31, 0x33, 0x31, 0x31, 0x31, 0x32, 0x35, 0x39, 0x34,
  0x38, 0x2c, 0x22, 0x57, 0x69, 0x6e, 0x64, 0x6f, 0x77, 0x53, 0x65, 0x63, 0x73,
  0x22, 0x3a, 0x34, 0x33, 0x32, 0x30, 0x30, 0x2c, 0x22, 0x44, 0x65, 0x6c, 0x74,
  0x61, 0x46, 0x72, 0x6f, 0x6d, 0x22, 0x3a, 0x30, 0x2c, 0x22, 0x4e, 0x75, 0x6d,
  0x50, 0x61, 0x72, 0x65, 0x6e, 0x74, 0x73, 0x22, 0x3a, 0x32, 0x2c, 0x22, 0x53,
  0x69, 0x67, 0x6e, 0x69, 0x6e, 0x67, 0x50, 0x75, 0x62, 0x6c, 0x69, 0x63, 0x4b,
  0x65, 0x79, 0x22, 0x3a, 0x22, 0x22, 0x2c, 0x22, 0x53, 0x69, 0x67, 0x6e, 0x69,
  0x6e, 0x67, 0x4b, 0x65, 0x79, 0x53, 0x69, 0x67, 0x6e, 0x61, 0x74, 0x75, 0x72,
  0x65, 0x22, 0x3a, 0x22, 0x22, 0x2c, 0x22, 0x50, 0x61, 0x79, 0x6c, 0x6f, 0x61,
  0x64, 0x53, 0x69, 0x67, 0x6e, 0x61, 0x74, 0x75, 0x72, 0x65, 0x22, 0x3a, 0x22,
  0x22, 0x7d, 0x02, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x78, 0x9c, 0x62,
  0x60, 0x04, 0x04, 0x00, 0x00, 0xff, 0xff, 0x00, 0x03, 0x00, 0x02, 0xe4, 0x2f,
  0x24, 0xbd, 0x4d, 0x37, 0xf4, 0xaa, 0x2e, 0x56, 0xb9, 0x79, 0xd8, 0x3d, 0x1e,
  0x65, 0x21, 0x9f, 0xe0, 0xe9, 0xe3, 0xa3, 0x82, 0xa1, 0xb3, 0xcb, 0x66, 0xc9,
  0x39, 0x55, 0xde, 0x75, 0x0c, 0x00, 0x00, 0x00, 0x01, 0x02, 0x01, 0x03, 0x01,
  0x04, 0x01, 0x05, 0x01, 0x06, 0x01, 0x07, 0x01, 0x08, 0x01, 0x09, 0x01, 0x2f,
  0x01, 0x30, 0x01, 0x31, 0x01, 0x32,
};

static const uint8 kRemoveCRLDelta[] = {
  0xc3, 0x00, 0x7b, 0x22, 0x56, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, 0x22, 0x3a,
  0x30, 0x2c, 0x22, 0x43, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74, 0x54, 0x79, 0x70,
  0x65, 0x22, 0x3a, 0x22, 0x43, 0x52, 0x4c, 0x53, 0x65, 0x74, 0x44, 0x65, 0x6c,
  0x74, 0x61, 0x22, 0x2c, 0x22, 0x53, 0x65, 0x71, 0x75, 0x65, 0x6e, 0x63, 0x65,
  0x22, 0x3a, 0x30, 0x2c, 0x22, 0x4e, 0x65, 0x78, 0x74, 0x55, 0x70, 0x64, 0x61,
  0x74, 0x65, 0x22, 0x3a, 0x31, 0x33, 0x31, 0x31, 0x31, 0x32, 0x36, 0x31, 0x31,
  0x36, 0x2c, 0x22, 0x57, 0x69, 0x6e, 0x64, 0x6f, 0x77, 0x53, 0x65, 0x63, 0x73,
  0x22, 0x3a, 0x34, 0x33, 0x32, 0x30, 0x30, 0x2c, 0x22, 0x44, 0x65, 0x6c, 0x74,
  0x61, 0x46, 0x72, 0x6f, 0x6d, 0x22, 0x3a, 0x30, 0x2c, 0x22, 0x4e, 0x75, 0x6d,
  0x50, 0x61, 0x72, 0x65, 0x6e, 0x74, 0x73, 0x22, 0x3a, 0x31, 0x2c, 0x22, 0x53,
  0x69, 0x67, 0x6e, 0x69, 0x6e, 0x67, 0x50, 0x75, 0x62, 0x6c, 0x69, 0x63, 0x4b,
  0x65, 0x79, 0x22, 0x3a, 0x22, 0x22, 0x2c, 0x22, 0x53, 0x69, 0x67, 0x6e, 0x69,
  0x6e, 0x67, 0x4b, 0x65, 0x79, 0x53, 0x69, 0x67, 0x6e, 0x61, 0x74, 0x75, 0x72,
  0x65, 0x22, 0x3a, 0x22, 0x22, 0x2c, 0x22, 0x50, 0x61, 0x79, 0x6c, 0x6f, 0x61,
  0x64, 0x53, 0x69, 0x67, 0x6e, 0x61, 0x74, 0x75, 0x72, 0x65, 0x22, 0x3a, 0x22,
  0x22, 0x7d, 0x02, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x78, 0x9c, 0x62,
  0x60, 0x02, 0x04, 0x00, 0x00, 0xff, 0xff, 0x00, 0x04, 0x00, 0x03,
};

static const uint8 kUpdateSerialsDelta[] = {
  0xc3, 0x00, 0x7b, 0x22, 0x56, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, 0x22, 0x3a,
  0x30, 0x2c, 0x22, 0x43, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74, 0x54, 0x79, 0x70,
  0x65, 0x22, 0x3a, 0x22, 0x43, 0x52, 0x4c, 0x53, 0x65, 0x74, 0x44, 0x65, 0x6c,
  0x74, 0x61, 0x22, 0x2c, 0x22, 0x53, 0x65, 0x71, 0x75, 0x65, 0x6e, 0x63, 0x65,
  0x22, 0x3a, 0x30, 0x2c, 0x22, 0x4e, 0x65, 0x78, 0x74, 0x55, 0x70, 0x64, 0x61,
  0x74, 0x65, 0x22, 0x3a, 0x31, 0x33, 0x31, 0x31, 0x31, 0x32, 0x36, 0x34, 0x36,
  0x31, 0x2c, 0x22, 0x57, 0x69, 0x6e, 0x64, 0x6f, 0x77, 0x53, 0x65, 0x63, 0x73,
  0x22, 0x3a, 0x34, 0x33, 0x32, 0x30, 0x30, 0x2c, 0x22, 0x44, 0x65, 0x6c, 0x74,
  0x61, 0x46, 0x72, 0x6f, 0x6d, 0x22, 0x3a, 0x30, 0x2c, 0x22, 0x4e, 0x75, 0x6d,
  0x50, 0x61, 0x72, 0x65, 0x6e, 0x74, 0x73, 0x22, 0x3a, 0x31, 0x2c, 0x22, 0x53,
  0x69, 0x67, 0x6e, 0x69, 0x6e, 0x67, 0x50, 0x75, 0x62, 0x6c, 0x69, 0x63, 0x4b,
  0x65, 0x79, 0x22, 0x3a, 0x22, 0x22, 0x2c, 0x22, 0x53, 0x69, 0x67, 0x6e, 0x69,
  0x6e, 0x67, 0x4b, 0x65, 0x79, 0x53, 0x69, 0x67, 0x6e, 0x61, 0x74, 0x75, 0x72,
  0x65, 0x22, 0x3a, 0x22, 0x22, 0x2c, 0x22, 0x50, 0x61, 0x79, 0x6c, 0x6f, 0x61,
  0x64, 0x53, 0x69, 0x67, 0x6e, 0x61, 0x74, 0x75, 0x72, 0x65, 0x22, 0x3a, 0x22,
  0x22, 0x7d, 0x01, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x78, 0x9c, 0x62,
  0x06, 0x04, 0x00, 0x00, 0xff, 0xff, 0x00, 0x04, 0x00, 0x04, 0x2d, 0x00, 0x00,
  0x00, 0x15, 0x00, 0x00, 0x00, 0x78, 0x9c, 0x62, 0x80, 0x00, 0x46, 0x2c, 0x00,
  0x45, 0x14, 0xac, 0x08, 0x10, 0x00, 0x00, 0xff, 0xff, 0x02, 0xe1, 0x00, 0x21,
  0x0a, 0x10, 0x0d, 0x7f, 0x30, 0x00, 0x03, 0x00, 0x00, 0x23, 0xb0, 0x0a, 0x10,
  0x0d, 0x7f, 0x30, 0x00, 0x03, 0x00, 0x00, 0x23, 0xb0, 0x0a, 0x10, 0x0d, 0x7f,
  0x30, 0x00, 0x03, 0x00, 0x00, 0x23, 0xb0, 0x0a, 0x10, 0x0d, 0x7f, 0x30, 0x00,
  0x03, 0x00, 0x00, 0x23, 0xb0, 0x0a, 0x10, 0x0d, 0x7f, 0x30, 0x00, 0x03, 0x00,
  0x00, 0x23, 0xb0, 0x0a, 0x10, 0x0d, 0x7f, 0x30, 0x00, 0x03, 0x00, 0x00, 0x23,
  0xb0, 0x0a, 0x10, 0x0d, 0x7f, 0x30, 0x00, 0x03, 0x00, 0x00, 0x23, 0xb0, 0x0a,
  0x10, 0x0d, 0x7f, 0x30, 0x00, 0x03, 0x00, 0x00, 0x23, 0xb0, 0x0a, 0x10, 0x0d,
  0x7f, 0x30, 0x00, 0x03, 0x00, 0x00, 0x23, 0xb0, 0x0a, 0x10, 0x0d, 0x7f, 0x30,
  0x00, 0x03, 0x00, 0x00, 0x23, 0xb0, 0x0a, 0x10, 0x0d, 0x7f, 0x30, 0x00, 0x03,
  0x00, 0x00, 0x23, 0xb0, 0x0a, 0x10, 0x0d, 0x7f, 0x30, 0x00, 0x03, 0x00, 0x00,
  0x23, 0xb0, 0x0a, 0x10, 0x0d, 0x7f, 0x30, 0x00, 0x03, 0x00, 0x00, 0x23, 0xb0,
  0x0a, 0x10, 0x0d, 0x7f, 0x30, 0x00, 0x03, 0x00, 0x00, 0x23, 0xb0, 0x0a, 0x10,
  0x0d, 0x7f, 0x30, 0x00, 0x03, 0x00, 0x00, 0x23, 0xb0, 0x0a, 0x10, 0x0d, 0x7f,
  0x30, 0x00, 0x03, 0x00, 0x00, 0x23, 0xb0, 0x0a, 0x10, 0x0d, 0x7f, 0x30, 0x00,
  0x03, 0x00, 0x00, 0x23, 0xb0, 0x0a, 0x10, 0x0d, 0x7f, 0x30, 0x00, 0x03, 0x00,
  0x00, 0x23, 0xb0, 0x0a, 0x10, 0x0d, 0x7f, 0x30, 0x00, 0x03, 0x00, 0x00, 0x23,
  0xb0, 0x0a, 0x10, 0x0d, 0x7f, 0x30, 0x00, 0x03, 0x00, 0x00, 0x23, 0xb0, 0x0a,
  0x10, 0x0d, 0x7f, 0x30, 0x00, 0x03, 0x00, 0x00, 0x23, 0xb0, 0x0a, 0x10, 0x0d,
  0x7f, 0x30, 0x00, 0x03, 0x00, 0x00, 0x23, 0xb0, 0x0a, 0x10, 0x0d, 0x7f, 0x30,
  0x00, 0x03, 0x00, 0x00, 0x23, 0xb0, 0x0a, 0x10, 0x0d, 0x7f, 0x30, 0x00, 0x03,
  0x00, 0x00, 0x23, 0xb0, 0x0a, 0x10, 0x0d, 0x7f, 0x30, 0x00, 0x03, 0x00, 0x00,
  0x23, 0xb0, 0x0a, 0x10, 0x0d, 0x7f, 0x30, 0x00, 0x03, 0x00, 0x00, 0x23, 0xb0,
  0x0a, 0x10, 0x0d, 0x7f, 0x30, 0x00, 0x03, 0x00, 0x00, 0x23, 0xb0, 0x0a, 0x10,
  0x0d, 0x7f, 0x30, 0x00, 0x03, 0x00, 0x00, 0x23, 0xb0, 0x0a, 0x10, 0x0d, 0x7f,
  0x30, 0x00, 0x03, 0x00, 0x00, 0x23, 0xb0, 0x0a, 0x10, 0x0d, 0x7f, 0x30, 0x00,
  0x03, 0x00, 0x00, 0x23, 0xb0, 0x0a, 0x10, 0x0d, 0x7f, 0x30, 0x00, 0x03, 0x00,
  0x00, 0x23, 0xb0, 0x0a, 0x10, 0x0d, 0x7f, 0x30, 0x00, 0x03, 0x00, 0x00, 0x23,
  0xb0,
};

// kGIASPKISHA256 is the SHA256 digest the Google Internet Authority's
// SubjectPublicKeyInfo.
static const uint8 kGIASPKISHA256[32] = {
  0xb6, 0xb9, 0x54, 0x32, 0xab, 0xae, 0x57, 0xfe, 0x02, 0x0c, 0xb2, 0xb7, 0x4f,
  0x4f, 0x9f, 0x91, 0x73, 0xc8, 0xc7, 0x08, 0xaf, 0xc9, 0xe7, 0x32, 0xac, 0xe2,
  0x32, 0x79, 0x04, 0x7c, 0x6d, 0x05,
};

TEST(CRLSetTest, Parse) {
  base::StringPiece s(reinterpret_cast<const char*>(kGIACRLSet),
                      sizeof(kGIACRLSet));
  scoped_refptr<net::CRLSet> set;
  EXPECT_TRUE(net::CRLSet::Parse(s, &set));
  ASSERT_TRUE(set.get() != NULL);

  const net::CRLSet::CRLList& crls = set->crls();
  ASSERT_EQ(1u, crls.size());
  const std::vector<std::string>& serials = crls[0].second;
  static const unsigned kExpectedNumSerials = 13;
  ASSERT_EQ(kExpectedNumSerials, serials.size());
  EXPECT_EQ(std::string("\x10\x0D\x7F\x30\x00\x03\x00\x00\x23\xB0", 10),
            serials[0]);
  EXPECT_EQ(std::string("\x64\x63\x49\xD2\x00\x03\x00\x00\x1D\x77", 10),
            serials[kExpectedNumSerials - 1]);

  const std::string gia_spki_hash(
      reinterpret_cast<const char*>(kGIASPKISHA256),
      sizeof(kGIASPKISHA256));
  EXPECT_EQ(net::CRLSet::REVOKED, set->CheckCertificate(
      std::string("\x16\x7D\x75\x9D\x00\x03\x00\x00\x14\x55", 10),
      gia_spki_hash));
  EXPECT_EQ(net::CRLSet::GOOD, set->CheckCertificate(
      std::string("\x47\x54\x3E\x79\x00\x03\x00\x00\x14\xF5", 10),
      gia_spki_hash));
}

TEST(CRLSetTest, NoOpDeltaUpdate) {
  base::StringPiece s(reinterpret_cast<const char*>(kGIACRLSet),
                      sizeof(kGIACRLSet));
  scoped_refptr<net::CRLSet> set;
  EXPECT_TRUE(net::CRLSet::Parse(s, &set));
  ASSERT_TRUE(set.get() != NULL);

  scoped_refptr<net::CRLSet> delta_set;
  base::StringPiece delta(reinterpret_cast<const char*>(kNoopDeltaCRL),
                          sizeof(kNoopDeltaCRL));
  EXPECT_TRUE(set->ApplyDelta(delta, &delta_set));
  ASSERT_TRUE(delta_set.get() != NULL);

  std::string out = delta_set->Serialize();
  EXPECT_EQ(s.as_string(), out);
}

TEST(CRLSetTest, AddCRLDelta) {
  base::StringPiece s(reinterpret_cast<const char*>(kGIACRLSet),
                      sizeof(kGIACRLSet));
  scoped_refptr<net::CRLSet> set;
  EXPECT_TRUE(net::CRLSet::Parse(s, &set));
  ASSERT_TRUE(set.get() != NULL);

  scoped_refptr<net::CRLSet> delta_set;
  base::StringPiece delta(reinterpret_cast<const char*>(kAddCRLDelta),
                          sizeof(kAddCRLDelta));
  EXPECT_TRUE(set->ApplyDelta(delta, &delta_set));
  ASSERT_TRUE(delta_set.get() != NULL);

  const net::CRLSet::CRLList& crls = delta_set->crls();
  ASSERT_EQ(2u, crls.size());
  const std::vector<std::string>& serials = crls[1].second;
  ASSERT_EQ(12u, serials.size());
  EXPECT_EQ(std::string("\x02", 1), serials[0]);
  EXPECT_EQ(std::string("\x03", 1), serials[1]);
  EXPECT_EQ(std::string("\x04", 1), serials[2]);
}

TEST(CRLSetTest, AddRemoveCRLDelta) {
  base::StringPiece s(reinterpret_cast<const char*>(kGIACRLSet),
                      sizeof(kGIACRLSet));
  scoped_refptr<net::CRLSet> set;
  EXPECT_TRUE(net::CRLSet::Parse(s, &set));
  ASSERT_TRUE(set.get() != NULL);

  scoped_refptr<net::CRLSet> delta_set;
  base::StringPiece delta(reinterpret_cast<const char*>(kAddCRLDelta),
                          sizeof(kAddCRLDelta));
  EXPECT_TRUE(set->ApplyDelta(delta, &delta_set));
  ASSERT_TRUE(delta_set.get() != NULL);

  scoped_refptr<net::CRLSet> delta2_set;
  base::StringPiece delta2(reinterpret_cast<const char*>(kRemoveCRLDelta),
                           sizeof(kRemoveCRLDelta));
  EXPECT_TRUE(delta_set->ApplyDelta(delta2, &delta2_set));
  ASSERT_TRUE(delta2_set.get() != NULL);

  const net::CRLSet::CRLList& crls = delta2_set->crls();
  ASSERT_EQ(1u, crls.size());
  const std::vector<std::string>& serials = crls[0].second;
  ASSERT_EQ(13u, serials.size());
}

TEST(CRLSetTest, UpdateSerialsDelta) {
  base::StringPiece s(reinterpret_cast<const char*>(kGIACRLSet),
                      sizeof(kGIACRLSet));
  scoped_refptr<net::CRLSet> set;
  EXPECT_TRUE(net::CRLSet::Parse(s, &set));
  ASSERT_TRUE(set.get() != NULL);

  scoped_refptr<net::CRLSet> delta_set;
  base::StringPiece delta(reinterpret_cast<const char*>(kUpdateSerialsDelta),
                          sizeof(kUpdateSerialsDelta));
  EXPECT_TRUE(set->ApplyDelta(delta, &delta_set));
  ASSERT_TRUE(delta_set.get() != NULL);

  const net::CRLSet::CRLList& crls = delta_set->crls();
  ASSERT_EQ(1u, crls.size());
  const std::vector<std::string>& serials = crls[0].second;
  EXPECT_EQ(45u, serials.size());
}
