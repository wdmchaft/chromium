// Copyright (c) 2006-2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/gpu/gpu_info_collector.h"

#include <dlfcn.h>
#include <vector>

#include "app/gfx/gl/gl_bindings.h"
#include "app/gfx/gl/gl_context.h"
#include "app/gfx/gl/gl_implementation.h"
#include "base/logging.h"
#include "base/scoped_ptr.h"
#include "base/string_util.h"

namespace {

// PciDevice and PciAccess are defined to access libpci functions.  Their
// members match the corresponding structures defined by libpci in size up to
// fields we may access.  For those members we don't use, their names are
// defined as "fieldX", etc., or, left out if they are declared after the
// members we care about in libpci.

struct PciDevice {
  PciDevice* next;

  uint16 field0;
  uint8 field1;
  uint8 field2;
  uint8 field3;
  int field4;

  uint16 vendor_id;
  uint16 device_id;
  uint16 device_class;
};

struct PciAccess {
  unsigned int field0;
  int field1;
  int field2;
  char* field3;
  int field4;
  int field5;
  unsigned int field6;
  int field7;

  void (*function0)();
  void (*function1)();
  void (*function2)();

  PciDevice* device_list;
};

// Define function types.
typedef PciAccess* (*FT_pci_alloc)();
typedef void (*FT_pci_init)(PciAccess*);
typedef void (*FT_pci_cleanup)(PciAccess*);
typedef void (*FT_pci_scan_bus)(PciAccess*);
typedef void (*FT_pci_scan_bus)(PciAccess*);
typedef int (*FT_pci_fill_info)(PciDevice*, int);
typedef char* (*FT_pci_lookup_name)(PciAccess*, char*, int, int, ...);

// This includes dynamically linked library handle and functions pointers from
// libpci.
struct PciInterface {
  void* lib_handle;

  FT_pci_alloc pci_alloc;
  FT_pci_init pci_init;
  FT_pci_cleanup pci_cleanup;
  FT_pci_scan_bus pci_scan_bus;
  FT_pci_fill_info pci_fill_info;
  FT_pci_lookup_name pci_lookup_name;
};

// This dynamically opens libpci and get function pointers we need.  Return
// NULL if library fails to open or any functions can not be located.
// Returned interface (if not NULL) should be deleted in FinalizeLibPci.
PciInterface* InitializeLibPci(const char* lib_name) {
  void* handle = dlopen(lib_name, RTLD_LAZY);
  if (handle == NULL) {
    LOG(ERROR) << "Fail to dlopen libpci";
    return NULL;
  }
  PciInterface* interface = new struct PciInterface;
  interface->lib_handle = handle;
  interface->pci_alloc = reinterpret_cast<FT_pci_alloc>(
      dlsym(handle, "pci_alloc"));
  interface->pci_init = reinterpret_cast<FT_pci_init>(
      dlsym(handle, "pci_init"));
  interface->pci_cleanup = reinterpret_cast<FT_pci_cleanup>(
      dlsym(handle, "pci_cleanup"));
  interface->pci_scan_bus = reinterpret_cast<FT_pci_scan_bus>(
      dlsym(handle, "pci_scan_bus"));
  interface->pci_fill_info = reinterpret_cast<FT_pci_fill_info>(
      dlsym(handle, "pci_fill_info"));
  interface->pci_lookup_name = reinterpret_cast<FT_pci_lookup_name>(
      dlsym(handle, "pci_lookup_name"));
  if (interface->pci_alloc == NULL ||
      interface->pci_init == NULL ||
      interface->pci_cleanup == NULL ||
      interface->pci_scan_bus == NULL ||
      interface->pci_fill_info == NULL ||
      interface->pci_lookup_name == NULL) {
    LOG(ERROR) << "Missing required function(s) from libpci";
    dlclose(handle);
    delete interface;
    return NULL;
  }
  return interface;
}

// This close the dynamically opened libpci and delete the interface.
void FinalizeLibPci(PciInterface*& interface) {
  DCHECK(interface != NULL && interface->lib_handle != NULL);
  dlclose(interface->lib_handle);
  delete interface;
  interface = NULL;
}

// This creates an offscreen GL context for gl queries.  Returned GLContext
// should be deleted in FinalizeGLContext.
gfx::GLContext* InitializeGLContext() {
  if (!gfx::GLContext::InitializeOneOff()) {
    LOG(ERROR) << "gfx::GLContext::InitializeOneOff() failed";
    return NULL;
  }
  gfx::GLContext* context = gfx::GLContext::CreateOffscreenGLContext(NULL);
  if (context == NULL) {
    LOG(ERROR) << "gfx::GLContext::CreateOffscreenGLContext(NULL) failed";
    return NULL;
  }
  if (!context->MakeCurrent()) {
    LOG(ERROR) << "gfx::GLContext::MakeCurrent() failed";
    context->Destroy();
    delete context;
    return NULL;
  }
  return context;
}

// This destroy and delete the GL context.
void FinalizeGLContext(gfx::GLContext*& context) {
  DCHECK(context != NULL);
  context->Destroy();
  delete context;
  context = NULL;
}

}  // namespace anonymous

