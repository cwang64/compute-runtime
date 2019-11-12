/*
 * Copyright (C) 2017-2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "runtime/command_queue/command_queue_hw.h"
#include "runtime/os_interface/linux/os_interface.h"
#include "test.h"
#include "unit_tests/fixtures/ult_command_stream_receiver_fixture.h"
#include "unit_tests/mocks/linux/mock_drm_memory_manager.h"
#include "unit_tests/mocks/mock_context.h"
#include "unit_tests/os_interface/linux/drm_mock.h"
#include "unit_tests/os_interface/linux/device_command_stream_fixture.h"

using namespace OCLRT;

struct clCreateCommandQueueWithPropertiesLinux : public UltCommandStreamReceiverTest {
    void SetUp() override {
        UltCommandStreamReceiverTest::SetUp();
        ExecutionEnvironment *executionEnvironment = new ExecutionEnvironment();
        auto osInterface = new OSInterface();
        osInterface->get()->setDrm(drm.get());
        executionEnvironment->osInterface.reset(osInterface);
        executionEnvironment->memoryManager.reset(new TestedDrmMemoryManager(drm.get()));
        mdevice.reset(MockDevice::create<MockDevice>(platformDevices[0], executionEnvironment));

        clDevice = mdevice.get();
        retVal = CL_SUCCESS;
        context = std::unique_ptr<Context>(Context::create<MockContext>(nullptr, DeviceVector(&clDevice, 1), nullptr, nullptr, retVal));
    }
    void TearDown() override {
        UltCommandStreamReceiverTest::TearDown();
    }
    std::unique_ptr<DrmMock> drm = std::make_unique<DrmMock>();
    std::unique_ptr<MockDevice> mdevice = nullptr;
    std::unique_ptr<Context> context;
    cl_device_id clDevice;
    cl_int retVal;
};

namespace ULT {

TEST_F(clCreateCommandQueueWithPropertiesLinux, givenUnPossiblePropertiesWithClQueueSliceCountWhenCreateCommandQueueThenQueueNotCreated) {
    uint64_t newSliceCount = 1;
    size_t maxSliceCount;
    clGetDeviceInfo(clDevice, CL_DEVICE_SLICE_COUNT_INTEL, sizeof(size_t), &maxSliceCount, nullptr);

    newSliceCount = maxSliceCount + 1;

    cl_queue_properties properties[] = {CL_QUEUE_SLICE_COUNT_INTEL, newSliceCount, 0};
    std::unique_ptr<DrmMock> drm = std::make_unique<DrmMock>();

    mdevice.get()->getExecutionEnvironment()->osInterface->get()->setDrm(drm.get());

    cl_command_queue cmdQ = clCreateCommandQueueWithProperties(context.get(), clDevice, properties, &retVal);

    EXPECT_EQ(nullptr, cmdQ);
    EXPECT_EQ(CL_INVALID_QUEUE_PROPERTIES, retVal);
}

TEST_F(clCreateCommandQueueWithPropertiesLinux, givenZeroWithClQueueSliceCountWhenCreateCommandQueueThenSliceCountEqualDefaultSliceCount) {

    uint64_t newSliceCount = 0;

    cl_queue_properties properties[] = {CL_QUEUE_SLICE_COUNT_INTEL, newSliceCount, 0};
    std::unique_ptr<DrmMock> drm = std::make_unique<DrmMock>();

    mdevice.get()->getExecutionEnvironment()->osInterface->get()->setDrm(drm.get());

    cl_command_queue cmdQ = clCreateCommandQueueWithProperties(context.get(), clDevice, properties, &retVal);

    ASSERT_NE(nullptr, cmdQ);
    ASSERT_EQ(CL_SUCCESS, retVal);

    auto commandQueue = castToObject<CommandQueue>(cmdQ);
    EXPECT_EQ(commandQueue->getSliceCount(), QueueSliceCount::defaultSliceCount);

    retVal = clReleaseCommandQueue(cmdQ);
    EXPECT_EQ(CL_SUCCESS, retVal);
}

TEST_F(clCreateCommandQueueWithPropertiesLinux, givenPossiblePropertiesWithClQueueSliceCountWhenCreateCommandQueueThenSliceCountIsSet) {

    uint64_t newSliceCount = 1;
    size_t maxSliceCount;
    clGetDeviceInfo(clDevice, CL_DEVICE_SLICE_COUNT_INTEL, sizeof(size_t), &maxSliceCount, nullptr);
    if (maxSliceCount > 1) {
        newSliceCount = maxSliceCount - 1;
    }

    cl_queue_properties properties[] = {CL_QUEUE_SLICE_COUNT_INTEL, newSliceCount, 0};
    std::unique_ptr<DrmMock> drm = std::make_unique<DrmMock>();

    mdevice.get()->getExecutionEnvironment()->osInterface->get()->setDrm(drm.get());

    cl_command_queue cmdQ = clCreateCommandQueueWithProperties(context.get(), clDevice, properties, &retVal);

    ASSERT_NE(nullptr, cmdQ);
    ASSERT_EQ(CL_SUCCESS, retVal);

    auto commandQueue = castToObject<CommandQueue>(cmdQ);
    EXPECT_EQ(commandQueue->getSliceCount(), newSliceCount);

    retVal = clReleaseCommandQueue(cmdQ);
    EXPECT_EQ(CL_SUCCESS, retVal);
}

} // namespace ULT
