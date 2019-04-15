/*
 * Copyright (C) 2017-2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#pragma once
#include "runtime/sharings/sharing.h"
#include "runtime/sharings/va/va_sharing_defines.h"

#include <functional>

namespace NEO {
class VASharingFunctions : public SharingFunctions {
  public:
    VASharingFunctions(VADisplay vaDisplay);
    ~VASharingFunctions() override;

    uint32_t getId() const override {
        return VASharingFunctions::sharingId;
    }
    static const uint32_t sharingId;

    MOCKABLE_VIRTUAL bool isValidVaDisplay() {
        return vaDisplayIsValidPFN(vaDisplay) == 1;
    }

    MOCKABLE_VIRTUAL VAStatus deriveImage(VASurfaceID vaSurface, VAImage *vaImage) {
        return vaDeriveImagePFN(vaDisplay, vaSurface, vaImage);
    }

    MOCKABLE_VIRTUAL VAStatus destroyImage(VAImageID vaImageId) {
        return vaDestroyImagePFN(vaDisplay, vaImageId);
    }

    MOCKABLE_VIRTUAL VAStatus extGetSurfaceHandle(VASurfaceID *vaSurface, unsigned int *handleId) {
        return vaExtGetSurfaceHandlePFN(vaDisplay, vaSurface, handleId);
    }

    MOCKABLE_VIRTUAL VAStatus syncSurface(VASurfaceID vaSurface) {
        return vaSyncSurfacePFN(vaDisplay, vaSurface);
    }

    MOCKABLE_VIRTUAL void *getLibFunc(const char *func) {
        if (vaGetLibFuncPFN) {
            return vaGetLibFuncPFN(vaDisplay, func);
        }
        return nullptr;
    }

    void initFunctions();
    static std::function<void *(const char *, int)> fdlopen;
    static std::function<void *(void *handle, const char *symbol)> fdlsym;
    static std::function<int(void *handle)> fdlclose;

    static bool isVaLibraryAvailable();

  protected:
    void *libHandle = nullptr;
    VADisplay vaDisplay = nullptr;
    VADisplayIsValidPFN vaDisplayIsValidPFN = [](VADisplay vaDisplay) { return 0; };
    VADeriveImagePFN vaDeriveImagePFN;
    VADestroyImagePFN vaDestroyImagePFN;
    VASyncSurfacePFN vaSyncSurfacePFN;
    VAExtGetSurfaceHandlePFN vaExtGetSurfaceHandlePFN;
    VAGetLibFuncPFN vaGetLibFuncPFN;
};
} // namespace NEO