namespace gpu_info_collector {

bool CollectGraphicsInfo(GPUInfo* gpu_info) {
  gfx::GLContext* context = InitializeGLContext();
  if (context == NULL)
    return false;

  // TODO(zmo): collect driver version, pixel shader version, vertex shader
  // version, and gl version.
  std::wstring driver_version = L"";
  uint32 pixel_shader_version = 0;
  uint32 vertex_shader_version = 0;
  uint32 gl_version = 0;
  bool can_lose_context =
      (gfx::GetGLImplementation() == gfx::kGLImplementationEGLGLES2);

  // TODO(zmo): be more flexible about library name.
  PciInterface* interface = InitializeLibPci("libpci.so.3");
  if (interface == NULL) {
    FinalizeGLContext(context);
    return false;
  }

  PciAccess* access = (interface->pci_alloc)();
  DCHECK(access != NULL);
  (interface->pci_init)(access);
  (interface->pci_scan_bus)(access);
  std::vector<PciDevice*> gpu_list;
  PciDevice* gpu_active = NULL;
  for (PciDevice* device = access->device_list;
       device != NULL; device = device->next) {
    (interface->pci_fill_info)(device, 33);  // Fill the IDs and class fields.
    if (device->device_class == 0x0300) {  // Device class is DISPLAY_VGA.
      gpu_list.push_back(device);
    }
  }
  if (gpu_list.size() == 1) {
    gpu_active = gpu_list[0];
  } else {
    // If more than one graphics card are identified, find the one that matches
    // gl VENDOR and RENDERER info.
    std::string gl_vendor_string =
        reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    std::string gl_renderer_string =
        reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    const int buffer_size = 255;
    scoped_array<char> buffer(new char[buffer_size]);
    std::vector<PciDevice*> candidates;
    for (size_t i = 0; i < gpu_list.size(); ++i) {
      PciDevice* gpu = gpu_list[i];
      // The current implementation of pci_lookup_name returns the same pointer
      // as the passed in upon success, and a different one (NULL or a pointer
      // to an error message) upon failure.
      if ((interface->pci_lookup_name)(access,
                                       buffer.get(),
                                       buffer_size,
                                       1,
                                       gpu->vendor_id) != buffer.get())
        continue;
      std::string vendor_string = buffer.get();
      const bool kCaseSensitive = false;
      if (!StartsWithASCII(gl_vendor_string, vendor_string, kCaseSensitive))
        continue;
      if ((interface->pci_lookup_name)(access,
                                       buffer.get(),
                                       buffer_size,
                                       2,
                                       gpu->vendor_id,
                                       gpu->device_id) != buffer.get())
        continue;
      std::string device_string = buffer.get();
      size_t begin = device_string.find_first_of('[');
      size_t end = device_string.find_last_of(']');
      if (begin != std::string::npos && end != std::string::npos &&
          begin < end) {
        device_string = device_string.substr(begin + 1, end - begin - 1);
      }
      if (StartsWithASCII(gl_renderer_string, device_string, kCaseSensitive)) {
        gpu_active = gpu;
        break;
      }
      // If a device's vendor matches gl VENDOR string, we want to consider the
      // possibility that libpci may not return the exact same name as gl
      // RENDERER string.
      candidates.push_back(gpu);
    }
    if (gpu_active == NULL && candidates.size() == 1)
      gpu_active = candidates[0];
  }
  if (gpu_active != NULL) {
    gpu_info->SetGraphicsInfo(gpu_active->vendor_id,
                              gpu_active->device_id,
                              driver_version,
                              pixel_shader_version,
                              vertex_shader_version,
                              gl_version,
                              can_lose_context);
    gpu_info->SetProgress(GPUInfo::kComplete);
  }
  (interface->pci_cleanup)(access);
  FinalizeLibPci(interface);
  FinalizeGLContext(context);
  return (gpu_active != NULL);
}

}  // namespace gpu_info_collector
